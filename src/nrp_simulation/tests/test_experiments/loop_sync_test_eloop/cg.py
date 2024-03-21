from nrp_core import *
from nrp_core.event_loop import *
from math import floor

# Number of times a functional node is called
# Shared between all fns
num_execs = [0, 0, 0, 0]

# Compute expected results
l1 = [i+1 for i in range(100)]
l2 = [floor(i/2) + 1 for i in range(100)]
l3 = [floor(i/3) + 1 for i in range(100)]
l4 = [floor(i/4) + 1 for i in range(100)]
expected_results = [e for e in zip(l1,l2,l3,l4)]


@OutputDummy('dummy', 'odummy2', compute_period=2)
@FunctionalNode(name="fn2", outputs=["dummy"], exec_policy=node_policies.functional_node.exec_policy.always)
def fn_body():
    global num_execs
    num_execs[1] += 1
    return [0]

@OutputDummy('dummy', 'odummy3', compute_period=3)
@FunctionalNode(name="fn3", outputs=["dummy"], exec_policy=node_policies.functional_node.exec_policy.always)
def fn_body():
    global num_execs
    num_execs[2] += 1
    return [0]

@OutputDummy('dummy', 'odummy4', compute_period=4)
@FunctionalNode(name="fn4", outputs=["dummy"], exec_policy=node_policies.functional_node.exec_policy.always)
def fn_body():
    global num_execs
    num_execs[3] += 1
    return [0]

@OutputDummy('dummy', 'odummy1', compute_period=1)
@FunctionalNode(name="fn1", outputs=["dummy"], exec_policy=node_policies.functional_node.exec_policy.always)
def fn_body():
    """The main transceiver function of the test, performs the actual testing

    There are 4 engines in the test. Every engine is running with a timestep
    2 times greater than the previous one. It means that the frequency will be 2 times smaller.
    The test checks if the engine time coming from each of the engines matches this assumption.
    Moreover, every engine has a transceiver function associated with it.
    The test will check if the transceiver functions are called at the same frequency
    as the engines they are bound to.
    """
    global num_execs

    # Get the expected values for this iteration before num_execs is incremented
    expected_values = expected_results[num_execs[0]]

    # Update iteration counter
    num_execs[0] += 1

    # Print values from current iterations
    for i in num_execs:
        print(i, end=" ")
    print()

    # Test if number of iterations and engine times are matching the template results
    for i, exp_value in enumerate(expected_values):
        if exp_value != num_execs[i]:
            raise ValueError(
                f"Tested value {num_execs[i]} doesn't match the expected value {int(exp_value)}, at line {num_execs[0]}")

    return [0]

# EOF
