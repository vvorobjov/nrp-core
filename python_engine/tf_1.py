import random

from nrp_core import *
from nrp_core.data.nrp_json import *
from nrp_core.data.nrp_protobuf import *

def get_info(info):
    print("Board position: " + str(info.board_position))
    print("Board rotation: " + str(info.board_rotation))
    print("Joint angles: " + str(info.joint_angles))

def get_camera(camera, i):
    _retrieve_image(camera, i)

def _retrieve_image(reply, i):
    # how to access the image data from reply.imageData and store it in a file, as an example
    # create folder "images" in the python workind directory and uncomment the lines below if you want to save the file
    with open('images/received-image-{}.{}'.format(i, 'jpg'), 'wb') as image_file:
        image_file.write(reply.image_data)
    pass

iteration = 0

@EngineDataPack(keyword='info', id=DataPackIdentifier('get_info', 'unity_grpc'))
@EngineDataPack(keyword='camera', id=DataPackIdentifier('get_camera', 'unity_grpc'))
@TransceiverFunction("python")
def transceiver_function(info, camera):

    device1 = JsonDataPack("device1", "python")

    # Process data coming from the game
    if not info.isEmpty():
        print(info.engine_name)
        print(info.type)
    else:
        print("EMPTY")
    # global iteration
    # get_info(info)
    # get_camera(camera, iteration)
    # iteration += 1

    # Send processed data to python script

    return [device1]

# EOF