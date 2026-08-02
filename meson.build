project(
  'practice',
  'cpp',
  default_options: [
    'cpp_std=c++20',
    'warning_level=3',
  ],
)

doctest_project = subproject('doctest')
doctest_dep = doctest_project.get_variable('doctest_dep')

doctest_main_library = static_library(
  'doctest_main',
  'tests/doctest_main.cpp',
  dependencies: doctest_dep,
)

doctest_main_dep = declare_dependency(
  dependencies: doctest_dep,
  link_with: doctest_main_library,
)

benchmarks_enabled = get_option('benchmarks')
if benchmarks_enabled
  google_benchmark_project = subproject(
    'google-benchmark',
    default_options: [
      'default_library=static',
      'tests=disabled',
    ],
  )
  google_benchmark_dep = google_benchmark_project.get_variable(
    'google_benchmark_dep',
  )
  benchmark_support_include = include_directories('benchmarks')
  benchmark_support_library = static_library(
    'benchmark_support',
    'benchmarks/allocation_tracker.cpp',
    include_directories: benchmark_support_include,
    build_by_default: false,
  )
  benchmark_support_dep = declare_dependency(
    link_with: benchmark_support_library,
    include_directories: benchmark_support_include,
  )
endif

subdir('problems/NeetCode/valid_parentheses')
