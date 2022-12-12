from nrp_core import *
from nrp_core.data.nrp_json import *
from nrp_core.event_loop import *

# Engine time from the last transceiver function call
engine_times = [0, 0, 0, 0]

# Number of times a transceiver function was called
# Shared between all transceiver functions
num_iterations = [0, 0, 0, 0]

# Load the expected results from the file
expected_results = []
with open('expected_results.txt', 'r') as fd:
    expected_results = fd.readlines()


@FromEngine(keyword='datapack_python1', address='/python_1/datapack1')
@FromEngine(keyword='datapack_python2', address='/python_2/datapack2')
@FromEngine(keyword='datapack_python3', address='/python_3/datapack3')
@FromEngine(keyword='nest_kernel', address='/nest/kernel')
# Only to make this node to be executed when python_1 is synced
@ToEngine(keyword="dummy", address="/python_1/dummy")
# Only to ensure that this FN is executed after all the rest at every cycle
@FromFunctionalNode(keyword='dummy2', address='/tf_1/dummy')
@FunctionalNode(name="tf_join", outputs=["dummy"])
def transceiver_function(datapack_python1, datapack_python2, datapack_python3, nest_kernel, dummy2):
    """The main transceiver function of the test, performs the actual testing

    There are 4 engines in the test. Every engine is running with a timestep
    2 times greater than the previous one. It means that the frequency will be 2 times smaller.
    The test checks if the engine time coming from each of the engines matches this assumption.
    Moreover, every engine has a transceiver function associated with it.
    The test will check if the transceiver functions are called at the same frequency
    as the engines they are bound to.
    """
    global num_iterations
    global engine_times

    # Get the expected values for this iteration
    expected_values = expected_results[num_iterations[0]-1].split()

    # Update engine times for all engines
    # This must be done in the fastest-running transceiver function in order to check
    # if the datapacks from the slower-running engines aren't updated more frequently
    # than expected

    engine_times[0] = datapack_python1.data["time"]
    engine_times[1] = datapack_python2.data["time"]
    engine_times[2] = int(nest_kernel.data["biological_time"] * 1e6)
    engine_times[3] = datapack_python3.data["time"]

    # Print values from current iterations for all engines
    for i, j in zip(num_iterations, engine_times):
        print(f'{i:3}', f'{j:10}', end="")
    print()

    # Get a flat list that corresponds to zipped (num_iterations, engine_times)
    tested_values = [item for pair in zip(num_iterations, engine_times) for item in pair]

    # Test if number of iterations and engine times are matching the template results
    for i, exp_value in enumerate(expected_values):
        if int(exp_value) != tested_values[i]:
            raise ValueError(
                f"Tested value {tested_values[i]} doesn't match the expected value {int(exp_value)}, at line {num_iterations[0]} column {i}")

    return [JsonDataPack("dummy", "python_1")]

# EOF
