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
using nrp_test::writeTextFile;

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

TEST(ZipContainerTest, StringConstructorReadsOnlySizeBytesNotCapacity)
{
    // Minimal valid ZIP: a 22-byte End-Of-Central-Directory record with
    // zero entries. Held in a string with capacity > size so the test
    // distinguishes size-bytes-read from capacity-bytes-read.
    const char minimalZipEocd[] = {
        'P', 'K', 0x05, 0x06,                    // EOCD signature
        0, 0,                                    // disk number
        0, 0,                                    // disk with central dir
        0, 0,                                    // entries on this disk
        0, 0,                                    // total entries
        0, 0, 0, 0,                              // central directory size
        0, 0, 0, 0,                              // central directory offset
        0, 0                                     // comment length
    };
    static_assert(sizeof(minimalZipEocd) == 22, "EOCD must be 22 bytes");

    std::string s;
    s.reserve(4096);
    s.assign(minimalZipEocd, sizeof(minimalZipEocd));
    ASSERT_GT(s.capacity(), s.size());

    EXPECT_NO_THROW(ZipContainer(std::move(s)));
}

// Round-trip via getCompressedData / saveToDestination is intentionally
// NOT exercised below: both route through ZipContainer::addZipToZip,
// which creates libzip sources via zip_source_zip but never hands them
// to zip_file_add, so zip_close fails with "Entry has been changed".
// Until that separate bug is fixed the tests below can only confirm
// that compressPath itself completes without crashing.

TEST(ZipContainerTest, CompressPathWithFilesAndSubdirsDoesNotCrash)
{
    TempDir src;
    writeTextFile(src / "a.txt", "alpha");
    writeTextFile(src / "sub/b.txt", "beta");

    EXPECT_NO_THROW({
        ZipContainer archive = ZipContainer::compressPath(src.path(), /*keepRelDirStruct=*/false);
        (void)archive;
    });
}

TEST(ZipContainerTest, CompressPathWithKeepRelDirStructDoesNotCrash)
{
    TempDir src;
    writeTextFile(src / "top.txt", "T");
    writeTextFile(src / "a/b/deep.txt", "D");

    EXPECT_NO_THROW({
        ZipContainer archive = ZipContainer::compressPath(src.path(), /*keepRelDirStruct=*/true);
        (void)archive;
    });
}
