"""Python Engine 1. Will get current engine time and make it accessible as a datapack"""

import os
import inspect
import numpy as np

from tvb.basic.profile import TvbProfile
TvbProfile.set_profile(TvbProfile.LIBRARY_PROFILE)

from tvb.simulator.plot.config import FiguresConfig
import tvb_data

from nrp_core.engines.python_json import EngineScript,RegisterEngine

TVB_DATA_PATH = os.path.dirname(inspect.getabsfile(tvb_data))
DEFAULT_SUBJECT_PATH = os.path.join(TVB_DATA_PATH, "berlinSubjects", "QL_20120814")
DEFAULT_CONNECTIVITY_ZIP = os.path.join(DEFAULT_SUBJECT_PATH, "QL_20120814_Connectivity.zip")

# Configuring the simulation mode:
# Add "Fingers" to extend the connectome with fingers and couple M1 and S1 through them
# Add "Cosim" to simulate "Fingers" with their own simulator.
MODEL = "HKBBrainFingersCosim"  # "HKBBrain" # "HKBSensoryMotorBrain" # "HKBMotorBrain", "HKBbrain"

DT = 0.1  # Time step of integration
# Speed of transmission of brain signals:
SPEED = 3.0   # Worked for 10.0 only for the antiphase...!! and 300.0
# Global coupling scaling for non-task-specific connectivity weights
G = 0.25  # G=0.25 works, G>=0.5 destabilizes the integration
NOISE = 0.0001  # Noise std level
FREQ = [0.1, 0.2, 0.3] # Hz  # Frequencies plateaus
FREQ_CR = 0.2 # Hz     # Critical frequency where the anti->in phase transition should happen
HKB = True             # True for HKB coupling with velocities and 0 interhemispheric delays
# Initial conditions' mode. Choose antiphase to see the transition to inphase
INIT_COND = "antiphase"  # "antiphase" or "inphase"
SIMULATION_LENGTH = 500.0  #ms, for every frequency plateau

# Options for plotting and writing results to files (the latter not really being used here)
work_path = os.getcwd()
data_path = os.path.join(work_path.split("tvb_nest")[0], "data")
outputs_path = os.path.join(work_path, "outputs/%s" % MODEL)

config = FiguresConfig(out_base=outputs_path)
config.SHOW_FLAG = True
config.SAVE_FLAG = True
config.FIG_FORMAT = 'png'
config.DEFAULT_NSIG = NOISE

from tvb.simulator.plot.plotter import Plotter

plotter = Plotter(config)
plotter.config.MATPLOTLIB_BACKEND = "agg"

from matplotlib import pyplot
from tvb.datatypes.connectivity import Connectivity


from tvb.basic.neotraits.api import NArray, Final, List, Range
from tvb.simulator.models.base import Model

from tvb.contrib.cosimulation.cosimulator import CoSimulator as Simulator
from tvb.simulator.coupling import Linear
from tvb.simulator.integrators import EulerStochastic
from tvb.simulator.monitors import Raw


