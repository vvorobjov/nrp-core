from flask import Flask, jsonify
app = Flask(__name__)

engine_time = 0
timestep = 200000000


import numpy as np
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

class Script():
    def initialize(self):
        print("Python engine is initializing. Registering datapack...")
        agent_name_g = "PPO_agent_canny_image_cnn_policy_150"
        log_dir_g = "./scripts/python_nrp/sb3/agent_models/" + agent_name_g
        self.model_g = PPO.load(log_dir_g + str(0))#, env)
 
    def runLoop(self):
        img = preprocess_image(1)
        self.model_g.predict(img, deterministic=True)
 
    def shutdown(self):
        print("Python engine is shutting down")

script = Script()


@app.route('/initialize', methods=["POST"])
def initialize():
    global script
    script.initialize()
    return jsonify({"InitExecStatus": True})

@app.route('/run_loop', methods=["POST"])
def run_loop():
    global script
    script.runLoop()
    global engine_time
    engine_time = engine_time + timestep
    return jsonify({"time": engine_time})

@app.route('/set_datapack', methods=["POST"])
def set_datapack():
    return jsonify({})

@app.route('/get_datapack_information', methods=["POST"])
def get_datapack():
    return jsonify({})

@app.route('/reset', methods=["POST"])
def reset():
    return 'Hello World!'

@app.route('/shutdown', methods=["POST"])
def shutdown():
    return {}

# EOF
