import os
import inspect
from pathlib import Path
import numpy as np
from copy import deepcopy
from matplotlib import pyplot

from tvb.basic.profile import TvbProfile
from tvb.contrib.cosimulation.cosim_monitors import CosimCoupling
from tvb.simulator.models.oscillator import Generic2dOscillator
from tvb.datatypes.connectivity import Connectivity
from tvb.contrib.cosimulation.cosimulator import CoSimulator as Simulator
from tvb.simulator.coupling import Linear
from tvb.simulator.integrators import EulerStochastic
from tvb.simulator.monitors import Raw

import tvb_data

from nrp_core.engines.python_json import EngineScript

TvbProfile.set_profile(TvbProfile.LIBRARY_PROFILE)

TVB_DATA_PATH = os.path.dirname(inspect.getabsfile(tvb_data))
DEFAULT_SUBJECT_PATH = os.path.join(TVB_DATA_PATH, "berlinSubjects", "QL_20120814")
DEFAULT_CONNECTIVITY_ZIP = os.path.join(DEFAULT_SUBJECT_PATH, "QL_20120814_Connectivity.zip")

DT = 0.1  # Time step of integration
SPEED = 3.0 # Speed of transmission of brain signals
# Global coupling scaling for non-task-specific connectivity weights
G = 0.25  # G=0.25 works, G>=0.5 destabilizes the integration
NOISE = 0.0001  # Noise std level


