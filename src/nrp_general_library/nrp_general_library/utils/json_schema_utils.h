/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2023 NRP Team
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

#ifndef JSON_SCHEMA_UTILS_H
#define JSON_SCHEMA_UTILS_H

#include "nrp_general_library/config/cmake_constants.h"
#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nlohmann/json-schema.hpp"
#include <fstream>
#include "iostream"

using nlohmann::json;
using nlohmann::json_schema::json_validator;
using nlohmann::json_uri;

using jsonSharedPtr = std::shared_ptr<nlohmann::json>;
using jsonConstSharedPtr = std::shared_ptr<const nlohmann::json>;

namespace json_utils {

    /*!
     * \brief Parse a JSON File and return it's values
     * \param fileName File Name
     * \return Returns parsed JSON
     */
    nlohmann::json parseJSONFile(const std::string &fileName);

    /*!
     * \brief Loads schema from json file given its uri. To be passed to a nlohmann::json_schema::json_validator constructor.
     *
     * The function is parametrized with two parameters: uri and schema.
     * uri provides the path to the json schema within NRP_CONFIG_INSTALL_DIR directory.
     * the schema will be loaded into "schema" json object.
     */
    void jsonSchemaLoader(const json_uri &uri, json &schema);

    /*!
     * \brief  Sets default value for a given parameter
     *
     * The function is parametrized with three parameters: instance, key, new_value.
     * A value 'new_value' for parameter 'key' is set in 'instance'. If 'key' already exists in 'instance' nothing is
     * done
     */
    template <typename VALUE_T>
    static void setDefault(nlohmann::json &instance, const std::string key, const VALUE_T new_value) {
        // if key doesn't exist set it with new_value
        if(!instance.contains(key))
            instance[key] = new_value;
    }


    /*!
     * \brief  Validates a json object using a given json schema
     *
     * Parameters:
     *  instance: json object to be validated
     *  schema_path: URI of the schema to use for validation
     *  addPatch: boolean attribute. If true, parameter default values defined in the schema are added to 'instance'
     *            after validation.
     */
    void validateJson(nlohmann::json &instance, std::string schema_path, bool addPatch = true);

}

#endif // JSON_SCHEMA_UTILS_H

// EOF
