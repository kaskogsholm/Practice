#pragma once

#include <stack>
#include <stdexcept>
#include <string>

class Solution {
  public:
    bool is_opening(char c) { return c == '(' || c == '{' || c == '['; }
    bool is_closing(char c) { return c == ')' || c == '}' || c == ']'; }
    char matching_opening(char closing_bracket) {
        switch (closing_bracket) {
        case ')':
            return '(';
        case '}':
            return '{';
        case ']':
            return '[';
        default:
            throw std::invalid_argument("Not a closing bracket");
        }
    }
    bool isValid(std::string s) {
        std::stack<char, std::string> openings;

        // main loop

        for (char character : s) {
            const bool opening = is_opening(character);

            if (opening) {
                openings.push(character);
                continue;
            }

            const bool closing = is_closing(character);

            if (!closing) {
                // character must not be an opening
                // so if it's also not a closing
                // it's not a bracket at all
                return false;
            }
            // Because the character is valid and isn't opening,
            // it must be closing.
            if (openings.empty() ||
                (openings.top() != matching_opening(character))) {
                // This is invalid because we have a closing
                // character but not matching previously encountered
                // opening
                return false;
            }

            openings.pop();
        }

        // at this point, if there are any dangling openings,
        // the string is invalid. Otherwise, it's valid
        return openings.empty();
    }
};
