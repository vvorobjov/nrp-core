from nrp_core import *
from nrp_core.data.nrp_json import *


@PreprocessedDataPack(keyword='camera', id=DataPackIdentifier('processed_image', 'gazebo'))
@TransceiverFunction("nest")
def compute_poisson_rates(camera):

    lpg = JsonDataPack("lpg", "nest")
    rpg = JsonDataPack("rpg", "nest")
    gpg = JsonDataPack("gpg", "nest")

    lpg.data['rate'] = 2000.0 * camera.data["left"]
    rpg.data['rate'] = 2000.0 * camera.data["right"]
    gpg.data['rate'] = 75.0 * camera.data["go_on"]

    return [lpg, rpg, gpg]