class Script(EngineScript):

    def _initialize_connectivity(self):
        arm_proxy_index = None
        MIN_TRACT_LENGTH = DT * SPEED

        # Human connectivity:
        connectivity = Connectivity.from_file(DEFAULT_CONNECTIVITY_ZIP)
        number_of_regions = connectivity.weights.shape[0]
        motor_regions = np.array(['precentral_L'])
        regions_labels = connectivity.region_labels.tolist()
        motor_index = np.array([regions_labels.index(mr) for mr in motor_regions])

        # Make sure that we don't get 0 delays for existing connections
        connectivity.tract_lengths = np.maximum(connectivity.tract_lengths, MIN_TRACT_LENGTH)
        # Normalize connectome:
        connectivity.weights = connectivity.scaled_weights(mode="region")
        connectivity.weights /= np.percentile(connectivity.weights, 99)
        connectivity.weights *= G   # Downscale non-task-specific connectome

        # Inject the proxy region into the brain

        connectivity.region_labels = np.concatenate([connectivity.region_labels,
                                                    np.array(["Arm"])])
        number_of_regions = connectivity.region_labels.shape[0]
        arm_proxy_index = number_of_regions - np.array([1]).astype('i')

        maxCentres = np.max(connectivity.centres, axis=0)
        connectivity.centres = np.concatenate([connectivity.centres,
                                              -maxCentres[np.newaxis],
                                               maxCentres[np.newaxis]])
        connectivity.areas = np.concatenate([connectivity.areas,
                                             np.array([connectivity.areas.min()])])
        connectivity.hemispheres = np.concatenate([connectivity.hemispheres,
                                                   np.array([True])])
        connectivity.cortical = np.concatenate([connectivity.cortical,
                                                np.array([False])])

        # Augment connectivity with the proxy region

        for attr, val in zip(["weights", "tract_lengths"], [1.0, MIN_TRACT_LENGTH]):
            prop = getattr(connectivity, attr).copy()
            prop = np.concatenate([prop, np.zeros((number_of_regions - len(motor_regions), len(motor_regions)))], axis=1)
            prop = np.concatenate([prop, np.zeros((len(motor_regions), number_of_regions))], axis=0)
            prop[arm_proxy_index[0], motor_index[0]] = val  # Motor region -> Arm
            prop[motor_index[0], arm_proxy_index[0]] = val  # Arm -> Motor region
            setattr(connectivity, attr, prop)

        connectivity.speed = np.array([SPEED])
        connectivity.configure()

        self.connectivity = connectivity
        self.iM = motor_index
        self.iF = arm_proxy_index


    def _initialize_simulator(self):
        # Create simulator, connectivity, linear coupling, integrator with noise, monitors
        simulator = Simulator()

        simulator.connectivity = self.connectivity

        simulator.coupling = Linear(a=np.array([1.0]))

        simulator.model = Generic2dOscillator()
        simulator.model.cvar = np.array([0, 1], dtype=np.int32)


        simulator.integrator = EulerStochastic()
        simulator.integrator.dt = DT
        simulator.integrator.noise.nsig = np.array([NOISE] * simulator.model.nvar)

        mon_raw = Raw(period=1.0)  # ms
        simulator.monitors = (mon_raw, )

        # Set initial conditions
        simulator.connectivity.set_idelays(simulator.integrator.dt)
        simulator.horizon = simulator.connectivity.idelays.max() + 1  # time horizon in the past due to delays
        simulator.initial_conditions = 0.1*np.ones((simulator.horizon,
                                                    simulator.model.nvar,
                                                    self.connectivity.number_of_regions,
                                                    1))

        simulator.synchronization_time = DT

        # Set CoSim simulator parameters

        simulator.voi = np.array([0, 1])  # State variables to be interchanged with the arm's cosimulator
        simulator.proxy_inds = self.iF    # The nodes simulated by the arm's cosimulator

        # Coupling from all TVB nodes, towards the nodes of the arm's cosimulator
        # is what will be transferred from TVB to the fingers:
        simulator.cosim_monitors = (CosimCoupling(), )
        simulator.cosim_monitors[0].coupling.a = np.array([1.0])
        simulator.exclusive = True  # Arm is exclusively simulated by the arm's cosimulator (opensim)

        # Configure the simulator

        simulator.configure()

        print(simulator)
        print(simulator.model)

        self.simulator = simulator


    def _initialize_tvb(self):
        self._initialize_connectivity()
        self._initialize_simulator()


    def _initialize_datapacks(self):
        self._registerDataPack("action")
        self._registerDataPack("joint_data")

        self.action = [-3.0, -3.0, -3.0, -3.0, -3.0, -3.0]
        self._setDataPack("action", { "act_list": self.action, "reset": 0})


    def initialize(self):
        self._initialize_tvb()
        self._initialize_datapacks()
        self.time_ms = 0
        self.iteration = 0
        self.prev_position = None
        self.target = []
        self.w = []
        self.prev_state = [np.array([self.simulator.current_step * self.simulator.integrator.dt]), # First and last times of the synchronization time
                                     self.simulator.initial_conditions[-1:, :, self.iF, :]]


    def simulate_fun(self, simulator, elbow_position, elbow_velocity):
        motor_commands_data = deepcopy(simulator.loop_cosim_monitor_output()[0])
        motor_commands_data[1] = motor_commands_data[1][:, :, self.iF, :]

        input = deepcopy(self.prev_state)

        # Inject arm's position into the brain
        # Meaning of indexes - [time = 0, data = 1][sample, state variable (V, W), brain region, mode (not used, always 0)]
        # Remove the scaling factor (5) to see a change in the model's behaviour
        input[1][0, 0, 0, 0] = elbow_position * 5

        # The second state variable isn't really used in the model, but it could be set up like this:
        #input[1][0, 1, 0, 0] = elbow_velocity

        # We want to inject the data 'in the past'
        input[0][0] -= simulator.integrator.dt

        # Simulate the brain

        dtres = list(simulator(cosim_updates=input))[0]

        sim_res = list(dtres)
        # For a single time point, correct the 1st dimension:
        sim_res[0][0] = np.array([sim_res[0][0]])
        sim_res[0][1] = sim_res[0][1][np.newaxis]

        self.prev_state[0] = deepcopy(motor_commands_data[0])
        self.prev_state[1] = deepcopy(motor_commands_data[1])

        return motor_commands_data, sim_res


    def calculate_velocity(self, position):
        if self.prev_position is None:
            velocity = self.simulator.initial_conditions[-1, 1, self.iF[0], 0]
            self.prev_position = position
        else:
            velocity = (position - self.prev_position) / self.simulator.integrator.dt

        self.prev_position = position

        return velocity


    def save_results(self, tempres):
        if self.time_ms == 0:
            self.results = list(tempres)
        else:
            self.results[0][0] = np.concatenate([self.results[0][0], tempres[0][0]])
            self.results[0][1] = np.concatenate([self.results[0][1], tempres[0][1]])


    def runLoop(self, timestep_ns):
        # Get elbow position

        elbow_position = self._getDataPack("joint_data")["elbow"]

        # Calculate velocity

        elbow_velocity = self.calculate_velocity(elbow_position)

        # Run the brain simulation
        # Data in tempres variable is sampled at a lower frequency, so it can't be used for setting targets

        commands, tempres = self.simulate_fun(self.simulator, elbow_position, elbow_velocity)

        # Save the results for plotting

        self.save_results(tempres)

        # Set new targets for the fingers

        target = commands[1][-1, 0, 0, 0]

        self.target.append(target)
        self.w.append(commands[1][-1, 1, 0, 0])
        # Indexes - ['TRIlong', 'TRIlat', 'TRImed', 'BIClong', 'BICshort', 'BRA']
        self._setDataPack("action", { "act_list": [-3.0, -3.0, -3.0, target, target, target], "reset": 0})

        # Increment iteration and time variables

        self.time_ms += self.simulator.integrator.dt
        self.iteration += 1
        print(self.iteration)


    def shutdown(self):
        Path("outputs").mkdir(exist_ok=True)

        pyplot.clf()
        pyplot.plot(self.w)
        pyplot.savefig('outputs/w.png')

        pyplot.clf()
        pyplot.plot(self.target)
        pyplot.savefig('outputs/target.png')

# EOF
