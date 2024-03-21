#include <boost/python.hpp>
#include <nlohmann/json.hpp>

#include "nrp_python_json_engine/nrp_client/python_engine_json_nrp_client.h"
#include "nrp_python_json_engine/config/cmake_constants.h"
#include "nrp_python_json_engine/config/python_config.h"


/*!
 * \brief Helper function for instantiating a PythonEngineJSONNRPClient to validate a PythonEngine Engine configuration
 *
 * \param config engine configuration to be processed
 * \return validated config
 */
std::string parseAndValidateEngineConfig(const std::string& configFile)
{
    nlohmann::json config(json_utils::parseJSONFile(configFile));
    json_utils::setDefault<std::string>(config.at("EngineConfig"), "EngineType", PythonConfigConst::EngineType);
    json_utils::validateJson(config, "json://nrp-core/event_loop.json#/event_loop_engine");

    PythonEngineJSONNRPClient client(config.at("EngineConfig"), ProcessLauncherInterface::unique_ptr{});
    config.at("EngineConfig") = client.engineConfig();

    return config.dump();
}

BOOST_PYTHON_MODULE(NRP_PYTHON_ENGINE_MODULE)
{
    boost::python::def("parseAndValidateEngineConfig", parseAndValidateEngineConfig);
}
