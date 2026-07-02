# Vesper Index

Vesper Index is a compact C++17 library and command-line toolkit for replaying,
auditing, and reporting on stateful archive journals. It is aimed at offline
content archive workflows where a journal stream is used to build document
metadata, page indexes, checkpoints, audit views, policy reports, and derived
catalog artifacts.

The engine models document metadata, stable views, paged payloads, compaction
cursors, key rotation, delta watches, checkpoint snapshots, inspection views,
policy evaluation, row schemas, reference tables, typed vectors, render labels,
color palettes, and final signature plans.

The input format is a deterministic line-oriented journal beginning with
`VSP1`. Each operation is validated and then applied to a shared store. This
makes the command stream useful as an interchange and replay format for archive
processing pipelines: later operations consume state assembled by earlier
operations, so a single journal can reproduce a full indexing or audit run.

## Typical uses

- replay a metadata journal and build a stable manifest digest
- checkpoint a long-running archive import and inspect the resulting state
- inspect cache, relation, quota, policy, digest, and timeline reports from a
  saved journal
- normalize document tokens and derive compact summary views for tooling
- run offline regression and robustness tests for journal compatibility

## Build and test

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build
ctest --test-dir build --output-on-failure
```

The default CTest surface is a stable behavioral baseline. A larger
exploratory inventory is available with:

```sh
cmake -S . -B build -DVESPER_BUILD_EXPLORATORY_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

Replay a journal with:

```sh
./build/vesper-replay fuzz/corpus/query_fuzzer/basic.vsp
```

Useful companion tools include:

- `vesper-inspect` for materialized store views
- `vesper-journal-stats` for journal sizing and command counts
- `vesper-checkpoint-report` for checkpoint inventories
- `vesper-ledger-report` and `vesper-quota-report` for summarized accounting
- `vesper-key-watch-report` for cross-checking key and watch state

The repository also includes optional libFuzzer harnesses under `fuzz/` for
robustness testing of the journal parser and replay engine. The seed corpora
are non-crashing examples of normal journal traffic.

## Example workflows

Replay and inspect a catalog journal:

```sh
./build/vesper-replay path/to/archive.vsp
./build/vesper-inspect path/to/archive.vsp
```

Generate a checkpoint-oriented report:

```sh
./build/vesper-checkpoint-report path/to/archive.vsp
./build/vesper-state-digest-report path/to/archive.vsp
```

Review key rotation and watch activity:

```sh
./build/vesper-key-watch-report path/to/archive.vsp
./build/vesper-watch-review-dump path/to/archive.vsp
```

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
