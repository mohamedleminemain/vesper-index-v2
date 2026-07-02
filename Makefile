CXX ?= c++
CXXFLAGS ?= -O2 -g -std=c++17 -Wall -Wextra -Wpedantic
CPPFLAGS ?= -Iinclude

SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(SOURCES:.cpp=.o)

.PHONY: all test clean asan

all: vesper-replay vesper-engine-tests

vesper-replay: tools/replay.cpp $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $^ -o $@

vesper-engine-tests: tests/engine_tests.cpp $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $^ -o $@

test: vesper-engine-tests
	./vesper-engine-tests

asan:
	$(MAKE) clean
	$(MAKE) CXXFLAGS="-O1 -g -std=c++17 -Wall -Wextra -Wpedantic -fsanitize=address -fno-omit-frame-pointer"

clean:
	rm -f src/*.o vesper-replay vesper-engine-tests
