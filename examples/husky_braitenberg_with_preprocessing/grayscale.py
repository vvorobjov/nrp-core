from nrp_core import *
import numpy as np

@EngineDataPack(keyword='camera', id=DataPackIdentifier('husky_camera::camera', 'gazebo'))
@PreprocessingFunction("gazebo")
def transceiver_function(camera):

    # Return an empty datapack, if there's no data in camera datapack
    if camera.isEmpty():
        return [ DataPackInterface("processed", "gazebo") ]

    # Convert image to grayscale
    rgb_weights = [0.2989, 0.5870, 0.1140]
    d = np.frombuffer(camera.data.imageData, np.uint8)
    image_data = d.reshape((camera.data.imageHeight,camera.data.imageWidth,3))

    # Save image in grayscale in a datapack and return it
    datapack = JsonDataPack("processed", "gazebo")
    datapack.data["image_height"] = camera.data.imageHeight
    datapack.data["image_width" ] = camera.data.imageWidth
    datapack.data["grayscale"   ] = np.dot(image_data[...,:3], rgb_weights).tolist()

    return [ datapack ]
