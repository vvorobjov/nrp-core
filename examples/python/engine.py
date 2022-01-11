import cv2

def preprocess_image(iteration): #-> np.ndarray:

    # Load image of the current step
    frame = cv2.imread('./images/received-image-' + str(iteration) + '.jpg')
    frame = cv2.cvtColor(frame, cv2.COLOR_RGB2GRAY)
    # If resizing is need
    frame = cv2.resize(frame, (150, 150), interpolation=cv2.INTER_AREA)
    # Preprocessing methods:

    # method 1: Edge detection
    frame = cv2.Canny(frame, 35, 100)
    # method 2: Threshold
    #frame = cv2.threshold(frame, 0, 255,cv2.THRESH_BINARY | cv2.THRESH_OTSU)[1]

    return frame[..., None]


from stable_baselines3 import A2C, PPO
from engine_script import EngineScript
import random

class Script(EngineScript):
    def initialize(self):
        print("Python engine is initializing. Registering datapack...")
        self._registerDataPack("JointAngles")
        self._setDataPack("JointAngles", { "joint_angles" : [1, 1, 1]})
        agent_name_g = "PPO_agent_canny_image_cnn_policy_150"
        log_dir_g = "./scripts/python_nrp/sb3/agent_models/" + agent_name_g
        self.model_g = PPO.load(log_dir_g + str(0))#, env)

    def runLoop(self):
        img = preprocess_image(1)
        self.model_g.predict(img, deterministic=True)
        self._setDataPack("JointAngles", { "joint_angles" : [(random.random() - 0.5) * 100, 1, 1]})

    def shutdown(self):
        print("Python engine is shutting down")

# EOF