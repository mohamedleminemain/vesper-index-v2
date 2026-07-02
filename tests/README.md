The default test target builds a small behavioral baseline around core archive
replay, checkpoint handling, inspection, and reporting flows.

When `VESPER_BUILD_EXPLORATORY_TESTS=ON` is enabled, CTest also builds a larger
regression inventory that exercises replay, storage, audit, projection,
checkpoint, and reporting combinations against the same public journal format.
