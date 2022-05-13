/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2021 NRP Team
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project has received funding from the European Union’s Horizon 2020
 * Framework Programme for Research and Innovation under the Specific Grant
 * Agreement No. 945539 (Human Brain Project SGA3).
 */

#include "nrp_general_library/utils/json_schema_utils.h"

namespace json_utils {

    nlohmann::json parseJSONFile(const std::string &fileName)
    {
        // Try to open file
        std::ifstream cfgFile(fileName);
        if(!cfgFile.is_open())
        {
            throw std::invalid_argument("Error: Could not open config file " + fileName);
        }

        // Parse JSON from file
        nlohmann::json cfgJSON;
        try
        {
            cfgFile >> cfgJSON;
        }
        catch(std::exception &e)
        {
            throw std::invalid_argument("Error: Could not parse config file " + fileName + "\n" + e.what());
        }

        return cfgJSON;
    }

    void jsonSchemaLoader(const json_uri &uri, json &schema) {
        std::string schema_path = NRP_CONFIG_INSTALL_DIR;
        schema_path += uri.path();

        std::fstream s(schema_path.c_str());
        if (!s.good())
            throw std::invalid_argument("could not open " + uri.url() + " for schema loading\n");

        try {
            s >> schema;
        } catch (std::exception &e) {
            throw e;
        }
    }

    void validateJson(nlohmann::json &instance, std::string schema_path, bool addPatch) {
        // set schema
        nlohmann::json schema = {{"$ref", schema_path}};

        json_validator validator(jsonSchemaLoader);
        try {
            validator.set_root_schema(schema);
        } catch (std::exception &e) {
            throw NRPException::logCreate(e, "Loading of schema " + schema_path + " failed");
        }

        // validate json
        nlohmann::json default_patch;
        try {
            default_patch = validator.validate(instance);
        } catch (std::exception &e) {
            throw NRPException::logCreate(e, "Validation with schema " + schema_path + " failed");
        }

        //std::cout << default_patch.dump(4) << "\n";

        // add patch with default values
        if(!default_patch.empty() and addPatch)
            instance = instance.patch(default_patch);
    }

}

// EOF
