from nrp_core import *
from nrp_core.data.nrp_protobuf import *
from nrp_core.data.nrp_json import *
from nrp_core.engines.nest_server import *
import numpy as np
from PIL import Image
import time
import cv2


def detect_red(camDataPack):
    """
    Performs a very simple image detection as used in the Braitenberg demo.
    Copied and modified from original NRP, see hbp_nrp_cle/hbp_nrp_cle/tf_framework/tf_lib.py

    :param image: The image
    :returns: An object with three properties:
        - *left*: This is the percentage of red pixels in the left half of the image
        - *right*: This is the percentage of red pixels in the right half of the image
        - *go_on*: This is the percentage of non-red pixels of the overall image

    :example: A completely red image (255,0,0) results in (1,1,0)
    :example: A completely yellow image (255,255,0) results in (0,0,1)

    The lightest color that is recognized as red is (255,127,127).
    """
    red_left = red_right = green_blue = 0
    if not camDataPack.isEmpty():

        lower_red = np.array([0, 30, 30])
        upper_red = np.array([0, 255, 255])

        # Reshape to proper size
        d = np.frombuffer(camDataPack.data.imageData, np.uint8)
        cv_image = d.reshape((camDataPack.data.imageHeight,camDataPack.data.imageWidth,3))

        # Transform image to HSV (easier to detect colors).
        hsv_image = cv2.cvtColor(cv_image, cv2.COLOR_RGB2HSV)

        # Create a mask where every non red pixel will be a Zero.
        mask = cv2.inRange(hsv_image, lower_red, upper_red)
        image_size = (cv_image.shape[0] * cv_image.shape[1])
        if (image_size > 0):
            half = cv_image.shape[1] // 2
            # Get the number of red pixels in the image.
            red_left = cv2.countNonZero(mask[:, :half])
            red_right = cv2.countNonZero(mask[:, half:])
            green_blue = (image_size - (red_left + red_right)) / image_size
            # We have to mutiply the rate by two since it is for an half image only.
            red_left = 2 * (red_left / image_size)
            red_right = 2 * (red_right / image_size)

    class __results(object):
        """
        An intermediate helper class for the results of detect_red
        """

        def __init__(self, left, right, go_on):
            self.left = left
            self.right = right
            self.go_on = go_on

    return __results(red_left, red_right, green_blue)


@FromEngineDataPack(keyword='camera', id=DataPackIdentifier('husky_camera::camera', 'gazebo'))
@TransceiverFunction("nest")
def transceiver_function(camera):
    #print("Camera Depth: " + str(camera.image_depth))
    
    # Set to True to display camera image data and pause for 10 s
    if False and not camera.isEmpty():
        d = np.frombuffer(camera.data.imageData, np.uint8)
        cv_image = d.reshape((camera.data.imageHeight,camera.data.imageWidth,3))
        img = Image.fromarray(cv_image)
        img.show()
        time.sleep(2)

    res = detect_red(camera)

    # print("Left Red:  " + str(res.left))
    # print("Right Red: " + str(res.right))
    # print("Go On:     " + str(res.go_on))

    lpg = JsonDataPack("lpg", "nest")
    rpg = JsonDataPack("rpg", "nest")
    gpg = JsonDataPack("gpg", "nest")

    lpg.data['rate'] = 2000.0*res.left
    rpg.data['rate'] = 2000.0*res.right
    gpg.data['rate'] = 75.0*res.go_on

    return [ lpg, rpg, gpg ]

