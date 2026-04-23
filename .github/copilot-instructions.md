# GitHub Copilot instructions for nrp-core

This file tells Copilot (chat, code completions, and agent mode) how to behave
in this repository. A longer, sibling file for other Claude-family assistants
lives at [../CLAUDE.md](../CLAUDE.md) — everything there applies here too.

## The acceptance gates (non-negotiable)

### Gate 1 — tests must pass

**No code change is accepted unless the full unit-test suite passes inside the
canonical container.** The canonical container is
`nrp-local/nrp-nest-gazebo-ubuntu20:local`, configured with the
[.ci/cmake_cache/nest-gazebo.cmake](../.ci/cmake_cache/nest-gazebo.cmake)
preset (Gazebo + NEST + ROS + MQTT, Ubuntu 20.04).

The canonical local verification command is one host-side script:

```bash
# from the host, at the repo root — ensures image, then runs
# configure + build + install + ctest inside the canonical container.
bash .ci/00-dev-rebuild-and-test.sh
```

This is the command Copilot should cite in PR descriptions and the command
to run after every commit on a branch. It bypasses the Jenkins-specific
`0x8` exit-mask that `.ci/30-run-tests.sh` applies, so any failing test
fails the script. Handy flags: `--keep-build` (reuse `build/`),
`--test-filter REGEX` (run a subset), `--rebuild-image` (force image
rebuild), `--no-image` (skip the image check).

If you are already shelled into the canonical container (e.g. VSCode
devcontainer), the lower-level primitives remain callable by hand:

```bash
# inside the canonical container, at the repo root
bash .ci/11-prepare-build.sh     # cmake configure
bash .ci/20-build.sh             # compile + install
bash .ci/30-run-tests.sh         # ctest — CI-focused; tolerates exit bit 0x8
```

If you cannot run these — because the environment is missing, the container
isn't built, or something else — say so explicitly in your PR description
and do **not** mark the change as ready. Suggest a reviewer run them on
your behalf.

### Gate 2 — every change is tied to an EBR2 Jira ticket

