solution_variants = {
  'opening_stack': 'solution.hpp',
  'in_place': 'solution_in_place.hpp',
}

foreach variant, solution_header : solution_variants
  test_name = 'valid_parentheses_' + variant
  solution_selection_args = [
    '-DSOLUTION_HEADER="@0@"'.format(solution_header),
  ]

  variant_tests = executable(
    test_name + '_tests',
    'test_solution.cpp',
    dependencies: doctest_main_dep,
    cpp_args: solution_selection_args,
  )

  test(
    test_name,
    variant_tests,
    suite: 'valid_parentheses',
  )

  if benchmarks_enabled
    benchmark_name = test_name + '_benchmark'
    variant_benchmark = executable(
      benchmark_name,
      'benchmark_solution.cpp',
      dependencies: [
        google_benchmark_dep,
        benchmark_support_dep,
      ],
      cpp_args: solution_selection_args,
      build_by_default: false,
    )

    benchmark(
      benchmark_name,
      variant_benchmark,
      suite: 'valid_parentheses',
    )
  endif
endforeach
