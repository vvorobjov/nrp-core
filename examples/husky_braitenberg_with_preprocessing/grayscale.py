from NRPPythonModule import *
import numpy as np

@FromEngineDevice(keyword='camera', id=DeviceIdentifier('husky_camera::camera', 'gazebo'))
@PreprocessingFunction("gazebo")
def transceiver_function(camera):

    # Return an empty device, if there's no data in camera device
    if camera.isEmpty():
        return [ DeviceInterface("processed", "gazebo") ]

    # Convert image to grayscale
    rgb_weights = [0.2989, 0.5870, 0.1140]
    d = np.frombuffer(camera.data.imageData, np.uint8)
    image_data = d.reshape((camera.data.imageHeight,camera.data.imageWidth,3))

    my_dict = {
        "image_height": camera.data.imageHeight,
        "image_width" : camera.data.imageWidth,
        "grayscale"   : np.dot(image_data[...,:3], rgb_weights),
    }

    # Save image in grayscale in a device and return it
    device = PythonDevice("processed", "gazebo")
    device.data = my_dict

    return [ device ]
