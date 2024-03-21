from nrp_core import *


@TransceiverFunction("nest")
def transceiver_function():
    """Updates iteration counter for this transceiver function"""
    global num_iterations

    num_iterations[2] += 1

    return []

# EOF
