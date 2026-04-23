# CLAUDE.md

Operating instructions for Claude Code (and any Claude-family assistant) working
in this repository. Keep this file short, correct, and machine-actionable — it
is loaded into every conversation.

## The two rules that always win

1. **No change lands unless the full unit-test suite passes inside the
   canonical container** (`nrp-local/nrp-nest-gazebo-ubuntu20:local`). This is
   the acceptance gate for every commit, merge, and review. If you cannot run
   the tests, say so explicitly — do not claim the task is done.
   The canonical *local* verification command is
   [`bash .ci/00-dev-rebuild-and-test.sh`](.ci/00-dev-rebuild-and-test.sh) — it
   runs from the host, ensures the devcontainer image, and executes configure
   + build + install + ctest inside it with no Jenkins-specific exit-bit
   masking. Prefer it over invoking the 11/20/30 scripts by hand.
   See [Running the tests](#running-the-tests) and
   [If tests fail](#if-tests-fail).

2. **Every fix or improvement is scoped to a Jira ticket on the
   [EBR2 board](https://hbpneurorobotics.atlassian.net/jira/software/c/projects/EBR2/boards/11).**
   Work happens on a branch whose name contains the issue key; every commit
   subject starts with the same key in brackets. See
   [Issue tracking and branch convention](#issue-tracking-and-branch-convention).

## Issue tracking and branch convention

- **Tracker:** Atlassian cloud at `hbpneurorobotics.atlassian.net`, project
  key **EBR2**, board 11.
- **One ticket per change.** Before you start, create (or claim) an EBR2 issue
  describing the problem and acceptance criteria. If you don't have direct
  Jira access from your Claude Code session, ask the user to create it via
  claude.ai (the repo's `docs/jira_prompt.md` has the ready-made prompt).
- **Branch name:** `EBR2-<num>-<short-slug>`, lower-kebab-case slug, ≤ 60
  chars total. Examples: `EBR2-234-fix-entrypoint-newline`,
  `EBR2-238-bump-paho-mqtt-1-4-1`.
- **Commit subject:** `[EBR2-<num>] <imperative summary>`. The `<num>` is the
  same as the branch's. Multiple commits on one branch are fine — all share
  the same key.
- **PR title:** `[EBR2-<num>] <summary>` and the PR description must link
  back to the issue URL.
- **Exception — placeholder keys.** You'll see `[EBR2-0000]` /
  `[EBR2-000]` in older commits. Don't use them for new work. If you find
  yourself tempted, open an issue first.
- **Merging:** do not squash commits that already carry the correct ticket
  key — the prefix is the audit trail.

If the issue key doesn't exist yet, stop and create it. A PR without a real
ticket will be bounced in review.

### Before starting work — you must hand the user a ready-to-paste claude.ai prompt

Claude Code cannot create Jira issues directly (no Atlassian MCP in this
session). Claude.ai can. To keep the user's effort to a single copy-paste:

1. **As soon as** you identify a new problem or improvement — before reading
   more code, before writing a plan, before touching anything — compose the
   issue text in your head (one-line summary, 1–3 sentence context, concrete
   acceptance criteria) and present it to the user **in the exact format from
   [docs/jira_prompt.md](docs/jira_prompt.md)** with every bracketed field
   already filled in.
2. Do this unprompted. Don't ask "should I draft the Jira prompt?" —
   just draft it. Output it as a single fenced code block so the user can
   paste without cleanup. Say one sentence above it: *"Paste this into
   claude.ai to open the ticket; reply with the issue key."*
3. **Stop and wait** for the user to come back with the issue key before
   writing code, creating a branch, or staging changes. Use the time to
   prepare — read related files, plan the diff — but do not commit or edit
   until the key is known.
4. When the user replies with the key (e.g. `EBR2-238`), immediately:
   - create the branch `EBR2-238-<slug>` off the current base,
   - use `[EBR2-238] …` in every commit subject on that branch,
   - reference the issue URL in the PR body.
5. If the user groups multiple small fixes under one ticket, use the short
   variant of the prompt. Don't unilaterally split one change across two
   tickets — ask.

This rule applies to every fix, refactor, doc-only update, and dependency
bump. The only exception is work explicitly scoped to an already-created
ticket (the user will tell you the key up front).

## What nrp-core is

`nrp-core` is the Neurorobotics Platform core framework: a C++17 synchronous
simulation engine that couples neural simulators (NEST, EDLUT, SpiNNaker),
physics/robotics simulators (Gazebo, OpenSim, PyBullet), and Python-driven
models through a client/server *Engine* abstraction and a *Computational Graph*
event loop. The entry binary is `NRPCoreSim`.

Top-level layout:

| Path | Purpose |
|---|---|
| [src/nrp_general_library/](src/nrp_general_library/) | Core library: Engine base classes, DataPacks, Python bindings, plugin manager |
| [src/nrp_engine_protocols/](src/nrp_engine_protocols/) | gRPC and JSON transport layers for Engines |
| [src/nrp_simulation/](src/nrp_simulation/) | `NRPCoreSim` executable, FTILoop, SimulationManager |
| [src/nrp_event_loop/](src/nrp_event_loop/) | Computational Graph (C++ + Python), ROS / MQTT nodes |
| [src/nrp_gazebo_engines/](src/nrp_gazebo_engines/) | Gazebo JSON + gRPC engines and plugins |
| [src/nrp_nest_engines/](src/nrp_nest_engines/) | NEST JSON engine and `nest-server` |
| [src/nrp_python_*_engine/](src/) | Python JSON and Python gRPC engines |
| [src/nrp_pysim_engines/](src/nrp_pysim_engines/) | PyBullet / OpenSim Python-driven engines |
| [src/nrp_protobuf/](src/nrp_protobuf/) | protobuf-defined message types and bindings |
| [src/nrp-core-msgs/](src/nrp-core-msgs/) | git submodule — proto + ROS msg definitions |
| [src/nrp_scripts/](src/nrp_scripts/) | Container entrypoint, `xvfb-run-nrp`, gazebo launchers |
| [examples/](examples/) | Reference experiments, also exercised by `ENABLE_EXAMPLES_TESTING=ON` |
| [dockerfiles/](dockerfiles/) | Dockerfiles for base/gazebo/nest/opensim/tvb/xpra/nvidia-cuda stacks |
| [.ci/](.ci/) | Jenkins-driven build + test pipeline, cmake cache presets, apt dep lists |
| [.devcontainer/](.devcontainer/) | VSCode devcontainer config — points at the canonical image |

## The canonical build+test environment

The project has only ever been validated on **Ubuntu 20.04** with a very
specific set of pinned dependencies (Gazebo 11, NEST 3.1, gRPC from the focal
PPA, ROS Noetic, Python 3.8, Boost.Python 1.71). Ubuntu 22.04 is known to build
(commit `4f047ecc` on `development`) but is not the primary target — do not
rely on it.

**Canonical image:** `nrp-local/nrp-nest-gazebo-ubuntu20:local`, declared in
[.devcontainer/devcontainer.json](.devcontainer/devcontainer.json) and built by
the `nrp-nest-gazebo` service in [docker-compose.yaml](docker-compose.yaml).

The corresponding cmake preset is
[.ci/cmake_cache/nest-gazebo.cmake](.ci/cmake_cache/nest-gazebo.cmake):

```
ENABLE_GAZEBO=ON  BUILD_GAZEBO_ENGINE_SERVER=ON
ENABLE_NEST=ON    BUILD_NEST_ENGINE_SERVER=ON
ENABLE_ROS=ON     ENABLE_MQTT=ON
COVERAGE=ON
```

This is the config every reviewer will use. Fixes should keep it green.

## Building

### Building the container (one-time, slow)

From the host, with Docker + `docker compose` v2+:

```bash
cp .env.template .env
# edit .env so that NRP_DOCKER_REGISTRY=nrp-local and NRP_CORE_TAG=local
./build_nrp_core_image.sh nrp-nest-gazebo
```

The helper script walks the `depends_on:` chain in `docker-compose.yaml` and
builds `base-env → gazebo-env → nest-gazebo-env → nrp-nest-gazebo-env →
nrp-nest-gazebo` in order. Expect a multi-hour initial build: it fetches Gazebo
11, compiles NEST 3.1, installs ROS Noetic, and compiles the whole of nrp-core
inside the final stage.

Other targets worth knowing:

| Target | When to use |
|---|---|
| `nrp-nest-gazebo` | Canonical — matches devcontainer |
| `nrp-vanilla` | Sanity check (no gazebo server, no MQTT, no ROS) |
| `nrp-opensim` / `nrp-tvb-opensim` | OpenSim + TVB only |
| `nrp-nvidia-cuda` | EDLUT GPU path |
| `nrp-xpra-nest-gazebo` | Same as canonical + xpra for remote X |

### Rebuilding nrp-core inside the container

The dev workflow is: mount the source tree into the running container (that's
what the VSCode devcontainer does) and rebuild in place.

```bash
# inside the container, at /workspace or wherever the repo is mounted
export CMAKE_CACHE_FILE=.ci/cmake_cache/nest-gazebo.cmake
bash .ci/11-prepare-build.sh      # cmake configure into ./build
bash .ci/20-build.sh              # make + make install, -j auto-sized
```

`.ci/20-build.sh` deliberately caps parallelism at `max(nproc * 0.7, min(nproc, 4))`
— don't replace it with `-j$(nproc)` on shared CI hosts, you'll OOM the box.

## Running the tests

### The one-command local path (preferred)

From the **host**, at the repo root:

```bash
bash .ci/00-dev-rebuild-and-test.sh
```

This is the script to run before claiming a change is done. It:

1. Ensures the canonical image `nrp-local/nrp-nest-gazebo-ubuntu20:local`
   exists (builds it if missing; `--rebuild-image` to force).
2. Runs `.ci/11-prepare-build.sh` + `.ci/20-build.sh` inside the container
   against the mounted source tree.
3. Runs `ctest` directly (bypassing `.ci/30-run-tests.sh`'s Jenkins `0x8`
   exit-mask) so a failing test fails the script.

Useful flags:

- `--keep-build` — reuse the existing `build/` directory (fast iteration).
- `--test-filter REGEX` — forward `-R REGEX` to ctest for a subset.
- `--no-image` — skip the docker-image existence check.
- `--rebuild-image` — force-rebuild the image first.

Copilot / Claude note: this is the command to cite in PR descriptions and
the command to run after every commit on a branch. Do *not* claim a change
passes if this script did not exit 0.

### Inside-container primitives

`.ci/00-dev-rebuild-and-test.sh` wraps these, but you can still call them by
hand when you are already shelled into the container (e.g. the VSCode
devcontainer):

```bash
bash .ci/30-run-tests.sh
```

What `.ci/30-run-tests.sh` does:

1. `cd build`, source `~/.bashrc` (sets `NRP_INSTALL_DIR`, `PYTHONPATH`, sources
   ROS Noetic).
2. If no `$DISPLAY`, runs `ctest` under `xvfb-run` — many Gazebo tests need an
   X server.
3. Runs `ctest -T Test` and tolerates exit bit `0x8` (failed tests) so Jenkins
   still collects results. **Do not copy this mask into a local pass/fail
   check — treat any non-zero ctest exit as a failure.** For a failure-strict
   local run, use `.ci/00-dev-rebuild-and-test.sh` instead.
4. Generates coverage with `make gcovr`.

For a faster local loop, run individual tests from `build/`:

```bash
cd build
ctest -R FunctionManager --output-on-failure
# or run the gtest binary directly:
./src/nrp_general_library/tests/FunctionManager --gtest_filter='*name*'
```

Python-side tests are registered with `add_test(... py.test ...)` and live next
to the C++ tests; `ctest -V -R EventLoopPython` runs them with the same env.

## If tests fail

This is the spot where most regressions sneak in. The required response is:

1. **Do not commit.** Do not open a PR. Do not say "the fix works" unless
   `ctest` finishes with exit 0.
2. Re-run the specific failing test with `--output-on-failure` (or the binary
   directly) so you can read the gtest assertion, not just the ctest summary.
3. If the failure is environmental (missing `roscore`, MQTT broker, `/dev/dri`,
   `DISPLAY`) — fix the environment, don't silence the test. Look at
   `src/nrp_scripts/nrp-core-entrypoint.bash` and `xvfb-run-nrp` for the
   canonical setup.
4. If the failure is real code, fix the code. Skipping or `DISABLED_`-prefixing
   a test requires an explicit "why" comment referencing an issue or commit.
5. Re-run `.ci/30-run-tests.sh` in full (not just the previously-failing
   subset) before claiming done — changes in core libraries cascade.

Tests that depend on external services and are cmake-guarded:

| Guard | Test examples |
|---|---|
| `BUILD_GAZEBO_ENGINE_SERVER` | `src/nrp_gazebo_engines/.../tests/test_gazebo_*.cpp` |
| `BUILD_NEST_ENGINE_SERVER` | `src/nrp_nest_engines/.../tests/test_nest_*.cpp` |
| `ENABLE_ROS` | `src/nrp_event_loop/tests/test_python_ros_nodes.cpp` (forks `roscore`) |
| `ENABLE_MQTT` | `src/nrp_event_loop/tests/test_event_loop_engine.*` (uses an MQTT mock, but the proxy code path still needs the libs linked) |

If a previously-failing test goes green by accident after an unrelated change,
that's a flake signal — note it and dig.

## Coding conventions specific to this repo

- **C++17, `-Wall -Wextra -Werror -Wno-maybe-uninitialized`** — set in
  [CMakeLists.txt](CMakeLists.txt). A warning in your patch will break the
  build. Do not add `#pragma` silencers without justification.
- Every compilation unit participates in `COVERAGE=ON` (gcov flags are appended
  globally). Keep the build type Release for CI; Debug is fine locally.
- Feature flags live in the top-level [CMakeLists.txt](CMakeLists.txt) as
  `option(...)` / `cmake_dependent_option(...)`. Any new simulator integration
  must be behind such a flag so the vanilla build stays small.
- Proto files go in the `src/nrp-core-msgs` submodule, not the main tree.
- Python modules are installed to `${NRP_INSTALL_DIR}/lib/python3.8/site-packages`
  — hardcoding `python3.8` is intentional (Ubuntu 20.04 system Python).
- Submodules must be initialized before cmake:
  `git submodule update --init --recursive`. The cmake configure will fail fast
  with a clear message if `src/nrp-core-msgs/protobuf` is missing.

## Working through the VSCode devcontainer

[.devcontainer/devcontainer.json](.devcontainer/devcontainer.json) pins the
image and sets `CMAKE_CACHE_FILE=.ci/cmake_cache/nest-gazebo.cmake`. After
"Reopen in Container":

1. `bash .ci/11-prepare-build.sh && bash .ci/20-build.sh`
2. `bash .ci/30-run-tests.sh`

The `onCreateCommand` just installs `gdb`.

## What NOT to do

- Don't change the canonical image (`nrp-nest-gazebo-ubuntu20:local`) without
  updating `.devcontainer/devcontainer.json`, `docker-compose.yaml`, and
  `.ci/cmake_cache/nest-gazebo.cmake` in the same commit — reviewers rely on
  all three lining up.
- Don't introduce Ubuntu-22-only syntax (python 3.10+, glibc-2.35-only APIs)
  unless you've also built it on 20.04.
- Don't bump `NEST`, `Gazebo`, or `grpc++` versions casually — the CMake files
  pin them deliberately (`v3.1`, `gazebo11`, `libgrpc++-dev` from focal). These
  are coupled to the Dockerfiles and to the NEST Python bindings path.
- Don't delete the ctest `0x8` mask in `.ci/30-run-tests.sh` — it exists so
  Jenkins can parse the xunit output even on failing builds. Your *local*
  pass/fail check must still treat failures as failures.
- Don't add GitHub Actions workflows silently; CI today runs on Jenkins
  (`Jenkinsfile`, `.ci/Jenkinsfile`). If you add GH Actions, mirror the Jenkins
  matrix or say explicitly which images are not covered.
- Don't commit a `.env` file. It's intentionally out of `.dockerignore`-only
  scope but should stay local. `.env.template` is the source of truth.

## Triage hints for the common early failures

- **`make` fails with "submodule nrp-core-msgs does not exist"** →
  `git submodule update --init --recursive`.
- **Gazebo unit tests hang in a fresh container** → `DISPLAY` not set; run
  under `xvfb-run-nrp` or source the provided `.ci/30-run-tests.sh` wrapper.
- **NEST-Python tests can't import `nest`** → `PYTHONPATH` missing
  `$NRP_DEPS_INSTALL_DIR/lib/python3.8/site-packages`; re-source
  `$NRP_INSTALL_DIR/bin/.nrp_env`.
- **ROS tests fail immediately** → `/opt/ros/noetic/setup.bash` not sourced.
  The canonical image's `~/.bashrc` handles this; if you shelled in with
  `docker exec --no-tty`, re-source it.
- **MQTT link errors on rebuild** → Paho C/C++ libs were built into
  `$NRP_DEPS_INSTALL_DIR`; make sure `LD_LIBRARY_PATH` includes it.

## Who/what this file serves

This file is loaded into every Claude Code conversation that starts in this
repo. Keep the total under ~400 lines so it doesn't crowd the context window.
Anything longer belongs in `docs/` (Doxygen-consumed) and should be linked from
here, not inlined.
