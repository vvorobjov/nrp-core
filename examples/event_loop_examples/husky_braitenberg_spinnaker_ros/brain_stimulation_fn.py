from nrp_core import *
from nrp_core.data.nrp_json import *

from nrp_core.event_loop import *


@ToSpinnaker(keyword="lpg", address="red_left_eye_poisson_control")
@ToSpinnaker(keyword="rpg", address="red_right_eye_poisson_control")
@ToSpinnaker(keyword="gpg", address="green_blue_eye_poisson_control")
@FromFunctionalNode('camera', '/image_preprocessor/processed_image')
@FunctionalNode(name="brain_stimulation", outputs=['lpg', 'rpg', 'gpg'])
def compute_poisson_rates(camera):

    lpg = NlohmannJson()
    rpg = NlohmannJson()
    gpg = NlohmannJson()

    lpg["rates"] = [{"neuron_id": 0, "rate": 2000.0 * camera.data["left"]}]
    rpg["rates"] = [{"neuron_id": 0, "rate": 2000.0 * camera.data["right"]}]
    gpg["rates"] = [{"neuron_id": 0, "rate": 75.0 * camera.data["go_on"]}]

    return [lpg, rpg, gpg]

