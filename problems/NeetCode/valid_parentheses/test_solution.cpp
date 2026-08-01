#ifndef SOLUTION_HEADER
#define SOLUTION_HEADER "solution.hpp"
#endif

#include SOLUTION_HEADER

#include <doctest/doctest.h>

TEST_CASE("valid bracket sequences are accepted") {
    Solution solution;

    CHECK(solution.isValid("[]"));
    CHECK(solution.isValid("([{}])"));
    CHECK(solution.isValid("()[]{}"));
    CHECK(solution.isValid("{[()]}"));
}

TEST_CASE("invalid bracket sequences are rejected") {
    Solution solution;

    CHECK_FALSE(solution.isValid("[(])"));
    CHECK_FALSE(solution.isValid("(]"));
    CHECK_FALSE(solution.isValid("("));
    CHECK_FALSE(solution.isValid(")"));
    CHECK_FALSE(solution.isValid("([)]"));
    CHECK_FALSE(solution.isValid("(()"));
    CHECK_FALSE(solution.isValid("())"));
}
