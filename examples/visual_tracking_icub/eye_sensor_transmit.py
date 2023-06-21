from nrp_core import *
from nrp_core.data.nrp_json import *
import numpy as np
import cv2
import math

def find_centroid_hsv(camera, lower, upper):
    """
    Finds the centroid of the pixels in an image lying in a given HSV slice.
    Copied and modified from original NRP, see hbp_nrp_cle/hbp_nrp_cle/tf_framework/tf_lib.py

    :param camera: A Gazebo Camera datapack
    :param lower: The lower value of the HSV slice we want to detect (a 3 elements int list with
        values in range 0-255, refer to cv2 documentation for more details).
    :param upper: The upper value of the HSV slice we want to detect (a 3 elements int list with
        values in range 0-255, refer to cv2 documentation for more details).
    :returns: a pair (x, y) denoting the image pixel where the centroid lies, or None if anything
        goes wrong.
    """

    if camera.isEmpty():
        return None

    try:
        d = np.frombuffer(camera.data.imageData, np.uint8)
        cv_image = d.reshape((camera.data.imageHeight, camera.data.imageWidth, 3))
        hsv_im = cv2.cvtColor(cv_image, cv2.COLOR_RGB2HSV)
        lower_np = np.array(lower, dtype="uint8")
        upper_np = np.array(upper, dtype="uint8")
        mask = cv2.inRange(hsv_im, lower_np, upper_np)
        img_out = cv2.bitwise_and(cv_image, cv_image, mask=mask)

        a = cv2.findNonZero(cv2.cvtColor(img_out, cv2.COLOR_RGB2GRAY))

        # At least one point in the HSV slice is detected in the image
        if a is not None and a.size != 0:
            b = np.array(
                [[float(x[0][0]) / (a.size / 2), float(x[0][1]) / (a.size / 2)] for x in a]
            ).transpose()
            y_c, x_c = int(cv2.sumElems(b[1])[0]), int(cv2.sumElems(b[0])[0])
            return x_c, y_c

        return None

    except Exception as e:
        return None


def pixel2angle(u, v):
    """
    Converts a pixel coordinate on the image to an (azimuth, elevation) pair.
    Copied and modified from original NRP, see hbp_nrp_cle/hbp_nrp_cle/tf_framework/tf_lib.py

    :param u: the x coordinate (column) of the pixel (px)
    :param v: the y coordinate (row) of the pixel (px)
    :return: a pair (a, e) denoting the azimuth and the elevation, the center being (0, 0)
        (deg)
    """

    # assumes an image size of 320x240 pixels and square pixels
    # see hbp_nrp_cle/hbp_nrp_cle/tf_framework/tf_lib.py
    xm = float(u - 160.0) / 160.0
    ym = float(v - 120.0) / 160.0

    _rho = math.sqrt(xm ** 2 + ym ** 2 + 1)
    a = -math.atan(xm) * 180 / np.pi  # azimuth
    e = -math.asin(ym / _rho) * 180 / math.pi  # elevation

    return a, e


@EngineDataPack(keyword='camera', id=DataPackIdentifier('iCub::left_eye::camera_left', 'gazebo'))
@TransceiverFunction("nest")
def eye_sensor_transmit(camera):

    xy_ball_pos = find_centroid_hsv(camera, [50, 100, 100], [70, 255, 255]) \
                  or (160, 120)
    ae_ball_pos = pixel2angle(xy_ball_pos[0], xy_ball_pos[1])
    red = 76800.0 / (1.0 + math.exp(-ae_ball_pos[0]))

    lpg = JsonDataPack("lpg", "nest")
    rpg = JsonDataPack("rpg", "nest")
    gpg = JsonDataPack("gpg", "nest")

    lpg.data['rate'] = 1000.0 * red / 76800.0
    rpg.data['rate'] = 1000.0 * red / 76800.0
    gpg.data['rate'] = 1000.0 * (76800.0 - red) / 76800.0

    return [lpg, rpg, gpg]
