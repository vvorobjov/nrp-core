from nrp_core import *
from nrp_core.data.nrp_json import *

# Engine time from the last transceiver function call
engine_times = [0, 0, 0, 0]

# Number of times a transceiver function was called
# Shared between all transceiver functions
num_iterations = [0, 0, 0, 0]

# Load the expected results from the file
expected_results = []
with open('expected_results.txt', 'r') as fd:
    expected_results = fd.readlines()


@EngineDataPack(keyword='datapack_python1', id=DataPackIdentifier('datapack1', 'python_1'))
@EngineDataPack(keyword='datapack_python2', id=DataPackIdentifier('datapack2', 'python_2'))
@EngineDataPack(keyword='datapack_python3', id=DataPackIdentifier('datapack3', 'python_3'))
@EngineDataPack(keyword='nest_kernel', id=DataPackIdentifier('kernel', 'nest'))
@TransceiverFunction("python_1")
def transceiver_function(datapack_python1, datapack_python2, datapack_python3, nest_kernel):
    """The main transceiver function of the test, performs the actual testing

    There are 4 engines in the test. Every engine is running with a timestep
    2 times greater than the previous one. It means that the frequency will be 2 times smaller.
    The test checks if the engine time coming from each of the engines matches this assumption.
    Moreover, every engine has a transceiver function associated with it.
    The test will check if the transceiver functions are called at the same frequency
    as the engines they are bound to.

    NOTE: the value of 'num_iterations' when it is read by this function to compose 'tested_values' will depend on the
    order in which tfs are executed within a loop step. Therefore, 'expected_values' assume a concrete order. If the
    order of execution change, which will not affect the synchronization behavior, the test will fail.
    """
    global num_iterations
    global engine_times

    # Get the expected values for this iteration before num_iterations is incremented

    expected_values = expected_results[num_iterations[0]].split()

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
                f"Tested value {tested_values[i]} doesn't match the expected value {int(exp_value)}, at line {num_iterations[0]}")

    # Update iteration counter for this transceiver function
    num_iterations[0] += 1

    return []

# EOF
