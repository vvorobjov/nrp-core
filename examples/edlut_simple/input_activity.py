from nrp_core import *
from nrp_core.event_loop import *
from nrp_core.data.nrp_protobuf import *
import heapq
import matplotlib.pyplot as plt

# Inject input spikes to the queue
input_queue = []

time = 0.001
while time < 1.0:
    heapq.heappush(input_queue, (time, 0))
    time += 0.005
time = 0.004
while time < 1.0:
    heapq.heappush(input_queue, (time, 1))
    time += 0.050
time = 0.002
while time < 1.0:
    heapq.heappush(input_queue, (time, 2))
    time += 0.010

# Inject input currents to the queue
time = 0.010
while time < 1.0:
    heapq.heappush(input_queue, (time, 3, 8.0))
    time += 0.100
    heapq.heappush(input_queue, (time, 3, 5.0))
    time += 0.100

output_indexes = []
output_times = []

@Clock("clock_in")
@ToEngine(keyword="input_currents", address="/edlut_engine")
@ToEngine(keyword="input_spikes", address="/edlut_engine")
@FromEngine(keyword="input_datapack", address="/edlut_engine/spikes_datapack")
@FunctionalNode(name="input_activity", outputs=['input_spikes','input_currents'])
def input_activity(input_datapack, clock_in):
    if(bool(input_datapack.data.spikes_time[:])):
        for i,j in zip((input_datapack.data.spikes_time[:]),input_datapack.data.neuron_indexes[:]):
            output_indexes.append(j)
            output_times.append(i)


    if(float(clock_in/1e3) +0.001 >= 1.0):
        plt.scatter(output_times, output_indexes, alpha=0.5)
        plt.xlim(0, 1.0)
        plt.xlabel('time (s)')
        plt.ylabel('5 = spikes (AMPA, GABA and NMDA);   6 = square current (5 Hz);   7 = sin current (2 Hz)')
        plt.savefig("output.png")
    global spikes
    global currents
    global input_queue

    sp_datapack = EdlutDataSpikesDataPack("spikes_datapack", "edlut_engine")
    cur_datapack = EdlutDataCurrentsDataPack("currents_datapack", "edlut_engine")
    print(float(clock_in/1e3))


    while bool(input_queue) and input_queue[0][0] < float(clock_in/1e3) +0.001:
        if(len(input_queue[0]) == 2):
            top_spike = input_queue[0]
            sp_datapack.data.spikes_time.append(top_spike[0])
            sp_datapack.data.neuron_indexes.append(int(top_spike[1]))
            heapq.heappop(input_queue)
        else:
            top_current = input_queue[0]
            cur_datapack.data.spikes_time.append(top_current[0])
            cur_datapack.data.neuron_indexes.append(int(top_current[1]))
            cur_datapack.data.current_values.append((top_current[2]))
            heapq.heappop(input_queue)


    return [sp_datapack,cur_datapack]

