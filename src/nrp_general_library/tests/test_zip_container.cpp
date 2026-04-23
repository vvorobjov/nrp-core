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

#include "nrp_general_library/utils/zip_container.h"
#include "tests/test_helpers.h"

#include <gtest/gtest.h>

#include <cstdint>
#include <string>
#include <vector>

using nrp_test::TempDir;

// Happy-path coverage (compressPath + getCompressedData + extractZipFiles)
// is deliberately NOT added here: writing those tests during EBR2-19
// surfaced a double-free in ZipSourceWrapper's destructor (see
// docs/probable_bugs.md PB-6). Per the "cover, don't fix" rule of this
// branch, we leave the crashing path untested until that bug has its own
// ticket. The two error-path tests below exercise the constructors that
// do NOT rely on the affected code path.

TEST(ZipContainerTest, OpenMissingArchivePathThrows)
{
    TempDir tmp;
    const std::string missing = (tmp / "does-not-exist.zip").string();
    EXPECT_THROW(
        ZipContainer(missing, /*readOnly=*/true, /*saveOnDestruct=*/false),
        std::exception
    );
}

TEST(ZipContainerTest, ConstructFromGarbageBytesThrows)
{
    std::vector<uint8_t> garbage{'n', 'o', 't', '-', 'a', '-', 'z', 'i', 'p'};
    EXPECT_THROW(ZipContainer(std::move(garbage)), std::exception);
}
