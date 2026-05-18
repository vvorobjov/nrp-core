# CLAUDE.md

Operating instructions for Claude Code (and any Claude-family assistant) working
in this repository. Keep this file short, correct, and machine-actionable — it
is loaded into every conversation.

## The two rules that always win

1. **No change lands unless the full unit-test suite AND the
   docker-compose example pass on the canonical Ubuntu 22.04 target**.
   The acceptance gate has two parts. First, one ctest run:

   ```bash
   bash .ci/00-dev-rebuild-and-test.sh              # ubuntu22 / Humble / py3.10
   ```

   Second, the husky_braitenberg docker-compose example must run end-
   to-end without errors in both supported display modes:

   ```bash
   bash examples/run_docker-compose_example.sh           # xvfb (headless)
   bash examples/run_docker-compose_example.sh --xpra    # xpra (browser-accessible)
   ```

   All three commands must exit 0 before a commit, merge, or review
   can be claimed done. The ctest run covers unit-test correctness;
   the compose runs catch integration regressions (entrypoint scripts,
   PYTHONPATH, gazebo plugins, nrp-core ↔ nest-server wire
   compatibility, MQTT proxy) that single-process unit tests miss.
   The compose example exits cleanly when SimulationTimeout is reached
   (success); container errors or non-zero exits are real failures.

   If you cannot run any of the three checks (e.g. missing image
   build artifact, no Docker access), say so explicitly — do not
   claim the task is done.

   The three commands above are the **minimum** gate — they catch
   build/wire-format/integration regressions cheaply. Above that
   sits the **ultimate-test gate**, the gold standard before
   shipping anything load-bearing:

   ```bash
   bash .ci/01-dev-run-experiments.sh                   # 20 NRPCoreSim experiments
   ```

   It walks every `examples/<subset>/simulation_config*.json` listed
   in the script (event_loop_examples, status_function_test,
   husky_braitenberg_multi_robot, nrp_vectorization, baseball_icub,
   generic_proto_test, nest_simple), launches each one inside the
   canonical jammy container, and reports PASS/FAIL/SKIP. Three
   SpiNNaker variants and one external-OpenSim variant are
   documented skips (counted, not silently dropped). Total
   wall-clock is on the order of 15 minutes. Run this whenever
   you change anything that the husky_braitenberg compose example
   doesn't exercise — Event Loop wiring, status functions,
   docker-launcher engines, the gRPC + protobuf path,
   multi-robot/iCub Gazebo plumbing. Tracked in EBR2-83.

   EBR2-81 dropped the parallel Ubuntu 20.04 (focal / Foxy / Py 3.8)
   chain. Anything in the tree that still says `ubuntu20`, `focal`,
   `foxy`, `python3.8`, or `--ubuntu22` is a leftover bug; fix it
   under a follow-up ticket.

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
  describing the problem and acceptance criteria. If the Atlassian MCP is
  loaded in this session (you'll see `mcp__*Atlassian*` tools available),
  create the issue directly via that MCP. Otherwise, hand the user a
  ready-to-paste claude.ai prompt instead — see the section below.
- **Branch name:** `EBR2-<num>-<short-slug>`, lower-kebab-case slug, ≤ 60
  chars total, branch from development always.
  Examples: `EBR2-234-fix-entrypoint-newline`,
  `EBR2-238-bump-paho-mqtt-1-4-1`.
- **Commit subject:** `[EBR2-<num>] <imperative summary>`. The `<num>` is the
  same as the branch's. Multiple commits on one branch are fine — all share
  the same key.
- **Atomic commits.** One logical change per commit. If you've done three
  things on the branch — a cmake fix, a script rewrite, and a Dockerfile
  patch — that's three commits, not one. The subject must describe
  *exactly* what landed in the commit, no broader. Stage selectively
  (`git add <path>` per file, or `git add -p` for partial hunks); never
  blanket-stage with `git add -A` / `git add .` just to flush the working
  tree. If a "bug fix" requires unrelated cleanup, the cleanup goes in
  its own commit (its own ticket if it's not trivial). Bisect, revert,
  and review all rely on this.
- **Commit body:** short but substantive — a few tight lines that name the
  symptom, the root cause, and the fix. Skip filler ("this commit", "in
  order to"); skip prose retellings of the diff. If the *why* fits in the
  subject, the body can be empty.
- **No AI co-author trailers.** Do not append `Co-Authored-By: Claude …`,
  `… Copilot …`, `… Codex …`, or any other AI/assistant identity to commit
  messages or PR bodies. The human author is the sole author of record.
- **PR title:** `[EBR2-<num>] <summary>` and the PR description must link
  back to the issue URL.
- **Exception — placeholder keys.** You'll see `[EBR2-0000]` /
  `[EBR2-000]` in older commits. Don't use them for new work. If you find
  yourself tempted, open an issue first.
- **Merging:** do not squash commits that already carry the correct ticket
  key — the prefix is the audit trail.
- **After merge — move the ticket to Done.** As soon as the PR for an
  `EBR2-<num>` branch is merged into `development` (or `master`),
  transition that Jira issue to **Done**. Don't leave it In Progress / In
  Review — the board is the single source of truth for what's shipped.
  If the Atlassian MCP is available, call `transitionJiraIssue` with the
  "Done" transition directly (look it up via `getTransitionsForJiraIssue`
  if you don't already know the ID). If the MCP is not available, tell
  the user one line: *"PR merged — please move EBR2-NNN to Done on the
  board."* Apply the same rule when handling a batch of merges: every
  ticket whose branch landed gets moved.

If the issue key doesn't exist yet, stop and create it. A PR without a real
ticket will be bounced in review.

### Before starting work — create or request the Jira ticket

This rule applies to every fix, refactor, doc-only update, and dependency
bump. The only exception is work explicitly scoped to an already-created
ticket (the user will tell you the key up front).

**First, check whether the Atlassian MCP is available in this session.**
Look at the tool list for names matching `mcp__*Atlassian*` (e.g.
`mcp__claude_ai_Atlassian__createJiraIssue`). Pick a path based on what
you see:

**Path A — Atlassian MCP is available**

1. **As soon as** you identify a new problem or improvement, compose the
   issue text (one-line summary, 1–3 sentence context, concrete acceptance
   criteria) and call the MCP's create-issue tool directly against project
   `EBR2`. Don't ask first — high-impact destructive operations need
   confirmation, but creating a tracker ticket is the standard precondition
   for any work and is the user's intent.
2. Tell the user one line: *"Opened EBR2-NNN — `<URL>`."*
3. Proceed immediately: create the `EBR2-NNN-<slug>` branch, use
   `[EBR2-NNN] …` in every commit subject, and reference the issue URL in
   the PR body.

**Path B — Atlassian MCP is NOT available**

Claude Code cannot create Jira issues directly without the MCP. Claude.ai
can. To keep the user's effort to a single copy-paste:

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

| Version | ROS 2 | Python | Status |
|---|---|---|---|
| Ubuntu 22.04 (jammy) | Humble | 3.10 | **Primary / canonical** |

Core deps: Gazebo 11 Classic (apt: `gazebo`, `gazebo-plugin-base`,
`libgazebo-dev`), NEST 3.1, Boost.Python 1.74, gRPC from apt. The
EBR2-26 work replaced ROS 1 Noetic with ROS 2 (rclcpp / ament_cmake /
colcon); EBR2-81 dropped the parallel Ubuntu 20.04 (focal / Foxy / Py
3.8) chain so there is now exactly one supported target.

**Canonical image:** `nrp-local/nrp-nest-gazebo:local`, declared in
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
nrp-nest-gazebo` in order. Expect a multi-hour initial build: it installs
Gazebo 11 + ROS 2 Humble from apt, compiles NEST 3.1, and compiles the
whole of nrp-core inside the final stage.

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

From the **host**, at the repo root, run the dev-helper. It must exit 0:

```bash
bash .ci/00-dev-rebuild-and-test.sh              # ubuntu22 / humble / py3.10
```

The script:

1. Ensures the canonical image `nrp-local/nrp-nest-gazebo:local`
   exists — builds it if missing; `--rebuild-image` to force.
2. Runs `.ci/11-prepare-build.sh` + `.ci/20-build.sh` inside the container
   against the mounted source tree.
3. Runs `ctest` directly (bypassing `.ci/30-run-tests.sh`'s Jenkins `0x8`
   exit-mask) so a failing test fails the script.

Useful flags:

- `--keep-build` — reuse the existing `build/` directory (fast iteration).
- `--test-filter REGEX` — forward `-R REGEX` to ctest for a subset.
- `--no-image` — skip the docker-image existence check.
- `--rebuild-image` — force-rebuild the image first.
- `--cmake-cache PATH` — override the cmake initial-cache preset (in-container
  path, e.g. `/workspace/.ci/cmake_cache/vanilla.cmake`). Defaults to the
  canonical `nest-gazebo.cmake` so the suite exercises MQTT / NEST / Gazebo
  / ROS. Pre-EBR2-71 the script silently fell back to `vanilla.cmake` and
  skipped those tests; the default preset is load-bearing.

The script also passes `--timeout 120` to `ctest` (EBR2-78), capping each
test at 2 min. The longest legitimately-passing test in the canonical preset
finishes in ~42 s, so 120 s leaves comfortable headroom; the default ctest
1500 s timeout burns half an hour per hung test. Override the cap for an
individual test in cmake via
`set_tests_properties(<name> PROPERTIES TIMEOUT <seconds>)`.

### Inside-container primitives

`.ci/00-dev-rebuild-and-test.sh` wraps these, but you can still call them by
hand when you are already shelled into the container (e.g. the VSCode
devcontainer):

```bash
bash .ci/30-run-tests.sh
```

What `.ci/30-run-tests.sh` does:

1. `cd build`, source `~/.bashrc` (sets `NRP_INSTALL_DIR`, `PYTHONPATH`, sources
   ROS 2 Humble).
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
- Python modules install to `${NRP_INSTALL_DIR}/lib/python<X.Y>/site-packages`
  where X.Y is detected by cmake's `find_package(Python3)` (3.10 on jammy).
  Don't hardcode `python3.10` even though jammy is now the only target —
  use `${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}` in cmake and
  `python3 -c "import sys; ..."` in shell scripts.
- The ROS 2 distribution is `humble` (image-building ARG `ROS_DISTRO`). In
  shell scripts that may be sourced before the env is set up, source
  `/opt/ros/humble/setup.bash` directly.
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

- Don't change the canonical image (`nrp-nest-gazebo:local`) without
  updating `.devcontainer/devcontainer.json`, `docker-compose.yaml`, and
  `.ci/cmake_cache/nest-gazebo.cmake` in the same commit — reviewers rely on
  all three lining up.
- Jammy (Ubuntu 22.04) is the only supported target. EBR2-81 dropped the
  parallel focal chain; do not reintroduce focal/Foxy/python 3.8 branches,
  `-ubuntu20` or `-ubuntu22` service suffixes, or `--ubuntu22` flags in
  helper scripts.
- Don't bump `NEST`, `Gazebo`, or `grpc++` versions casually — the CMake
  files pin them deliberately (`v3.1`, Gazebo 11 from jammy apt,
  `libgrpc++-dev` from jammy). These are coupled to the Dockerfiles and to
  the NEST Python bindings path. NEST 3.9 migration is tracked separately
  in EBR2-82.
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
  `$NRP_DEPS_INSTALL_DIR/lib/python3.10/site-packages`; re-source
  `$NRP_INSTALL_DIR/bin/.nrp_env`.
- **ROS tests fail immediately** → `/opt/ros/humble/setup.bash` not
  sourced. The canonical image's `~/.bashrc` sources it; if you shelled
  in with `docker exec --no-tty`, re-source it.
- **MQTT link errors on rebuild** → Paho C/C++ libs were built into
  `$NRP_DEPS_INSTALL_DIR`; make sure `LD_LIBRARY_PATH` includes it.

## Who/what this file serves

This file is loaded into every Claude Code conversation that starts in this
repo. Keep the total under ~400 lines so it doesn't crowd the context window.
Anything longer belongs in `docs/` (Doxygen-consumed) and should be linked from
here, not inlined.
