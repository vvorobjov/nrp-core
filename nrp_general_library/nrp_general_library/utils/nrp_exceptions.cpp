//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020-2023 NRP Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This project has received funding from the European Union’s Horizon 2020
// Framework Programme for Research and Innovation under the Specific Grant
// Agreement No. 945539 (Human Brain Project SGA3).
//

#include "nrp_general_library/utils/nrp_exceptions.h"


NRPException::~NRPException() = default;

const char *NRPException::what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW
{
    return this->_errMsg.data();
}

template<>
void NRPException::logCreate<void>(const std::string &msg, NRPLogger::spdlog_out_fcn_t spdlogCall)
{
    std::invoke(spdlogCall, msg);
}

NRPExceptionNonRecoverable::~NRPExceptionNonRecoverable() = default;

NRPExceptionRecoverable::~NRPExceptionRecoverable() = default;