- **Tracker:** [EBR2 board](https://hbpneurorobotics.atlassian.net/jira/software/c/projects/EBR2/boards/11)
  on `hbpneurorobotics.atlassian.net`.
- **Create or claim an issue before writing code.** The issue holds the
  acceptance criteria; the PR references the issue.
- **Branch name:** `EBR2-<num>-<short-slug>` (kebab-case, ≤ 60 chars total).
- **Commit subject:** `[EBR2-<num>] <imperative summary>` — same key as the
  branch. If a change spans multiple commits, all of them carry the same key.
- **PR title:** `[EBR2-<num>] <summary>`; body links the issue URL.
- Don't use `EBR2-0000` / `EBR2-000` placeholders that appear in older
  commits — they predate this rule and are not acceptable for new work.

Copilot: when suggesting a commit message, default to the `[EBR2-<num>] …`
prefix and ask for the real `<num>` if it isn't already in the branch name
or PR context. Never invent an issue key.

#### Before starting work — hand the user a ready-to-paste claude.ai prompt

Copilot (and any Claude agent) cannot create Jira issues from the IDE. The
user expects you to eliminate the typing for them:

1. **As soon as** you identify a new fix or improvement — before reading
   more code, before planning, before editing — draft the issue text
   (summary, 1–3 sentence context, acceptance criteria) and present it to
   the user in the exact format from [../docs/jira_prompt.md](../docs/jira_prompt.md)
   with every `<bracketed>` field already filled in.
2. Do this unprompted. Do not ask "should I draft the prompt?" — draft it.
   Output it as a single fenced code block labelled `text` so the user can
   paste into claude.ai with no edits. Precede it with one sentence:
   *"Paste this into claude.ai to open the ticket; reply with the issue key."*
3. **Stop and wait** for the user to reply with the issue key. Do not
   create a branch, stage changes, or commit until the key is known.
4. On reply: create branch `EBR2-<num>-<slug>`, use `[EBR2-<num>] …` in
   every commit subject, link the issue URL in the PR body.
5. For small, grouped fixes use the short prompt variant; never silently
   split one change across two tickets — ask the user first.

This applies to every fix, refactor, doc-only update, and dependency bump.
Exception: work against an already-existing ticket the user names up front.

## Project context Copilot needs

- **Language mix:** C++17 (most of the codebase), Python 3.8 (engines, scripts,
  pytest tests), CMake. Boost.Python is the C++↔Python bridge.
- **Target OS:** Ubuntu 20.04. Ubuntu 22.04 is an unofficial secondary target;
  do not assume a newer glibc, Python 3.10+, or gcc ≥ 10 behaviors.
- **Canonical image:** see
  [.devcontainer/devcontainer.json](../.devcontainer/devcontainer.json).
- **CI:** Jenkins only, orchestrated via the root
  [Jenkinsfile](../Jenkinsfile) and [.ci/Jenkinsfile](../.ci/Jenkinsfile).
  There are no GitHub Actions workflows today — don't invent one in a drive-by
  edit.
- **Build system:** CMake with feature flags (`ENABLE_GAZEBO`, `ENABLE_NEST`,
  `ENABLE_ROS`, `ENABLE_MQTT`, `ENABLE_OPENSIM`, `ENABLE_TVB`,
  `ENABLE_SPINNAKER`, `ENABLE_EDLUT`). Every new simulator integration must
  live behind such a flag.
- **Required hard flags:** `-Wall -Wextra -Werror -Wno-maybe-uninitialized`.
  A warning breaks the build. Don't silence with `#pragma` unless you explain
  why in the commit message.

## How Copilot should respond to common asks

### "Add a new Engine / node / datapack"

1. Scaffold under the appropriate `src/nrp_*_engines/` directory.
2. Register it in the parent `CMakeLists.txt` behind a new `ENABLE_*` option
   if it pulls in new third-party libs.
3. Add a `tests/` directory alongside and wire it into `gtest_discover_tests`.
4. Add a matching plugin-registration line so `NRPCoreSim` can load it at
   runtime (see [src/nrp_general_library/plugin_system/](../src/nrp_general_library/plugin_system/)).
5. Regenerate/check protobuf stubs — proto lives in the
   [src/nrp-core-msgs/](../src/nrp-core-msgs/) submodule.

### "Fix bug X"

1. Reproduce with the **smallest possible ctest filter** first.
2. Add a regression test *before* changing production code if the bug is
   observable in a unit test.
3. Re-run the full `bash .ci/30-run-tests.sh` suite — single-test runs are
   necessary but not sufficient.

### "Disable this flaky test"

Default answer: don't. If the test is truly flaky, prepend `DISABLED_` in the
gtest name and leave a comment citing an issue number. Removing the test or
the `add_test(...)` line requires approval from a maintainer.

### "Bump dependency Y"

Do not casually change versions pinned in:

- [dockerfiles/nest.Dockerfile](../dockerfiles/nest.Dockerfile) — `nest v3.1`
- [dockerfiles/gazebo.Dockerfile](../dockerfiles/gazebo.Dockerfile) —
  `gazebo11`, `ros-noetic-*`
- [dockerfiles/nrp-core.Dockerfile](../dockerfiles/nrp-core.Dockerfile) —
  `paho.mqtt.cpp v1.4.0`
- [.ci/dependencies/apt/requirements.*.txt](../.ci/dependencies/apt/)

Those versions are coupled to the Ubuntu 20.04 package set and to the Python
3.8 ABI. Bumping them requires rebuilding every image in the compose graph
and rerunning the full test matrix.

### "Add a GitHub Action / change the Jenkinsfile"

CI today is Jenkins-only. If you are asked to add GitHub Actions, propose a
workflow that:

1. Runs on `pull_request` and `push` to `master` / `development`.
2. Uses the canonical `nrp-nest-gazebo-ubuntu20` image (pulled from a
   registry — do not rebuild it per-run).
3. Invokes exactly the three shell scripts above, mirroring the
   `Jenkinsfile` matrix.
4. Fails the check on any non-zero ctest exit (do **not** mask bit `0x8`
   locally, that mask is a Jenkins reporting workaround only).

## Style

- Follow the existing style: 4-space indent in C++ and Python, snake_case for
  C++ methods/functions, PascalCase for classes, `m_`-prefixed private
  members. Headers end in `.h`, not `.hpp`.
- Prefer `std::filesystem` over Boost filesystem for new code. Boost is
  already a dependency (Boost.Python), so using it is fine if the
  surrounding file already does.
- JSON handling uses `nlohmann::json`. YAML is not used in production code;
  Python tools can use PyYAML, but avoid introducing runtime YAML in C++.
- Comments: say *why*, not *what*. Do not write docstrings that repeat the
  signature.

## Don't do these

- Don't generate code that hardcodes paths outside `$NRP_INSTALL_DIR`,
  `$NRP_DEPS_INSTALL_DIR`, or the working directory.
- Don't assume network access from the tests (only a subset of tests with
  external service deps expect it — everything else is mocked).
- Don't add `// TODO: fix later` without a ticket reference — open an issue
  first.
- Don't commit generated files (`build/`, `.pytest_cache/`,
  `generatedJUnitFiles/`). They're already in `.dockerignore` and
  `.gitignore`; don't remove those entries.
- Don't rename or delete the `src/nrp-core-msgs` submodule.

## PR / commit checklist

Before Copilot's suggested change is ready for review, confirm:

- [ ] `git submodule status` shows `nrp-core-msgs` at the expected commit.
- [ ] `bash .ci/00-dev-rebuild-and-test.sh` exits 0 (or, if running the
      lower-level scripts by hand inside the container:
      `11-prepare-build.sh` + `20-build.sh` + `30-run-tests.sh` all green).
- [ ] Changelog entry added under the current unreleased section of
      [../Changelog.md](../Changelog.md).
- [ ] If a new feature: the relevant `ENABLE_*` flag + doxygen page exist.
- [ ] If a Dockerfile or .ci/ file changed: the corresponding image was
      rebuilt and the test suite re-run inside it.

If any of those is un-checked, the PR description must say so and ask the
reviewer for help.
