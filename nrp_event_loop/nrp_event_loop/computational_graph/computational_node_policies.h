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

#ifndef COMPUTATION_NODE_POLICIES_H
#define COMPUTATION_NODE_POLICIES_H

namespace InputNodePolicies {
    /*! \brief Defines input node message cache behavior */
    enum MsgCachePolicy {
        CLEAR_CACHE, /*!< if no new msg arrives, stored msgs are replaced by nullptr in the next cycle  */
        KEEP_CACHE /*!< if no new msg arrives, stored msgs are kept  */
    };

    /*! \brief Defines how an input node publish stored msgs */
    enum MsgPublishPolicy {
        LAST, /*!< only sends the last msg received  */
        ALL /*!< sends all msgs received since last 'compute' call  */
    };
}

namespace FunctionalNodePolicies {
    /*! \brief Possible execution policies for this node */
    enum ExecutionPolicy {
        ALWAYS, /*!< the node is always executed when 'compute' is called */
        ON_NEW_INPUT /*!< the node is executed only if at least one of its inputs have a fresh value */
    };
}

namespace OutputNodePolicies {

    /*! \brief Defines how output nodes send stored msgs */
    enum PublishFormatPolicy {
        SERIES, /*!< sends received msgs one by one */
        BATCH /*!< sends all msgs received in a single batch  */
    };
}

#endif //COMPUTATION_NODE_POLICIES_H
