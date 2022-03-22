from nrp_core import *
from nrp_core.data.nrp_json import *
import numpy as np
from PIL import Image
import time
import cv2


@EngineDataPack(keyword='camera', id=DataPackIdentifier('husky_camera::camera', 'gazebo'))
@PreprocessingFunction("gazebo")
def detect_red(camera):
    """
    Performs a very simple image detection as used in the Braitenberg demo.
    Copied and modified from original NRP, see hbp_nrp_cle/hbp_nrp_cle/tf_framework/tf_lib.py

    :param camera: The DataPack containing the image to process
    :returns: A JsonDataPack with three properties:
        - *left*: This is the percentage of red pixels in the left half of the image
        - *right*: This is the percentage of red pixels in the right half of the image
        - *go_on*: This is the percentage of non-red pixels of the overall image

    :example: A completely red image (255,0,0) results in (1,1,0)
    :example: A completely yellow image (255,255,0) results in (0,0,1)

    The lightest color that is recognized as red is (255,127,127).
    """

    red_left = red_right = green_blue = 0
    if not camera.isEmpty():

        # Set to True to display camera image data and pause for 10 s
        show_image = False
        if show_image:
            d = np.asarray(camera.data["image_data"], np.uint8)
            cv_image = d.reshape((camera.data["image_height"], camera.data["image_width"], 3))
            img = Image.fromarray(cv_image)
            img.show()
            time.sleep(10)

        lower_red = np.array([0, 30, 30])
        upper_red = np.array([0, 255, 255])

        # Reshape to proper size
        d = np.asarray(camera.data["image_data"], np.uint8)
        cv_image = d.reshape((camera.data["image_height"], camera.data["image_width"], 3))

        # Transform image to HSV (easier to detect colors).
        hsv_image = cv2.cvtColor(cv_image, cv2.COLOR_RGB2HSV)

        # Create a mask where every non red pixel will be a Zero.
        mask = cv2.inRange(hsv_image, lower_red, upper_red)
        image_size = (cv_image.shape[0] * cv_image.shape[1])
        if image_size > 0:
            half = cv_image.shape[1] // 2
            # Get the number of red pixels in the image.
            red_left = cv2.countNonZero(mask[:, :half])
            red_right = cv2.countNonZero(mask[:, half:])
            green_blue = (image_size - (red_left + red_right)) / image_size
            # We have to multiply the rate by two since it is for an half image only.
            red_left = 2 * (red_left / image_size)
            red_right = 2 * (red_right / image_size)

    processed_image = JsonDataPack("processed_image", "gazebo")
    processed_image.data["left"] = red_left
    processed_image.data["right"] = red_right
    processed_image.data["go_on"] = green_blue

    # print("------------------")
    # print("Left Red:  {}".format(processed_image.data["left"]))
    # print("Right Red: {}".format(processed_image.data["right"]))
    # print("Go On:     {}".format(processed_image.data["go_on"]))

    return [processed_image]
