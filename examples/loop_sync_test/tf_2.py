from nrp_core import *

@TransceiverFunction("python_2")
def transceiver_function():
    """Updates iteration counter for this transceiver function"""
    global num_iterations

    num_iterations[1] = num_iterations[1] + 1

    return [  ]

# EOF
