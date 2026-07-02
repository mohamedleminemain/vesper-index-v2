#!/bin/sh
set -eu

ROOT="${SRC:-$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)}"
BUILD_DIR="${WORK:-/tmp}/vesper-index-fuzz"
mkdir -p "$BUILD_DIR" "$OUT"

SOURCES="$(printf '%s\n' "$ROOT"/src/*.cpp)"

OBJECTS=""
for source in $SOURCES; do
  name="$(basename "$source" .cpp)"
  object="$BUILD_DIR/$name.o"
  "$CXX" $CXXFLAGS -O1 -std=c++17 -I"$ROOT/include" \
    -c "$source" -o "$object"
  OBJECTS="$OBJECTS $object"
done

"${AR:-llvm-ar}" rcs "$BUILD_DIR/libvesper_index.a" $OBJECTS

for target in archive_fuzzer query_fuzzer checkpoint_fuzzer replay_fuzzer; do
  "$CXX" $CXXFLAGS -O1 -std=c++17 -I"$ROOT/include" \
    "$ROOT/fuzz/$target.cpp" "$BUILD_DIR/libvesper_index.a" \
    $LIB_FUZZING_ENGINE -o "$OUT/$target"
  if [ -d "$ROOT/fuzz/corpus/$target" ]; then
    (cd "$ROOT/fuzz/corpus/$target" && zip -q \
      "$OUT/${target}_seed_corpus.zip" ./*)
  fi
done
