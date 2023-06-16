#!/usr/bin/env python3

# NRP Core - Backend infrastructure to synchronize simulations
#
# Copyright 2020-2021 NRP Team
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# This project has received funding from the European Union’s Horizon 2020
# Framework Programme for Research and Innovation under the Specific Grant
# Agreement No. 945539 (Human Brain Project SGA3).


from argparse import Namespace, ArgumentParser
import nrp_core.engines.python_grpc.grpc_server_callbacks as server_callbacks
import urllib.parse
import json
import grpc
from concurrent import futures
import nrp_protobuf.engine_grpc_pb2 as python_grpc_engine_pb2
import nrp_protobuf.engine_grpc_pb2_grpc as python_grpc_engine_pb2_grpc

import time

class StandaloneApplication(python_grpc_engine_pb2_grpc.EngineGrpcServiceServicer):
    def initialize(self, request, context):
        res = server_callbacks.initialize(json.loads(request.json))
        return python_grpc_engine_pb2.InitializeReply(json=json.dumps(res))

    def shutdown(self, request, context):
        res = server_callbacks.shutdown(json.loads(request.json))
        return python_grpc_engine_pb2.ShutdownReply(json=json.dumps(res))

    def reset(self, request, context):
        res = server_callbacks.reset(json.loads(request.json))
        return python_grpc_engine_pb2.ResetReply(json=json.dumps(res))

    def runLoopStep(self, request, context):
        res = server_callbacks.run_loop(request.timeStep)
        return python_grpc_engine_pb2.RunLoopStepReply(engineTime=res)

    def setDataPacks(self, request, context):
        server_callbacks.set_datapacks(request)
        return python_grpc_engine_pb2.SetDataPacksReply()

    def getDataPacks(self, request, context):
        res = server_callbacks.get_datapacks(request)
        return res


if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument('--engine',     type=str, required=True)
    parser.add_argument('--serverurl',  type=str, required=True)
    args = parser.parse_args()
    
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=5))
    python_grpc_engine_pb2_grpc.add_EngineGrpcServiceServicer_to_server(
        StandaloneApplication(), server)
    server.add_insecure_port(args.serverurl)
    server.start()
    print("grpc server start...")
    server.wait_for_termination()

# EOF
