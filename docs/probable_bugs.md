# Probable bugs — catalog for follow-up tickets

This file is the working notebook for suspected bugs that surfaced while
expanding unit-test coverage under [EBR2-19].

**Scope rule:** this file does NOT fix bugs. Each entry is a seed for a
separate EBR2 ticket. When a bug here gets its own ticket and is fixed, move
the entry to the "Resolved" section at the bottom (or delete it), and flip
the checkbox for "ticket opened".

## Entry template

Copy this block when adding a new entry. Keep each entry terse — code
details live in the test that pins current behaviour, not here.

```markdown
### PB-N. <one-line summary>

- **File:** `path/to/file.h:LLL`
- **Symptom:** <what's observably wrong>
- **Suspected cause:** <one sentence — hypothesis only, do not debug here>
- **Reproduction:** <the test name that pins current behaviour, if any>
- **Severity guess:** BLOCKER | IMPORTANT | NICE-TO-HAVE
- [ ] EBR2 ticket opened (key: )
- [ ] Fix merged
```

## Guidelines for writing tests around a probable bug

When a test reveals a behaviour you believe is wrong:

1. Do NOT change production code under the current (coverage-expansion)
   ticket. The rule is: cover, don't fix.
2. Write the test so that it asserts the **current** behaviour, not the
   expected-correct behaviour. Otherwise the test fails today for a reason
   unrelated to its purpose (and CI goes red, which stops the branch from
   progressing).
3. Above the gtest body, leave a comment of the form:
   `// BUG suspected — see docs/probable_bugs.md#pb-N.`
4. Add (or update) the corresponding entry in this file.
5. When the bug gets its own ticket, extend the comment to
   `// BUG suspected — EBR2-<key>; see docs/probable_bugs.md#pb-N.` and
   check the "ticket opened" box here.

## Active entries

### PB-1. `ZipContainer::createZip` discards the real zip error code

- **File:** `src/nrp_general_library/nrp_general_library/utils/zip_container.cpp:302-310`
- **Symptom:** The error message produced when `zip_source_begin_write` /
  `zip_source_write` / `zip_source_commit_write` fails is always derived
  from a `cErr` value of `0` or `1` (the boolean result of the `||`
  chain), never the actual libzip error code. The user-visible message
  therefore carries no useful information about *why* the zip source
  failed.
- **Suspected cause:** Operator-precedence bug.
  `int cErr = A < 0 || B < 0 || C < 0;` binds the initialiser to the
  boolean expression, not to any of the sub-calls. The intended pattern
  is presumably `if((cErr = A) < 0 || (cErr = B) < 0 || (cErr = C) < 0)`
  where each assignment captures the real return code.
- **Reproduction:** No test added. Triggering the path reliably would
  require forcing libzip to fail on a specific sub-call.
- **Severity guess:** IMPORTANT — data-loss masked by misleading error.
- [ ] EBR2 ticket opened (key: )
- [ ] Fix merged

### PB-2. `ZipContainer(std::string&&)` copies `capacity()` bytes, not `size()`

- **File:** `src/nrp_general_library/nrp_general_library/utils/zip_container.cpp:115-117`
  (+ corresponding header doc string
  `zip_container.h:78-82`)
