from NRPPythonModule import *

@FromEngineDevice(keyword='camera', id=DeviceIdentifier('husky_camera::camera', 'gazebo'))
@PreprocessingFunction("gazebo")
def transceiver_function(camera):

    # Return an empty device, if there's no data in camera device
    if camera.isEmpty():
        return [ DeviceInterface("processed", "gazebo") ]

    # Convert image to grayscale
    rgb_weights = [0.2989, 0.5870, 0.1140]
    image_data = camera.image_data.reshape((camera.image_height, camera.image_width, 3))

    my_dict = {
        "image_height": camera.image_height,
        "image_width" : camera.image_width,
        "grayscale"   : np.dot(image_data[...,:3], rgb_weights),
    }

    # Save image in grayscale in a device and return it
    device = PythonDevice("processed", "gazebo")
    device.data = my_dict

    return [ device ]
