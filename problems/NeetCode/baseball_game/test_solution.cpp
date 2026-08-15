#ifndef SOLUTION_HEADER
#define SOLUTION_HEADER "solution.hpp"
#endif

#include SOLUTION_HEADER

#include <doctest/doctest.h>

#include <string>
#include <vector>

TEST_CASE("calculates the score for the first public example") {
    Solution solution;
    std::vector<std::string> operations{"5", "2", "C", "D", "+"};

    CHECK(solution.calPoints(operations) == 30);
}

TEST_CASE("calculates the score for the second public example") {
    Solution solution;
    std::vector<std::string> operations{
        "5", "-2", "4", "C", "D", "9", "+", "+"};

    CHECK(solution.calPoints(operations) == 27);
}

TEST_CASE("an empty final record has a score of zero") {
    Solution solution;
    std::vector<std::string> operations{"1", "C"};

    CHECK(solution.calPoints(operations) == 0);
}

TEST_CASE("a single negative score is returned") {
    Solution solution;
    std::vector<std::string> operations{"-30000"};

    CHECK(solution.calPoints(operations) == -30000);
}

TEST_CASE("addition and doubling can be chained") {
    Solution solution;
    std::vector<std::string> operations{"30000", "30000", "+", "D"};

    CHECK(solution.calPoints(operations) == 240000);
}
