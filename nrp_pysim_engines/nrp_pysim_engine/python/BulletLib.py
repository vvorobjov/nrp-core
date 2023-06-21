# Link of py_bullet:
# -- https://pybullet.org/wordpress/
# Documentation of PyBullet:
# -- https://docs.google.com/document/d/10sXEhzFRSnvFcl3XxNGhnD4N2SedqwdAvK3dsihxVUA/edit#heading=h.jxof6bt5vhut

import os
import pybullet
import pybullet_data


class BulletInterface(object):
    def __init__(self, model_name, start_visualizer, time_step, extra_params):
        super().__init__()

        self.physics_client = pybullet.connect(pybullet.GUI if start_visualizer else pybullet.DIRECT)

        # default time step in pybullet is 1/240 s
        pybullet.setTimeStep(time_step, self.physics_client)
        pybullet.setGravity(0, 0, -9.8)

        pybullet.setAdditionalSearchPath(pybullet_data.getDataPath())
        pybullet.loadURDF(os.path.join(pybullet_data.getDataPath(), "plane.urdf"))  # TODO hardcoded?

        self.model = pybullet.loadURDF(model_name)

        # bullet assumes joint_num == link_num
        self.joint_num = pybullet.getNumJoints(self.model,
                                               physicsClientId=self.physics_client)
        self.body_num = pybullet.getNumBodies(physicsClientId=self.physics_client)
        self.joint_name_to_id = {}
        self.link_name_to_id = {}
        self.body_name_to_id = {}
        for i in range(self.joint_num):
            joint_info = pybullet.getJointInfo(self.model, i)

            self.joint_name_to_id[joint_info[1].decode('UTF-8')] = i  # JOINT_NAME = 1
            self.link_name_to_id[joint_info[12].decode('UTF-8')] = i  # LINK_NAME = 12

        for i in range(self.body_num):
            body_info = pybullet.getBodyInfo(i)

            self.body_name_to_id[body_info[1].decode('UTF-8')] = i  # JOINT_NAME = 1

        self.sim_time = 0  # seconds

    def run_one_step(self, action, timestep_ns):
        timestep_s = timestep_ns / 1e9
        pybullet.setTimeStep(timestep_s, self.physics_client)

        if "Joint" in action:
            for joint in action['Joint']:
                joint_dict = {'index': 0,
                              'controlMode': 'POSITION_CONTROL',
                              'targetPosition': 0, 'targetVelocity': 0,
                              'force': 0,
                              'positionGain': 1, 'velocityGain': 1,
                              'maxVelocity': 0}
                joint_dict.update(joint)

                pybullet.setJointMotorControl2(self.model,
                                               joint_dict['index'],
                                               controlMode=getattr(pybullet, joint_dict['controlMode']),
                                               targetPosition=joint_dict['targetPosition'],
                                               targetVelocity=joint_dict['targetVelocity'],
                                               force=joint_dict['force'],
                                               positionGain=joint_dict['positionGain'],
                                               velocityGain=joint_dict['velocityGain'],
                                               maxVelocity=joint_dict['maxVelocity'],
                                               physicsClientId=self.physics_client)
        if "Force" in action:
            for force in action['Force']:
                force_dict = {'index': 0,
                              'forceObj': [0, 0, 0],
                              'posObj': [0, 0, 0],
                              'flags': 'LINK_FRAME'}
                force_dict.update(force)

                pybullet.applyExternalForce(self.model,
                                            force_dict["index"],
                                            forceObj=force_dict['forceObj'],
                                            posObj=force_dict['posObj'],
                                            flags=force_dict['flags'],
                                            physicsClientId=self.physics_client)
        if "Torque" in action:
            for index, torque in enumerate(action['Torque']):
                torque_dict = {'index': 0,
                               'torqueObj': [0, 0, 0],
                               'flags': 'LINK_FRAME'}
                torque_dict.update(torque)

                pybullet.applyExternalTorque(self.model,
                                             torque_dict["index"],
                                             torqueObj=torque_dict['torqueObj'],
                                             flags=torque_dict['flags'],
                                             physicsClientId=self.physics_client)
        pybullet.stepSimulation()
        self.sim_time += timestep_s

    def reset(self):
        pybullet.resetSimulation()
        return 0

    def shutdown(self):
        pybullet.disconnect(physicsClientId=self.physics_client)

    def get_model_properties(self, p_type):
        if p_type == 'Joint':
            return self.joint_name_to_id
        elif p_type == 'Link':
            return self.link_name_to_id
        elif p_type == 'Body':
            return self.body_name_to_id 
        else:
            raise Exception(f"Wrong Type {p_type}. Supported property types are 'Joint', 'Link' and 'Body'.")

    def get_model_property(self, p_name, p_type):
        if p_type == 'Joint':
            return pybullet.getJointState(self.model,
                                          self.joint_name_to_id[p_name],
                                          physicsClientId=self.physics_client)
        elif p_type == 'Link':
            return pybullet.getLinkState(self.model,
                                         self.link_name_to_id[p_name],
                                         physicsClientId=self.physics_client)
        elif p_type == 'Body':
            return pybullet.getDynamicsInfo(self.model,
                                            p_name,
                                            physicsClientId=self.physics_client)
        else:
            raise Exception(f"Wrong Type {p_type}. Supported property types are 'Joint', 'Link' and 'Body'.")

    def get_model_all_properties(self, p_type):
        if p_type == 'Joint':
            return pybullet.getJointStates(self.model,
                                           list(range(self.joint_num)),
                                           physicsClientId=self.physics_client)
        elif p_type == 'Link':
            return pybullet.getLinkStates(self.model,
                                          list(self.link_name_to_id.values()),
                                          physicsClientId=self.physics_client)
        elif p_type == 'Body':
            return [pybullet.getDynamicsInfo(body_id, -1, physicsClientId=self.physics_client)  # -1 means base
                    for body_id in range(pybullet.getNumBodies())]
        else:
            raise Exception(f"Wrong Type {p_type}. Supported property types are 'Joint', 'Link' and 'Body'.")

    def get_sim_time(self):
        return self.sim_time
