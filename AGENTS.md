# Repository Purpose

This repository is for practicing C++ through LeetCode- and NeetCode-style
problems. The user is expected to write and reason about the algorithm. Do not
solve a problem unless the user explicitly asks for the solution.

Each problem normally contains:

- `details.md` with the prompt, constraints, and public examples
- `solution.hpp` with the primary submit-ready `Solution` class
- optional `solution_<approach>.hpp` files for alternative implementations
- `test_solution.cpp` with doctest cases shared by every approach
- optional `benchmark_solution.cpp` with workloads shared by every approach
- `meson.build` registering one test executable per approach

The local test infrastructure is development-only. Keep framework and build
code out of every `solution.hpp`.

Use descriptive suffixes such as `solution_in_place.hpp` for alternatives. The
primary file stays `solution.hpp` so single-solution problems remain simple.
Each solution header may define the LeetCode-required `class Solution` because
the variants are compiled in separate executables. `test_solution.cpp` selects
one header through `SOLUTION_HEADER`; never include multiple variant headers in
the same translation unit.

# Standard Workflow

Run the documented commands directly from the repository root:

```sh
meson setup build
meson test -C build --suite <problem_name> --print-errorlogs
```

Use `meson test -C build <problem_name>_<approach> --print-errorlogs` when only
one approach should run.

Use the system-installed `c++`, `meson`, and `ninja`. Do not create temporary
virtual environments, alter `PATH`, download substitute executables, invoke an
ad hoc compiler command, or introduce a parallel build workflow to bypass a
missing or broken prerequisite.

Direct compiler invocations are allowed when the user explicitly asks to
compare compilers or experiment with compilation flags. Use the checked-in
sources and pinned doctest headers described in `README.md`. Do not present such
an experiment as equivalent to the standard Meson verification workflow.

If a required tool is unavailable or the documented workflow fails, stop and
surface the issue. State the exact command, relevant error, expected tool or
configuration, and the smallest user action needed. Do not install system or
project dependencies without explicit permission.

The doctest and Google Benchmark versions are pinned by their wrap files under
`subprojects/`. Do not replace them with unpinned system dependencies or other
frameworks as a workaround.

# Tests

Tests should cover public examples plus clearly reasoned boundary and failure
cases permitted by the problem constraints. Do not claim that locally authored
cases are LeetCode's hidden tests.

An unsolved scaffold may intentionally have red tests. Do not weaken tests,
change expected values, or implement the algorithm merely to make verification
green. Distinguish a successful build with expected assertion failures from an
infrastructure or compilation failure.

Keep tests concise and specific to the current problem. Prefer doctest's own
assertions and diagnostics over custom loops, counters, or reporting helpers.
Compile the same test source against every registered solution variant rather
than duplicating test cases per approach.

# Benchmarks

Benchmarks are optional and must not run as part of the normal correctness test
workflow. Use the documented release build directly from the repository root:

```sh
meson setup build-bench --buildtype=release -Dbenchmarks=true
meson test -C build-bench --benchmark --suite <problem_name> --verbose
```

Use fixed seeds and inputs permitted by the problem constraints. Generate
datasets outside timed loops, compile the same benchmark source against every
variant, and prevent the optimizer from discarding measured calls. Do not edit
solution algorithms merely to facilitate benchmarking.

Treat timing results as machine- and configuration-specific observations, not
universal performance claims. Allocation counters report allocation traffic,
not peak resident memory. Keep correctness claims based on tests rather than
benchmark checksums or timing output.

# Communication And Uncertainty

Surface uncertainty, environmental issues, conflicting requirements, and
unexpected behavior aggressively. Never hide them behind a silent fallback or
workaround.

Clearly distinguish verified facts from assumptions. When verification cannot
be completed exactly as documented, say so immediately and explain why. Do not
report substituted verification as equivalent to the requested workflow.

Do not silently skip tests, suppress failures, relax compiler settings, change
the environment, or add compatibility code. If more than one materially
different resolution is possible, ask the user one focused question before
proceeding.

Preserve unrelated user changes. Keep repository documentation synchronized
with any intentional changes to the standard build or test workflow.