- **Symptom:** When a `std::string` is moved into the constructor and the
  string's allocated buffer is larger than its logical content, the
  extra bytes past `size()` (uninitialised or leftover) are treated as
  part of the zip payload. libzip will either reject the archive with a
  cryptic error or silently include garbage. The header documents this
  behaviour as intentional ("Will use entire data.capacity() as ZIP file
  array, not just data.size()") which is almost certainly wrong.
- **Suspected cause:** Original author confused `capacity()` with
  `size()` for the Pistache use case, then wrote the behaviour into the
  docstring to "codify" what they saw.
- **Reproduction:** `ZipContainerTest` suite (commit on EBR2-19) uses
  the `std::vector<uint8_t>` constructor specifically to dodge this
  path. An explicit regression test should use a string with
  `reserve(N)` + `resize(M<N)` and confirm behaviour is sane.
- **Severity guess:** IMPORTANT — latent memory-safety / data-corruption.
- [ ] EBR2 ticket opened (key: )
- [ ] Fix merged

### PB-3. `jsonSchemaLoader` slices exceptions on rethrow

- **File:** `src/nrp_general_library/nrp_general_library/utils/json_schema_utils.cpp:56-60`
- **Symptom:** When a malformed schema file is encountered, downstream
  `catch(const nlohmann::json::parse_error&)` blocks no longer fire
  because the exception has been rethrown as a plain `std::exception`
  (type info lost) and by value (copy-constructed, not the original
  object).
- **Suspected cause:** The pattern
  `try { ... } catch (std::exception &e) { throw e; }`
  is a classic slicing bug. The canonical form is a bare `throw;` (no
  variable) which preserves both the type and the original object.
- **Reproduction:** No test added -- exercising this path cleanly needs
  a malformed schema file on disk under `NRP_CONFIG_INSTALL_DIR`, which
  our tests deliberately avoid coupling to.
- **Severity guess:** NICE-TO-HAVE -- cosmetic for logs, but silently
  degrades error-handling fidelity in callers.
- [ ] EBR2 ticket opened (key: )
- [ ] Fix merged

### PB-4. `Port::id()` / `Port::parent()` accessors are non-const

- **File:** `src/nrp_event_loop/nrp_event_loop/computational_graph/port.h:46-47`
  (and matching `parent()` accessor further down)
- **Symptom:** Callers holding a `const Port&` cannot query the port's
  id or parent. This forces spurious non-const references throughout
  the graph-iteration code and blocks straightforward const-correct API
  use in tests.
- **Suspected cause:** The accessor returns a const reference
  (`const std::string&`) but the method itself is not marked `const`.
  Almost certainly an oversight at class-authoring time.
- **Reproduction:** Changing the method signatures to `const` is a
  one-line fix that unlocks const-ref call sites. Current tests pass a
  non-const `Port&` which is why this hasn't been caught.
- **Severity guess:** NICE-TO-HAVE.
- [ ] EBR2 ticket opened (key: )
- [ ] Fix merged

### PB-6. `ZipSourceWrapper` double-frees sources owned by libzip after successful `zip_file_add`

- **File:** `src/nrp_general_library/nrp_general_library/utils/zip_container.cpp:36-48`
  (anonymous-namespace `ZipSourceWrapper`) and the consumer at
  `zip_container.cpp:151-184` (`ZipContainer::compressPath`).
- **Symptom:** Every call to `ZipContainer::compressPath` ending in a
  successful `zip_file_add(...)` ultimately crashes with either a
  glibc `free(): double free detected in tcache 2` or a segfault at
  `zip_close` time. Surfaced when writing
  `ZipContainerTest.CompressAndExtractDirectoryPreservesFileContents`
  under EBR2-19 (three tests crashed; see commit that removed them).
- **Suspected cause:** libzip's ownership contract for
  `zip_file_add(archive, name, source, flags)` is "on success, the
  archive takes ownership of `source`; on failure, the caller retains
  ownership and must free it". `ZipSourceWrapper`'s destructor calls
  `zip_source_free(_zip_source)` unconditionally, so on success path
  the source gets freed twice -- once by libzip when the archive is
  later closed/discarded, once by the wrapper.
- **Reproduction:** Build with EBR2-19's original test_zip_container
  coverage (see commit history) and run the three removed tests:
  `CompressAndExtractDirectoryPreservesFileContents`,
  `SaveToDestinationWritesOpenableArchive`,
  `CompressPathPreservesRelativeStructureWhenRequested`.
- **Likely fix shape:** Make `ZipSourceWrapper` releasable (add a
  `release()` that zeroes `_zip_source`), and call it in the
  consumer right after a successful `zip_file_add`. Same fix pattern
  as the existing `ZipWrapper::release()` already used for the
  archive handle.
- **Impact scope:** Currently zero production callers use
  `compressPath` / `getCompressedData` / `extractZipFiles`; the code
  is dead-but-exported. That is why the bug has gone undetected.
  **Do not delete the feature as a workaround** -- fix it.
- **Severity guess:** BLOCKER (crashes on happy path) **but only if
  any caller actually uses it**. In practice we can treat it as
  IMPORTANT until the first real caller is introduced.
- [ ] EBR2 ticket opened (key: )
- [ ] Fix merged

### PB-5. `ProtoOpsManager::resetInstance()` orphans outstanding references

- **File:** `src/nrp_protobuf/nrp_protobuf/proto_ops/proto_ops_manager.cpp:101-104`
- **Symptom:** Any code that captured the reference returned by a prior
  `getInstance()` retains a dangling pointer after a subsequent
  `resetInstance()` call. The singleton's replacement is silent.
- **Suspected cause:** The API returns `ProtoOpsManager&` (live
  reference), but `resetInstance()` destroys the underlying object via
  `unique_ptr::reset`. Tests that cache the reference across a reset are
  undefined behaviour. Unit tests under EBR2-19 deliberately re-call
  `getInstance()` after every reset to avoid tripping this.
- **Suggested direction:** Either (a) document that callers must not
  hold the reference across a reset, (b) return a `shared_ptr<ProtoOpsManager>`
  whose reset semantics are clearer, or (c) reuse the same storage
  slot and only reset the *state*, not the object itself.
- **Severity guess:** NICE-TO-HAVE -- current callers don't hit this in
  production.
- [ ] EBR2 ticket opened (key: )
- [ ] Fix merged

## Resolved

<!-- Move resolved entries here (or delete). -->
