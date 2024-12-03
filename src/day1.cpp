#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

int main() {
    // **********************************************************************************************
    // PARSING
    // **********************************************************************************************

    std::ifstream input("./input/day1", std::ios::in);
    // std::ifstream input("./input/day1_example", std::ios::in);

    std::vector<int> first;
    std::vector<int> second;

    for (std::string line; std::getline(input, line);) {
        int i = 0;
        bool state = true;
        std::string value_str;

        auto push_value = [&]() {
            // append value
            int value = std::stoi(value_str);
            state ? first.push_back(value) : second.push_back(value);
            state = !state;
            value_str.clear();
        };

        while (i < line.length()) {
            char c = line.at(i);
            if (std::isspace(c)) {
                push_value();
                // forward to next non-whitespace character
                while (std::isspace(c)) {
                    i++;
                    c = line.at(i);
                }
            }

            value_str.push_back(c);
            i++;
        }

        push_value();
    }

    std::sort(first.begin(), first.end());
    std::sort(second.begin(), second.end());

    // **********************************************************************************************
    // FIRST PART
    // **********************************************************************************************

    int i = 0;

    long long result = 0;
    while (i < first.size()) {
        int dt = std::abs(first.at(i) - second.at(i));
        result += dt;
        i++;
    }

    std::cout << "FIRST PART RESULT: " << result << std::endl;

    // **********************************************************************************************
    // SECOND PART
    // **********************************************************************************************

    std::unordered_map<int, int> freqs;
    std::for_each(second.begin(), second.end(), [&](int v) {
        if (freqs.contains(v)) {
            int c = freqs.at(v);
            freqs[v] = c + 1;
        } else {
            freqs.insert({v, 1});
        }
    });

    auto get_or_default = [&](int v) {
        int freq;
        if (freqs.contains(v)) {
            return freqs.at(v);
        }

        return 0;
    };

    result = 0;
    std::for_each(first.begin(), first.end(), [&](int v) {
        int freq = get_or_default(v);
        int score = v * freq;
        result += score;
    });

    std::cout << "SECOND PART RESULT: " << result << std::endl;

    return 0;
}
