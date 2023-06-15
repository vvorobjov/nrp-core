#include <boost/python.hpp>
#include <nlohmann/json.hpp>

#include "nrp_python_grpc_engine/nrp_client/python_engine_grpc_nrp_client.h"
#include "nrp_python_grpc_engine/config/cmake_constants.h"
#include "nrp_python_grpc_engine/config/python_grpc_config.h"


/*!
 * \brief Helper function for instantiating a PythonEngineGRPCNRPClient to validate a PythonEngineGRPC Engine configuration
 *
 * \param config engine configuration to be processed
 * \return validated config
 */
std::string parseAndValidateEngineConfig(const std::string& configFile)
{
    nlohmann::json config(json_utils::parseJSONFile(configFile));
    json_utils::setDefault<std::string>(config.at("EngineConfig"), "EngineType", PythonGrpcConfigConst::EngineType);
    json_utils::validateJson(config, "json://nrp-core/event_loop.json#/event_loop_engine");

    PythonEngineGRPCNRPClient client(config.at("EngineConfig"), ProcessLauncherInterface::unique_ptr{});
    config.at("EngineConfig") = client.engineConfig();

    return config.dump();
}

BOOST_PYTHON_MODULE(NRP_PYTHON_GRPC_ENGINE_MODULE)
{
    boost::python::def("parseAndValidateEngineConfig", parseAndValidateEngineConfig);
}
