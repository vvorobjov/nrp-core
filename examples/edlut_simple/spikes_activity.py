from nrp_core import *
from nrp_core.event_loop import *
from nrp_core.data.nrp_protobuf import *
import heapq

with open('Activity_A.dat', 'r') as f:
    activity = [[float(num) for num in line.split(' ')] for line in f]

activity_queue = []
for l in activity:
    heapq.heappush(activity_queue, (l[0], l[3]))


@Clock("clock_in")
@ToEngine(keyword="input_spikes", address="/edlut_engine")
@FromEngine(keyword="input_datapack", address="/edlut_engine/spikes_datapack")
@FunctionalNode(name="spikes_activity", outputs=['input_spikes'])
def spikes_activity(input_datapack,clock_in):
    print("EDLUT output data: ")
    print("Spikes firing times: " + str(input_datapack.data.spikes_time[:]))
    print("Neuron indexes: ", input_datapack.data.neuron_indexes[:])


    global activity_queue
    output_datapack = EdlutDataSpikesDataPack("spikes_datapack", "edlut_engine")

    while bool(activity_queue) and activity_queue[0][0] < float(clock_in/1e3)+0.001:

        top_spike = activity_queue[0]
        output_datapack.data.spikes_time.append(top_spike[0])
        output_datapack.data.neuron_indexes.append(int(top_spike[1]))
        heapq.heappop(activity_queue)

    return [output_datapack]

