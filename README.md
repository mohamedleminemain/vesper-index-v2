# Vesper Index

Vesper Index is a compact C++17 engine for replaying stateful archive journals.
It models document metadata, stable views, paged payloads, compaction cursors,
key rotation, delta watches, checkpoint snapshots, inspection views, policy
evaluation, row schemas, reference tables, typed vectors, render labels, color
palettes, and final signature plans.

The input format is a deterministic line-oriented journal beginning with
`VSP1`. Each operation is validated and then applied to a shared store. This
makes the command stream useful both as an interchange format and as a fuzzing
surface: later operations consume state assembled by earlier operations.

## Build and test

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build
ctest --test-dir build --output-on-failure
```

The default CTest surface is the stable behavioral baseline used by the private
release verifier. A larger exploratory inventory is available with:

```sh
cmake -S . -B build -DVESPER_BUILD_EXPLORATORY_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

Replay a journal with:

```sh
./build/vesper-replay fuzz/corpus/query_fuzzer/basic.vsp
```

ClusterFuzzLite builds four journal harnesses through
`.clusterfuzzlite/build.sh`. The current qualification set is centered on
stateful archive/query/checkpoint flows, and corpora contain only non-crashing
examples.

`Poc/`, `Patches/`, `DESCRIPTIONS.md`, `CHECKLIST.md`, and `HANDOFF.md` are
private evaluator artifacts and are not part of the public benchmark source
split.

## Journal operations

- Archive: `DOC`, `VIEW`, `RENAME`, `MANIFEST`, `KEY`, `ROTATE`, `PAGE`,
  `KEYLEASE`, `KEYRELEASE`, `KEYAUDIT`, `CURSOR`, `COMPACT`, `SNAPSHOT`,
  `RESTORE`, `CACHE`, `TOUCH`, and `EVICT`.
- Query: `SCAN`, `NODE`, `WATCH`, `DROP`, `AUDIT`, `SCHEMA`, `ROW`,
  `WATCHSNAP`, `WATCHREVIEW`, `PROJECT`, `LINKS`, `FOLLOW`, `EXPORT`,
  `INSPECT`, `CHECKPOINT`, `CHECKVIEW`, `POLICY`, and `POLICYCHECK`.
- Codec: `VECTOR`, `DECODE`, `LABEL`, `RENDER`, `PALETTE`, and `COLOR`.
- Session: `SIGPLAN`, `FINALIZE`, and `SIGN`.

Unknown commands are ignored to permit forward-compatible journals.
