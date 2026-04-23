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

#ifndef NRP_TEST_HELPERS_H
#define NRP_TEST_HELPERS_H

// Header-only helpers for nrp-core unit tests. Intentionally zero non-STL /
// nlohmann_json dependencies so every test target can `#include` it without
// CMake wiring. If a helper here grows a dependency heavier than that, move
// it into its own tests/helpers_*.h and keep this one thin.
//
// See EBR2-19. Extracted from patterns that already appeared in >= 2 test
// files (JSON-from-file load) or that upcoming tests will need (temp dirs).

#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <system_error>

#include <nlohmann/json.hpp>

namespace nrp_test
{

/*!
 * \brief RAII wrapper that creates a unique temporary directory in the
 *        system temp dir at construction and recursively removes it at
 *        destruction. Non-copyable, movable.
 *
 * Intended purely for tests. If creation fails it throws; if cleanup fails
 * it swallows the error (destructors must not throw).
 */
class TempDir
{
    public:
        explicit TempDir(const std::string &prefix = "nrp-test-")
        {
            const std::filesystem::path base = std::filesystem::temp_directory_path();
            std::string tmpl = (base / (prefix + "XXXXXX")).string();
            // mkdtemp mutates the buffer in place; &tmpl[0] is contiguous in C++17.
            if(::mkdtemp(&tmpl[0]) == nullptr)
                throw std::runtime_error(std::string("TempDir: mkdtemp failed: ") + std::strerror(errno));
            _path = std::filesystem::path(tmpl);
        }

        TempDir(const TempDir &) = delete;
        TempDir &operator=(const TempDir &) = delete;

        TempDir(TempDir &&other) noexcept
            : _path(std::move(other._path))
        {   other._path.clear(); }

        TempDir &operator=(TempDir &&other) noexcept
        {
            if(this != &other)
            {
                cleanup();
                _path = std::move(other._path);
                other._path.clear();
            }
            return *this;
        }

        ~TempDir() noexcept
        {   cleanup(); }

        const std::filesystem::path &path() const noexcept
        {   return _path; }

        std::filesystem::path operator/(const std::string &subpath) const
        {   return _path / subpath; }

    private:
        void cleanup() noexcept
        {
            if(_path.empty())
                return;
            std::error_code ec;
            std::filesystem::remove_all(_path, ec);  // ignore ec by design
            _path.clear();
        }

        std::filesystem::path _path;
};

/*!
 * \brief Write `content` to `path`, truncating. Creates parent dirs.
 *        Throws std::runtime_error on failure.
 */
inline void writeTextFile(const std::filesystem::path &path, const std::string &content)
{
    std::error_code ec;
    if(path.has_parent_path())
        std::filesystem::create_directories(path.parent_path(), ec);
    std::ofstream os(path, std::ios::out | std::ios::trunc);
    if(!os)
        throw std::runtime_error("writeTextFile: cannot open " + path.string());
    os << content;
    if(!os)
        throw std::runtime_error("writeTextFile: write failed for " + path.string());
}

/*!
 * \brief Read `path` back as a string. Throws std::runtime_error if the
 *        file cannot be opened.
 */
inline std::string readTextFile(const std::filesystem::path &path)
{
    std::ifstream is(path, std::ios::in);
    if(!is)
        throw std::runtime_error("readTextFile: cannot open " + path.string());
    std::stringstream buf;
    buf << is.rdbuf();
    return buf.str();
}

/*!
 * \brief Load a JSON document from `path`. Replaces the 3-line
 *        `fstream + nlohmann::json::parse` pattern currently duplicated
 *        in nrp_datatransfer_grpc_engine tests.
 *        Throws std::runtime_error if the file cannot be opened, or the
 *        underlying nlohmann::json::parse_error on malformed input.
 */
inline nlohmann::json loadJsonFile(const std::filesystem::path &path)
{
    std::ifstream is(path);
    if(!is)
        throw std::runtime_error("loadJsonFile: cannot open " + path.string());
    return nlohmann::json::parse(is);
}

} // namespace nrp_test

#endif // NRP_TEST_HELPERS_H
