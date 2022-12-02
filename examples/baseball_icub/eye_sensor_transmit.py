from nrp_core import *
from nrp_core.data.nrp_json import *
import numpy as np

last_mean_green = None

@EngineDataPack(keyword='camera', id=DataPackIdentifier('iCub::left_eye::camera_left', 'gazebo'))
@TransceiverFunction("nest")
def eye_sensor_transmit(camera):

    if camera.isEmpty():
        return []

    d = np.frombuffer(camera.data.imageData, np.uint8)
    cv_image = d.reshape((camera.data.imageHeight, camera.data.imageWidth, 3))
    green_channel = cv_image[:,:,1]
    mean_green = np.mean(green_channel)
    global last_mean_green
    if last_mean_green is None:
        last_mean_green = mean_green
    delta_mean_green = mean_green - last_mean_green

    s = JsonDataPack("sensors", "nest")
    for i in range(20):
        s.data.append({'amplitude': 3000. * max(0., delta_mean_green) * np.random.rand()})

    last_mean_green = mean_green

    return [s]