class HKBMBrain(Model):

    f = NArray(
        label=r":math:`f`",
        default=np.array([1.0]),
        domain=Range(lo=0.0, hi=4.0, step=0.1),
        doc="Frequency of the oscillator")

    omega = NArray(
        label=r":math:`\omega`",
        default=np.array([0.0]),
        domain=Range(lo=0.0, hi=8*np.pi, step=0.1*np.pi),
        doc="Angular frequency of the oscillator")

    f_cr = NArray(
        label=r":math:`f_cr`",
        default=np.array([2.0]),
        domain=Range(lo=1.0, hi=4.0, step=0.1),
        doc="Critical frequency of the oscillator")

    omega_cr = NArray(
        label=r":math:`\omega_cr`",
        default=np.array([0.0]),
        domain=Range(lo=0.0, hi=8*np.pi, step=0.1*np.pi),
        doc="Critical angular frequency of the oscillator")

    gamma = NArray(
        label=r":math:`\gamma`",
        default=np.array([1.0]),
        domain=Range(lo=0.0, hi=1.0, step=0.1),
        doc="\gamma parameter")

    A = NArray(
        label=r":math:`A`",
        default=np.array([1.0]),
        domain=Range(lo=0.0, hi=1.0, step=0.1),
        doc="A parameter")

    B = NArray(
        label=r":math:`B`",
        default=np.array([1.0]),
        domain=Range(lo=0.0, hi=1.0, step=0.1),
        doc="B parameter")

    alpha = NArray(
        label=r":math:`\alpha`",
        default=np.array([-0.1]),
        domain=Range(lo=-1.0, hi=0.0, step=0.1),
        doc="\alpha parameter")

    beta = NArray(
        label=r":math:`\beta`",
        default=np.array([0.0]),
        domain=Range(lo=0.0, hi=1.0, step=0.1),
        doc="\beta parameter")

    state_variable_range = Final(
        label="State Variable ranges [lo, hi]",
        default={"x": np.array([-1.0, 1.0]),
                 "y": np.array([-1.0, 1.0])},
        doc="""The values for each state-variable should be set to encompass
        the expected dynamic range of that state-variable for the current
        parameters, it is used as a mechanism for bounding random inital
        conditions when the simulation isn't started from an explicit history,
        it is also provides the default range of phase-plane plots.""")

    variables_of_interest = List(
        of=str,
        label="Variables watched by Monitors",
        choices=("x", 'y'),
        default=("x", 'y'),
        doc="""This represents the default state-variables of this Model to be
               monitored. It can be overridden for each Monitor if desired. The
               corresponding state-variable indices for this model are :math:`E = 0`
               and :math:`I = 1`.""")

    state_variables = ('x','y')
    _nvar = 2
    cvar = np.array([0, 1], dtype=np.int32)

    rc = None

    HKB = True

    def update_derived_parameters(self):
        # Compute angular frequency from frequency,
        # if the former has not been determined by the user:
        self.omega = np.where(self.omega == 0.0, 2*np.pi*self.f, self.omega)
        # Compute critical angular frequency from critical frequency,
        # if the former has not been determined by the user:
        self.omega_cr = np.where(self.omega_cr == 0.0, 2*np.pi*self.f_cr, self.omega_cr)
        self.omega2 = np.power(self.omega, 2)
        # Compute critical amplitude:
        self.rc2 = self.gamma / (self.A + 3 * self.B * np.power(self.omega_cr, 2))
        self.rc = np.sqrt(self.rc2)
        # Computer beta coupling parameter from alpha coupling parameter and critical amplitude:
        self.beta = np.where(self.beta == 0, -self.alpha/self.rc2/4, self.beta)

    def dfun(self, state, coupling, local_coupling=0.0):
        """
        .. math::
            \dot{x} &= y
            \dot{y} &= (\gamma- Ax - 3*By)y - omega*x^2 + (y-\sum_jy_j)*(a + b*(x-\sum_jx_j)**2)
        """
        x = state[0]
        y = state[1]

        x2 = np.power(x, 2)
        y2 = np.power(y, 2)

        cx = x - coupling[0]  # x_i - sum_j(Cij*x_j)    x[::-1] #
        cx2 = np.power(cx, 2) # (x_i - sum_j(Cij*x_j))^2

        dx = np.empty_like(state)

        # dx/dt = y
        dx[0] = y

        # dy/dt =
        dx[1] = (self.gamma - self.A*x2 - self.B*y2)*y - self.omega2*x
        # Add coupling:
        if self.HKB:
            # HKB coupling. It requires instantaneous interhemispheric connections.
            cy = y - coupling[1]  # # y_i - sum_j(Cij*y_j)   # y[::-1] #
            dx[1] += cy*(self.alpha + self.beta*cx2)
        else:
            # Nonlinear coupling without velocity information. It might function with long intehemispheric delays.
            dx[1] -= cx*(self.alpha + self.beta*cx2)
            # dx[1] -= cx/(1 + np.exp(-cx))

        return dx


