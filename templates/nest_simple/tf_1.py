from nrp_core import *
from nrp_core.data.nrp_json import *

from math import sin, cos

range_max = 100
sin_x = [abs(1000 * sin(x)) for x in range(range_max)]
cos_x = [abs(1000 * cos(x)) for x in range(range_max)]

n = 0
print(sin_x)


@EngineDataPack(keyword='voltage', id=DataPackIdentifier('voltage', 'nest'))
@TransceiverFunction("nest")
def transceiver_function(voltage):
    # Read voltage
    print(voltage)

    # Set rate
    global n
    n = (n + 1) % range_max

    noise_datapack = JsonDataPack("noise", "nest")
    noise_datapack.data.append({"rate": sin_x[n]})
    noise_datapack.data.append({"rate": cos_x[n]})

    return [noise_datapack]
