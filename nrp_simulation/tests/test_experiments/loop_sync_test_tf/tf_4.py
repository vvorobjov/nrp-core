from nrp_core import *


@TransceiverFunction("python_3")
def transceiver_function():
    """Updates iteration counter for this transceiver function"""
    global num_iterations

    num_iterations[3] += 1

    return []

# EOF