@RegisterEngine()
class Script(EngineScript):

    def _initialize_connectivity(self):
        iF = None
        MIN_TRACT_LENGTH = DT * SPEED
        if "HKBBrain" in MODEL:
            # Human connectivity:
            # connectivity = Connectivity.from_file(config.DEFAULT_CONNECTIVITY_ZIP)
            connectivity = Connectivity.from_file(DEFAULT_CONNECTIVITY_ZIP)
            number_of_regions = connectivity.weights.shape[0]
            Motor_regions = np.array(['precentral_L', 'precentral_R'])
            Sensory_regions = np.array(['postcentral_L','postcentral_R',])
            regions_labels = connectivity.region_labels.tolist()
            iM = np.array([regions_labels.index(mr) for mr in Motor_regions])
            iS = np.array([regions_labels.index(mr) for mr in Sensory_regions])
            # Make sure that we don't get 0 delays for existing connections
            connectivity.tract_lengths = np.maximum(connectivity.tract_lengths, MIN_TRACT_LENGTH)
            # Normalize connectome:
            connectivity.weights = connectivity.scaled_weights(mode="region")
            connectivity.weights /= np.percentile(connectivity.weights, 99)
            connectivity.weights *= G   # Downscale non-task-specific connectome
            # Set task specific connectome:
            # Strong contralateral S1 -> M1 connections
            connectivity.weights[iM, iS[::-1]] = np.array([1.0, 1.0])
            # Symmetric contralateral S1 -> M1 connections' delays
            connectivity.tract_lengths[iM, iS[::-1]] = connectivity.tract_lengths[iM, iS[::-1]].mean()
            if "Fingers" not in MODEL:
                # If there are no fingers, we need to connect directly unilateral M1 -> S1
                # Strong unilateral M1 -> S1 connections
                connectivity.weights[iS, iM] = np.array([1.0, 1.0])
                # Symmetric unilateral M1 -> S1 connections' delays
                connectivity.tract_lengths[iS, iM] = connectivity.tract_lengths[iS, iM].mean()
                # connectivity.tract_lengths[iS, iM] = np.array([MIN_TRACT_LENGTH]*2)
            if HKB:
                # Minimize delays for contralateral S1 -> M1 connections for HKB coupling...
                connectivity.tract_lengths[iM, iS[::-1]] = np.array([MIN_TRACT_LENGTH]*2)  # Contralateral S1 -> M1

            # Plot task-specific connectivity:
            from copy import deepcopy
            connMS = deepcopy(connectivity)
            iMS = np.concatenate([iM, iS])
            connMS.region_labels = connectivity.region_labels[iMS]
            connMS.centres = connectivity.centres[iMS]
            connMS.areas = connectivity.areas[iMS]
            connMS.hemispheres = connectivity.hemispheres[iMS]
            connMS.cortical = connectivity.cortical[iMS]
            connMS.weights = connectivity.weights[iMS][:, iMS]
            connMS.tract_lengths = connectivity.tract_lengths[iMS][:, iMS]
            connMS.speed = np.array([SPEED])
            connMS.configure()
            pyplot.figure()
            plotter.plot_tvb_connectivity(connMS)

        if "Fingers" in MODEL:
            # Augment connectivity with the two fingers
            connectivity.region_labels = np.concatenate([connectivity.region_labels,
                                                        np.array(["Finger_L", "Finger_R"])])
            number_of_regions = connectivity.region_labels.shape[0]
            iF = number_of_regions - np.array([1, 2]).astype('i')
            maxCentres = np.max(connectivity.centres, axis=0)
            connectivity.centres = np.concatenate([connectivity.centres,
                                                -maxCentres[np.newaxis],
                                                maxCentres[np.newaxis]])
            connectivity.areas = np.concatenate([connectivity.areas,
                                                np.array([connectivity.areas.min()]*2)])
            connectivity.hemispheres = np.concatenate([connectivity.hemispheres,
                                                    np.array([True]*2)])
            connectivity.cortical = np.concatenate([connectivity.cortical,
                                                    np.array([False]*2)])
            for attr, val in zip(["weights", "tract_lengths"], [1.0, MIN_TRACT_LENGTH]):
                prop = getattr(connectivity, attr).copy()
                prop = np.concatenate([prop, np.zeros((number_of_regions-2, 2))], axis=1)
                prop = np.concatenate([prop, np.zeros((2, number_of_regions))], axis=0)
                # Connect the unilateral motor & sensory hemispheres to the fingers
                # i.e., the connectivity goes as
                # unilateral M1 -> unilateral Finger -> unilateral S1 -> contralateral M1
                for iH in range(2):
                    # Left = 0, Right = 1
                    prop[iF[iH], iM[iH]] = val  # M1 -> Finger
                    prop[iS[iH], iF[iH]] = val  # Finger -> S1
                setattr(connectivity, attr, prop)

        # Plot the new task specific connectome, augmented by the fingers:
        from copy import deepcopy
        connMS = deepcopy(connectivity)
        iMS = np.concatenate([iM, iS, iF])
        connMS.region_labels = connectivity.region_labels[iMS]
        connMS.centres = connectivity.centres[iMS]
        connMS.areas = connectivity.areas[iMS]
        connMS.hemispheres = connectivity.hemispheres[iMS]
        connMS.cortical = connectivity.cortical[iMS]
        connMS.weights = connectivity.weights[iMS][:, iMS]
        connMS.tract_lengths = connectivity.tract_lengths[iMS][:, iMS]
        connMS.speed = np.array([SPEED])
        connMS.configure()
        pyplot.figure()
        plotter.plot_tvb_connectivity(connMS)

        connectivity.speed = np.array([SPEED])
        connectivity.configure()

        pyplot.figure()
        plotter.plot_tvb_connectivity(connectivity)

        self.connectivity = connectivity
        self.iM = iM
        self.iS = iS
        self.iMS = np.concatenate([iM, iS])
        self.iF = iF


    def _initialize_simulator(self):
        # Create simulator, connectivity, linear coupling, integrator with noise, monitors
        simulator = Simulator()

        simulator.connectivity = self.connectivity

        simulator.coupling = Linear(a=np.array([1.0]))

        simulator.model = HKBMBrain(f_cr=np.array([FREQ_CR]))
        simulator.model.HKB = HKB

        simulator.integrator = EulerStochastic()
        simulator.integrator.dt = DT
        simulator.integrator.noise.nsig = np.array([config.DEFAULT_NSIG] * simulator.model.nvar)

        mon_raw = Raw(period=1.0)  # ms
        simulator.monitors = (mon_raw, )

        # Set initial conditions appropriate for inphase and antiphase coordination mode:
        simulator.connectivity.set_idelays(simulator.integrator.dt)
        simulator.horizon = simulator.connectivity.idelays.max() + 1  # time horizon in the past due to delays
        simulator.initial_conditions = 0.1*np.ones((simulator.horizon,
                                                    simulator.model.nvar,
                                                    self.connectivity.number_of_regions,
                                                    1)) # last dimension is modes, you could simulate both inphase and antiphase!
        if INIT_COND == "antiphase":
            # The right M1 and finger should be antiphase:
            simulator.initial_conditions[:, :, self.iM[1]] = -0.1
            if "Fingers" in MODEL:
                simulator.initial_conditions[:, :, self.iF[1]] = -0.1


        if "Cosim" in MODEL:
            from tvb.contrib.cosimulation.cosim_monitors import CosimCoupling
            simulator.voi = np.array([0, 1])  # State variables to be interchanged with the fingers' cosimulator
            simulator.proxy_inds = self.iF         # The nodes simulated by the fingers' cosimulator
            simulator.synchronization_time = DT
            # Coupling from all TVB nodes, towards the nodes of the fingers' cosimulator
            # is what will be transferred from TVB to the fingers:
            simulator.cosim_monitors = (CosimCoupling(), )
            simulator.cosim_monitors[0].coupling.a = np.array([1.0])  # Linear coupling of course!
            simulator.exclusive = True  # Fingers are exclusively simulated by the fingers' cosimulator

        # Configure the simulator...
        simulator.simulation_length = SIMULATION_LENGTH # ms
        simulator.configure()

        print(simulator)
        print(simulator.model)
        print("\nHKB network weights:")
        print(self.connectivity.weights[self.iMS][:, self.iMS])
        print("\nHKB network delay steps:")
        print(self.connectivity.idelays[self.iMS][:, self.iMS])

        print("critical angular frequency = %g" % simulator.model.omega_cr[0])
        print("critical frequency = %g" % simulator.model.f_cr[0])
        print("critical amplitudes = %s" % str(simulator.model.rc))
        print("alpha coupling coefficient = %s" % str(simulator.model.alpha))
        print("beta coupling coefficient = %s" % str(simulator.model.beta))

        # Initializing the fingers position and velocity
        # (this is the format of data that gets communicated to/from TVB,
        # i.e., [np.array(times), np.array(data_of_shape[n_times,
        #                                                n_vars=position and velocity,
        #                                                n_nodes=2 fingers,
        #                                                n_modes=1])]):
        simulator.last_fingers_data = [np.array([simulator.current_step * simulator.integrator.dt]), # First and last times of the synchronization time
                                    simulator.initial_conditions[-1:, :, self.iF, :]]  # data

        self.simulator = simulator


    def _initialize_tvb(self):
        self._initialize_connectivity()
        self._initialize_simulator()


    def _initialize_datapacks(self):
        self._registerDataPack("left_index_finger_joints")
        self._registerDataPack("right_index_finger_joints")

        self._registerDataPack("left_index_finger_target")
        self._registerDataPack("right_index_finger_target")

        self._setDataPack("left_index_finger_target",  { "positions" : [0.0, 0.0, 0.0, 0.0] })
        self._setDataPack("right_index_finger_target", { "positions" : [0.0, 0.0, 0.0, 0.0] })


    def initialize(self):
        self._initialize_tvb()
        self._initialize_datapacks()
        self.time_ms = 0
        self.iteration = 0
        self.prev_left_position = None
        self.prev_right_position = None
        self.left_pos_buffer = []
        self.left_vel_buffer = []
        self.right_pos_buffer = []
        self.right_vel_buffer = []
        self.init = True
        # Total scaling factor = amplitude_scaling * tvb_EngineTimestep / gazebo_EngineTimestep * 1 / integrator.dt
        self.amplitude_scaling = 10


    def simulate_fun(self, simulator, left_position, right_position, left_velocity, right_velocity):
        from copy import deepcopy

        motor_commands_data = deepcopy(simulator.loop_cosim_monitor_output()[0])
        motor_commands_data[1] = motor_commands_data[1][:, :, self.iF, :]

        input = deepcopy(simulator.last_fingers_data)

        input[1][0, 0, 0, 0] = left_position
        input[1][0, 0, 1, 0] = right_position

        input[1][0, 1, 0, 0] = left_velocity
        input[1][0, 1, 1, 0] = right_velocity

        input[0][0] = input[0][0] - simulator.integrator.dt

        # 2. Simulate the TVB brain by giving as input the current finger's position and velocity data
        dtres = list(simulator(cosim_updates=input))[0]

        sim_res = list(dtres)
        # For a single time point, correct the 1st dimension:
        sim_res[0][0] = np.array([sim_res[0][0]])
        sim_res[0][1] = sim_res[0][1][np.newaxis]

        simulator.last_fingers_data = deepcopy(motor_commands_data)

        return motor_commands_data, sim_res


    def select_frequency(self):
        prev_time_ms = self.time_ms - self.simulator.integrator.dt
        if self.time_ms == 0:
            self.simulator.model.f = np.array([FREQ[0]])
            self.simulator.model.omega = np.array([0.0])
            self.simulator.model.update_derived_parameters()
        elif prev_time_ms < SIMULATION_LENGTH and self.time_ms >= SIMULATION_LENGTH:
            self.simulator.model.f = np.array([FREQ[1]])
            self.simulator.model.omega = np.array([0.0])
            self.simulator.model.update_derived_parameters()
        elif self.time_ms >= 2 * SIMULATION_LENGTH:
            self.simulator.model.f = np.array([FREQ[2]])
            self.simulator.model.omega = np.array([0.0])
            self.simulator.model.update_derived_parameters()


    def calculate_left_velocity(self, left_position):
        if self.prev_left_position is None:
            left_velocity = self.simulator.initial_conditions[-1, 1, self.iF[0], 0]
            self.prev_left_position = left_position
        else:
            left_velocity  = (left_position  - self.prev_left_position)  / self.simulator.integrator.dt

        # Cache the position

        self.prev_left_position = left_position

        return left_velocity


    def calculate_right_velocity(self, right_position):
        if self.prev_right_position is None:
            right_velocity = self.simulator.initial_conditions[-1, 1, self.iF[1], 0]
            self.prev_right_position = right_position
        else:
            right_velocity = (right_position - self.prev_right_position) / self.simulator.integrator.dt

        # Cache the position

        self.prev_right_position = right_position

        return right_velocity


    def save_results(self, tempres):
        if self.time_ms == 0:
            self.results = list(tempres)
        else:
            self.results[0][0] = np.concatenate([self.results[0][0], tempres[0][0]])
            self.results[0][1] = np.concatenate([self.results[0][1], tempres[0][1]])


    def runLoop(self, timestep_ns):
        # Positions should be in TVB units

        # Get positions of left and right index finger's joints from previous iteration

        left_position  = self._getDataPack("left_index_finger_joints") ["positions"][2]
        right_position = self._getDataPack("right_index_finger_joints")["positions"][2]

        left_position  *= self.amplitude_scaling
        right_position *= self.amplitude_scaling

        # TODO Try to put the initial conditions in the world/model files

        if self.init:
            init_position_left  = self.simulator.initial_conditions[-1, 0, self.iF[0], 0]
            init_position_right = self.simulator.initial_conditions[-1, 0, self.iF[1], 0]

            if abs(left_position - init_position_left) > 0.01 or abs(right_position - init_position_right) > 0.01:
                self._setDataPack("left_index_finger_target",  { "positions" : [0.0, 0.0, init_position_left  / self.amplitude_scaling, 0.0] })
                self._setDataPack("right_index_finger_target", { "positions" : [0.0, 0.0, init_position_right / self.amplitude_scaling, 0.0] })

                self.iteration += 1
                print("Init: ", self.iteration)
                return
            else:
                self.init = False

        # Choose the frequency based on simulation time

        self.select_frequency()

        # Calculate velocities

        left_velocity  = self.calculate_left_velocity(left_position)
        right_velocity = self.calculate_right_velocity(right_position)

        # Save positions and velocities for plotting

        self.left_pos_buffer.append(left_position)
        self.left_vel_buffer.append(left_velocity)

        self.right_pos_buffer.append(right_position)
        self.right_vel_buffer.append(right_velocity)

        # Run the brain simulation

        commands, tempres = self.simulate_fun(self.simulator, left_position, right_position, left_velocity, right_velocity)

        # Save the results for plotting

        self.save_results(tempres)

        # Set new targets for the fingers

        left_target  = commands[1][-1, 0, 0, 0] / self.amplitude_scaling
        right_target = commands[1][-1, 0, 1, 0] / self.amplitude_scaling

        self._setDataPack("left_index_finger_target",  { "positions" : [0.0, 0.0, left_target,  0.0] })
        self._setDataPack("right_index_finger_target", { "positions" : [0.0, 0.0, right_target, 0.0] })

        # Increment iteration and time variables

        self.time_ms += self.simulator.integrator.dt
        self.iteration += 1
        print(self.iteration)


    def plot(self):
        # Gather the results
        t = self.results[0][0]
        source = self.results[0][1]

        # Put the results in a Timeseries instance
        from tvb.contrib.scripts.datatypes.time_series_xarray import TimeSeriesRegion

        source_ts = TimeSeriesRegion(source, time=t,
                    connectivity=self.simulator.connectivity,
                    labels_ordering=["Time", "State Variable", "Region", "Neurons"],
                    labels_dimensions=
                            {"State Variable": self.simulator.model.state_variables,
                            "Region": self.simulator.connectivity.region_labels.tolist()},
                    sample_period=self.simulator.integrator.dt)
        source_ts.configure()

        FIGSIZE = (20, 10)

        if "Fingers" in MODEL:
            iX = self.iF
        else:
            iX = self.iM
        DX = np.abs(source_ts[:, 0, iX[0]].data.squeeze() - source_ts[:, 0, iX[1]].data.squeeze())
        SX = ( np.abs(source_ts[:, 0, iX[0]].data.squeeze()) + np.abs(source_ts[:, 0, iX[1]].data.squeeze()) )/ 2
        DXc = np.convolve(DX/SX, np.ones((100,))/100, mode='same')

        X1 = np.convolve(np.abs(source_ts[:, 0, iX[0]].data.squeeze()),
                        np.ones((100,))/100, mode='same')
        X2 = np.convolve(np.abs(source_ts[:, 0, iX[1]].data.squeeze()),
                        np.ones((100,))/100, mode='same')
        tp = int(SIMULATION_LENGTH/DT/len(FREQ))

        fig, axes = pyplot.subplots(3, 1, figsize=FIGSIZE)
        axes[0].plot(t, DXc, 'k', label='|x1-x2|/(|x1|+|x2|)/2)')
        axes[0].plot(t, X1, 'b', label='|x1|')
        axes[0].plot(t, X2, 'g', label='|x2|')
        minDX = 0.0
        maxDX = np.max([DXc.max(), X1.max(), X2.max()])
        axes[0].plot([0.0, t[tp]], [minDX, minDX], 'r', label=None)
        axes[0].plot([0.0, t[tp]], [maxDX, maxDX], 'r', label=None)
        axes[0].plot([0.0, 0.0], [minDX, maxDX], 'r', label=None)
        axes[0].plot([t[tp], t[tp]], [minDX, maxDX], 'r', label=None)
        axes[0].plot([t[-tp], t[-1]], [minDX, minDX], 'r', label=None)
        axes[0].plot([t[-tp], t[-1]], [maxDX, maxDX], 'r', label=None)
        axes[0].plot([t[-1], t[-1]], [minDX, maxDX], 'r', label=None)
        axes[0].plot([t[-tp], t[-tp]], [minDX, maxDX], 'r', label=None)
        axes[0].set_title('Convolutions')
        axes[0].legend()
        axes[1].plot(t[:tp:], source_ts[:tp, 0, iX[0]].data.squeeze()[:tp], 'b')
        axes[1].plot(t[:tp:], source_ts[:tp:, 0, iX[1]].data.squeeze()[:tp], 'g')

        axes[1].plot(t[:tp:], source_ts[:tp, 0, self.iM[0]].data.squeeze()[:tp], 'b.')
        axes[1].plot(t[:tp:], source_ts[:tp:, 0, self.iM[1]].data.squeeze()[:tp], 'g.')

        axes[1].set_title('mean(|x1-x2|/(|x1|+|x2|)/2)) = %g' % np.mean(DX[:tp]/SX[:tp]))
        axes[2].plot(t[-tp:], source_ts[-tp:, 0, iX[0]].data.squeeze()[-tp:], 'b')
        axes[2].plot(t[-tp:], source_ts[-tp:, 0, iX[1]].data.squeeze()[-tp:], 'g')

        axes[2].plot(t[-tp:], source_ts[-tp:, 0, self.iM[0]].data.squeeze()[-tp:], 'b.')
        axes[2].plot(t[-tp:], source_ts[-tp:, 0, self.iM[1]].data.squeeze()[-tp:], 'g.')

        axes[2].set_title('mean(|x1-x2|/(|x1|+|x2|)/2)) = %g' % np.mean(DX[-tp:]/SX[-tp:]))
        fig.tight_layout()
        pyplot.savefig('outputs/out.png')


    def shutdown(self):
        pyplot.clf()
        pyplot.plot(self.results[0][1][:, 0, self.iF[0], 0])
        pyplot.plot(self.left_pos_buffer)
        pyplot.savefig('outputs/pos_left_001.png')

        pyplot.clf()
        pyplot.plot(self.results[0][1][:, 1, self.iF[0], 0])
        pyplot.plot(self.left_vel_buffer)
        pyplot.savefig('outputs/vel_left_001.png')

        pyplot.clf()
        pyplot.plot(self.results[0][1][:, 0, self.iF[1], 0])
        pyplot.plot(self.right_pos_buffer)
        pyplot.savefig('outputs/pos_right_001.png')

        pyplot.clf()
        pyplot.plot(self.results[0][1][:, 1, self.iF[1], 0])
        pyplot.plot(self.right_vel_buffer)
        pyplot.savefig('outputs/vel_right_001.png')

        self.plot()

# EOF
