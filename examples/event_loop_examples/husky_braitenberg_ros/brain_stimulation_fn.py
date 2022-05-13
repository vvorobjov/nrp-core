from nrp_core import *
from nrp_core.data.nrp_json import *

from nrp_core.event_loop import *


@ToEngine(keyword="gpg", address="/nest/gpg")
@ToEngine(keyword="rpg", address="/nest/rpg")
@ToEngine(keyword="lpg", address="/nest/lpg")
@FromFunctionalNode('camera', '/image_preprocessor/processed_image')
@FunctionalNode(name="brain_stimulation", outputs=['lpg', 'rpg', 'gpg'])
def compute_poisson_rates(camera):

    lpg = JsonDataPack("lpg", "nest")
    rpg = JsonDataPack("rpg", "nest")
    gpg = JsonDataPack("gpg", "nest")

    lpg.data['rate'] = 2000.0 * camera.data["left"]
    rpg.data['rate'] = 2000.0 * camera.data["right"]
    gpg.data['rate'] = 75.0 * camera.data["go_on"]

    return [lpg, rpg, gpg]

