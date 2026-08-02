# Practice

C++ solutions are kept separate from their local test harnesses. A problem's
primary `solution.hpp` contains the `Solution` class intended for submission.
Optional alternatives use flat, descriptive names such as
`solution_in_place.hpp`. All approaches use the same doctest cases, while Meson
builds and runs each one independently.

## Prerequisites

- A C++20 compiler
- Meson
- Ninja

On Arch Linux, install the required tools with:

```sh
sudo pacman -Syu --needed gcc meson ninja
```

Confirm that the normal shell environment can find them:

```sh
c++ --version
meson --version
ninja --version
```

Meson downloads the version of doctest pinned in `subprojects/doctest.wrap`
during the initial setup. The optional benchmark setup similarly uses the
pinned `subprojects/google-benchmark.wrap`. The solution code itself has no
external dependencies.

## Build setup

Configure the shared build directory from the repository root:

```sh
meson setup build
```

The generated build files and test executables are placed under `build/`,
which is ignored by Git.

If an existing build directory was configured with a different toolchain or
environment, recreate it with the installed tools:

```sh
meson setup --wipe build
```

## Running tests

Run every solution for one problem:

```sh
meson test -C build --suite valid_parentheses --print-errorlogs
```

This is the normal command while working on `valid_parentheses`. Meson rebuilds
changed sources before running every solution registered for that problem.

Run one specific approach:

```sh
meson test -C build valid_parentheses_opening_stack --print-errorlogs
meson test -C build valid_parentheses_in_place --print-errorlogs
```

Use `--verbose` instead of `--print-errorlogs` to display test output as it
runs:

```sh
meson test -C build --suite valid_parentheses --verbose
```

Run every registered problem:

```sh
meson test -C build --print-errorlogs
```

Meson selects and launches the requested problem executable. doctest supplies
the executable's `main()` function, discovers its `TEST_CASE` blocks, and
reports assertion failures with their source locations and evaluated values.

An unsolved problem or approach may have failing tests and a nonzero test exit
status. That red state can be expected algorithm behavior rather than a build
or test-infrastructure failure.

## Running benchmarks

Benchmarks are optional and disabled in the normal build. Configure a separate
optimized build directory when comparing implementations:

```sh
meson setup build-bench --buildtype=release -Dbenchmarks=true
```

Run every benchmark implementation for one problem:

```sh
meson test -C build-bench \
  --benchmark \
  --suite valid_parentheses \
  --verbose
```

Run one implementation or filter Google Benchmark's workload names:

```sh
meson test -C build-bench \
  --benchmark \
  valid_parentheses_in_place_benchmark \
  --verbose

meson test -C build-bench \
  --benchmark \
  valid_parentheses_in_place_benchmark \
  --test-args='--benchmark_filter=isValid/opening_only' \
  --verbose
```

The valid-parentheses benchmarks use fixed-seed datasets at lengths 16, 128,
and 1000. They cover uniformly random brackets, random opening-only strings,
random valid nesting, and random valid shallow pairs. Input generation is
outside the timed loop, and each solution receives the same datasets.

The custom counters `allocs_per_input` and `allocated_bytes_per_input` measure
heap allocation traffic while calling `isValid`. They do not measure peak
resident memory. Benchmark timings depend on the compiler, optimization flags,
CPU state, and system load; use correctness tests before comparing performance.

Use Google Benchmark's pinned comparison tool to calculate relative timing
between two executables. For example, compare one opening-heavy workload with a
short measurement interval:

```sh
python3 subprojects/benchmark-1.8.4/tools/compare.py benchmarks \
  build-bench/problems/NeetCode/valid_parentheses/valid_parentheses_opening_stack_benchmark \
  build-bench/problems/NeetCode/valid_parentheses/valid_parentheses_in_place_benchmark \
  --benchmark_filter=isValid/opening_only/1000 \
  --benchmark_min_time=0.1s
```

The first executable is the baseline and the second is the contender. Negative
time deltas mean the contender was faster in that run. Use repeated runs before
drawing conclusions from small differences.

## Direct compiler experiments

Meson does not generate or transform the C++ test sources. A solution can also
be compiled directly when comparing compilers or experimenting with flags. From
the repository root, compile the opening-stack approach with:

```sh
c++ -std=c++20 -Wall -Wextra -Wpedantic \
  -Isubprojects/doctest-2.5.3 \
  problems/NeetCode/valid_parentheses/test_solution.cpp \
  tests/doctest_main.cpp \
  -o /tmp/valid_parentheses_opening_stack_tests
```

Select the in-place variant with a preprocessor definition:

```sh
c++ -std=c++20 -Wall -Wextra -Wpedantic \
  '-DSOLUTION_HEADER="solution_in_place.hpp"' \
  -Isubprojects/doctest-2.5.3 \
  problems/NeetCode/valid_parentheses/test_solution.cpp \
  tests/doctest_main.cpp \
  -o /tmp/valid_parentheses_in_place_tests
```

Change `c++` or add compiler flags to test another configuration. Both source
files must be compiled and linked: `test_solution.cpp` contains the cases, and
`tests/doctest_main.cpp` supplies the test program's `main()` function.

Direct compiler commands are for deliberate experiments. The Meson commands
remain the standard repository verification workflow.

## Adding a problem

Create a problem directory containing:

- `details.md` for the prompt and links
- `solution.hpp` for the primary submit-ready `Solution` class
- optional `solution_<approach>.hpp` files for alternative implementations
- `test_solution.cpp` for local doctest cases
- optional `benchmark_solution.cpp` for shared performance workloads
- `meson.build` for the problem's test executable

The test source should include the solution and doctest:

```cpp
#ifndef SOLUTION_HEADER
#define SOLUTION_HEADER "solution.hpp"
#endif

#include SOLUTION_HEADER

#include <doctest/doctest.h>

TEST_CASE("descriptive behavior") {
    Solution solution;

    CHECK(solution.method(/* input */) == /* expected */);
}
```

The primary implementation is selected when `SOLUTION_HEADER` is not defined.
Each alternative Meson target defines it as that variant's flat header name, so
the same cases are compiled against every implementation without including
multiple `Solution` classes together.

Register descriptive approach names in the problem's `solution_variants` list,
then add the problem directory with `subdir()` in the root `meson.build`. Test
names and executable target names must be unique across the repository.
