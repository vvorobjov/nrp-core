"""PreprocessingFunction: camera frame → per-channel left/right Poisson rates.

The arithmetic core lives in ``helpers.image_to_channel_rates`` so the
unit tests under ``tests/test_tfs.py`` can exercise it without a
``nrp_core`` import (the decorator-driven entrypoint below is what
needs the framework).

Reads ``husky::eye_vision_camera::camera`` (Gazebo) and writes a
``sensory`` JsonDataPack with six per-channel Hz values that
``obstacle_to_avoidance.py`` consumes one TF tick later.

Tracked in EBR2-32.
"""

import numpy as np

from nrp_core import (
    EngineDataPack,
    PreprocessingFunction,
    DataPackIdentifier,
)
from nrp_core.data.nrp_json import JsonDataPack

import params as P
from helpers import image_to_channel_rates


# Module-level tick counter — the decorator framework rebuilds the
# function namespace on every call, so a closure won't survive.
_tick_count = [0]


@EngineDataPack(keyword='camera',
                id=DataPackIdentifier('husky::eye_vision_camera::camera',
                                      'gazebo'))
@PreprocessingFunction("gazebo")
def camera_to_sensory(camera):
    sensory = JsonDataPack("sensory", "gazebo")
    sensory.data["green_left"] = 0.0
    sensory.data["green_right"] = 0.0
    sensory.data["blue_left"] = 0.0
    sensory.data["blue_right"] = 0.0
    sensory.data["grey_left"] = 0.0
    sensory.data["grey_right"] = 0.0

    cam_empty = camera.isEmpty()
    if not cam_empty:
        d = np.frombuffer(camera.data.imageData, np.uint8)
        cv_image = d.reshape((camera.data.imageHeight,
                              camera.data.imageWidth, 3))
        rates = image_to_channel_rates(cv_image)
        for k, v in rates.items():
            sensory.data[k] = v

    _tick_count[0] += 1
    if _tick_count[0] % P.DIAG_LOG_EVERY_N_TICKS == 0:
        # Single grep-friendly line; check_functionality.sh asserts at
        # least one of these appears with any channel rate > 0.
        print(
            f"FORAGING_DIAG cam tick={_tick_count[0]} empty={int(cam_empty)} "
            f"green_l={sensory.data['green_left']:.1f} "
            f"green_r={sensory.data['green_right']:.1f} "
            f"blue_l={sensory.data['blue_left']:.1f} "
            f"blue_r={sensory.data['blue_right']:.1f} "
            f"grey_l={sensory.data['grey_left']:.1f} "
            f"grey_r={sensory.data['grey_right']:.1f}",
            flush=True,
        )

    return [sensory]
