solution_variants = {
  'opening_stack': 'solution.hpp',
  'in_place': 'solution_in_place.hpp',
}

foreach variant, solution_header : solution_variants
  test_name = 'valid_parentheses_' + variant

  variant_tests = executable(
    test_name + '_tests',
    'test_solution.cpp',
    dependencies: doctest_main_dep,
    cpp_args: [
      '-DSOLUTION_HEADER="@0@"'.format(solution_header),
    ],
  )

  test(
    test_name,
    variant_tests,
    suite: 'valid_parentheses',
  )
endforeach
