from nrp_core import *
from nrp_core.data.nrp_json import *


@PreprocessedDataPack(keyword='camera', id=DataPackIdentifier('processed_image_2', 'gazebo'))
@TransceiverFunction("husky_brain_2")
def compute_poisson_rates(camera):

    lpg = JsonDataPack("lpg", "husky_brain_2")
    rpg = JsonDataPack("rpg", "husky_brain_2")
    gpg = JsonDataPack("gpg", "husky_brain_2")

    lpg.data['rate'] = 2000.0 * camera.data["left"]
    rpg.data['rate'] = 2000.0 * camera.data["right"]
    gpg.data['rate'] = 75.0 * camera.data["go_on"]

    return [lpg, rpg, gpg]
