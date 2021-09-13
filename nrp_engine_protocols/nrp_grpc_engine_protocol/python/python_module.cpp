#include <boost/python.hpp>

#include "nrp_grpc_engine_protocol/config/cmake_constants.h"
#include "nrp_grpc_engine_protocol/grpc_server/engine_grpc.grpc.pb.h"
#include "nrp_grpc_engine_protocol/engine_client/engine_grpc_client.h"

#include "nrp_general_library/device_interface/device.h"

#include "proto_python_bindings/proto_python_bindings.h"
#include "proto_python_bindings/repeated_field_proxy.h"

BOOST_PYTHON_MODULE(PROTO_PYTHON_MODULE_NAME)
{
    // Import General NRP Python Module
    boost::python::import(PYTHON_MODULE_NAME_STR);

    RepeatedScalarFieldProxy::create();
    RepeatedScalarFieldIterProxy::create();

    // TODO: python wrappers for all compiled proto message definitions should be automatically generated

    proto_python_bindings<EngineGrpc::GazeboCamera>::create();
    Device<EngineGrpc::GazeboCamera>::create_python("CameraDevice");

    proto_python_bindings<EngineGrpc::GazeboLink>::create();
    Device<EngineGrpc::GazeboJoint>::create_python("LinkDevice");

    proto_python_bindings<EngineGrpc::GazeboJoint>::create();
    Device<EngineGrpc::GazeboJoint>::create_python("JointDevice");

    proto_python_bindings<EngineGrpc::TestPayload>::create();
}