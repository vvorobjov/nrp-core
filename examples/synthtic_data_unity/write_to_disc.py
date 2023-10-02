"""Python Engine . Will get current frame and make it accessible as a datapack"""

from nrp_core.engines.python_grpc import GrpcEngineScript
from nrp_core.data.nrp_protobuf import nrpgenericproto_pb2
import json
class Script(GrpcEngineScript):
    def initialize(self):
        """Initialize python image receiver"""
        print("Python Engine is initializing. Registering datapack...")
        self._registerDataPack("image", nrpgenericproto_pb2.Image)
        self._registerDataPack("cameraSensorStrings", nrpgenericproto_pb2.ArrayString)
        self._registerDataPack("cameraSensorFloats", nrpgenericproto_pb2.ArrayFloat)
        self._registerDataPack("cameraSensorInts", nrpgenericproto_pb2.ArrayInt32)
        self._registerDataPack("cameraSensorBools", nrpgenericproto_pb2.ArrayBool)
        self.counter = 0
        self.sensorDefinitionsDict = {"sensorDefinitions":[]}
        self.metricDefinitionsDict = {"metricDefinitions":[]}
        self.metadataDict = dict()
        self.annotationDefinitionsDict = {"annotationDefinitions":[]}

    def runLoop(self, timestep_ns):
        """receive datapack and dump it on the disc"""
        snapshot = self._getDataPack("image").data
        if self.counter == 0:
            strings = self._getDataPack("cameraSensorStrings").array
            floats = self._getDataPack("cameraSensorFloats").array
            ints = self._getDataPack("cameraSensorInts").array
            bools = self._getDataPack("cameraSensorBools").array


            self.sensorDefinitionsDict["sensorDefinitions"] = []
            self.sensorDefinitionsDict["sensorDefinitions"].append({"@type":strings[0], \
                                                             "id":strings[1],\
                                                             "modality":strings[2], \
                                                             "description":strings[3], \
                                                             "firstCaptureFrame": floats[0], \
                                                             "captureTriggerMode":strings[4], \
                                                             "simulationDeltaTime":floats[1], \
                                                             "framesBetweenCaptures":ints[0], \
                                                             "manualSensorsAffectTiming":bools[0]})
            self.annotationDefinitionsDict["annotationDefinitions"] = []

            self.annotationDefinitionsDict["annotationDefinitions"].append({"@type":strings[5], \
                                                                "id":strings[6],\
                                                                "description":strings[7], \
                                                                "spec": [{ "label_id":int(strings[8]), "label_name":strings[9] }] })

            self.metricDefinitionsDict["metricDefinitions"] = []
            self.metricDefinitionsDict["metricDefinitions"].append({"@type":strings[10], \
                                                                "id":strings[11],\
                                                                "description":strings[12] })
            self.metricDefinitionsDict["metricDefinitions"].append({"@type": strings[13], \
                                                                    "id": strings[14], \
                                                                    "description": strings[15]})

            sensor_ready_to_dump = json.dumps(self.sensorDefinitionsDict, indent=2)
            # print(sensor_ready_to_dump)
            annotation_ready_to_dump = json.dumps(self.annotationDefinitionsDict, indent=2)
            # print(annotation_ready_to_dump)
            metric_ready_to_dump = json.dumps(self.metricDefinitionsDict, indent=2)
            # print(metric_ready_to_dump)

            with open(f"./output/metric_definitions.json", 'w') as f:
                f.write(metric_ready_to_dump)

            with open(f"./output/annotationDefinition.json", 'w') as f:
                f.write(annotation_ready_to_dump)
            with open(f"./output/sensorDefinition.json", 'w') as f:
                f.write(sensor_ready_to_dump)

        self.counter += 1
        with open(f"./output/rgbout/frame_{self.counter}.png", 'wb') as f:
            f.write(snapshot)

    def shutdown(self):
        print("Python Engine  is shutting down")

    def reset(self):
        print("Python Engine is resetting")
