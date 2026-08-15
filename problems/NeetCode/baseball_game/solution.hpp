#pragma once

#include <cassert>
#include <iostream>
#include <numeric>
#include <stack>
#include <string>
#include <vector>

using namespace std;
void validate_special_op(const stack<int> &record,
                         std::stack<int>::size_type min_scores_in_record) {
    assert(record.size() >= min_scores_in_record);
    // if (record.size() < min_scores_in_record) {
    // throw std::invalid_argument("Not enough scores in record.");
    //}
}

class Solution {
  public:
    int calPoints(vector<string> &operations) {
        stack<int> record;

        for (string op : operations) {
            cout << op << '\n';
            int last_score;
            int new_score;
            switch (op[0]) {
                case '+':
                    validate_special_op(record, 2);
                    last_score = record.top();
                    record.pop();
                    new_score = last_score + record.top();
                    record.push(last_score);
                    record.push(new_score);
                    break;
                case 'D':
                    validate_special_op(record, 1);
                    record.push(record.top() * 2);
                    break;
                case 'C':
                    validate_special_op(record, 1);
                    record.pop();
                    break;
                default:
                    int score = std::stoi(op);
                    record.push(score);

                    // integer
            }
        }
        int sum = 0;
        while (!record.empty()) {
            sum += record.top();
            record.pop();
        }
        return sum;
    }
};
