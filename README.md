# Vesper Index

Vesper Index is a compact C++17 library and command-line toolkit for replaying,
auditing, and reporting on stateful archive journals. It is designed for
offline archive processing workflows where a journal stream records document
ingest, page materialization, checkpoint snapshots, derived views, and audit
reports in a format that can be replayed later.

The engine models document metadata, stable views, paged payloads, compaction
cursors, key rotation, watch snapshots, inspection views, policy evaluation,
row schemas, reference tables, typed vectors, render labels, color palettes,
and final signature plans.

The input format is a deterministic line-oriented journal beginning with
`VSP1`. Each operation is validated and then applied to a shared store. A
single journal can therefore capture an entire archive-processing run and be
replayed later to reproduce manifest state, checkpoint inventories, and audit
outputs.

## Typical uses

- replay a metadata journal and build a stable manifest digest
- checkpoint a long-running archive import and inspect the resulting state
- inspect cache, relation, quota, policy, digest, and timeline reports from a
  saved journal
- normalize document tokens and derive compact summary views for tooling
- run offline regression tests for journal compatibility

## Build and test

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build
ctest --test-dir build --output-on-failure
```

The default CTest surface is a stable behavioral baseline. A larger regression
inventory is available with:

```sh
cmake -S . -B build -DVESPER_BUILD_EXPLORATORY_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

Replay a journal with:

```sh
./build/vesper-replay examples/watch_audit.vsp
```

Useful companion tools include:

- `vesper-inspect` for materialized store views
- `vesper-journal-stats` for journal sizing and command counts
- `vesper-checkpoint-report` for checkpoint inventories
- `vesper-ledger-report` and `vesper-quota-report` for summarized accounting
- `vesper-key-watch-report` for cross-checking key and watch state

## Example workflows

Replay and inspect a catalog journal:

```sh
./build/vesper-replay examples/basic_archive.vsp
./build/vesper-inspect examples/basic_archive.vsp
```

Generate a checkpoint-oriented report:

```sh
./build/vesper-checkpoint-report examples/checkpoint_audit.vsp
./build/vesper-state-digest-report examples/checkpoint_audit.vsp
```

Review key rotation and watch activity:

```sh
./build/vesper-key-watch-report examples/watch_audit.vsp
./build/vesper-watch-review-dump examples/watch_audit.vsp
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

## Development notes

The repository includes a small set of sample journals under `examples/` and a
broader regression inventory under `tests/`. Optional robustness harnesses live
under `fuzz/` for development builds, while the checked-in corpus files remain
non-crashing examples of ordinary journal traffic.
