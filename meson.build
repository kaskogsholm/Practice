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

subdir('problems/NeetCode/valid_parentheses')
