#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>

using namespace std;

enum Direction {
    Increasing,
    Decreasing,
};

inline Direction get_direction(vector<int> &a, int x, int y) {
    if (a.at(x) > a.at(y)) {
        return Direction::Decreasing;
    }
    return Direction::Increasing;
}

bool are_indexes_safe(vector<int> &levels, Direction prev_dir, int x, int y) {
    if (get_direction(levels, x, y) != prev_dir) {
        return false;
    }

    int dt = std::abs(levels.at(x) - levels.at(y));

    if (!(dt >= 1) || !(dt <= 3)) {
        return false;
    }

    return true;
}

bool is_level_safe(vector<int> &levels, int dampening) {
    unordered_set<int> skiplist;
    int a = 0;
    int b = 1;

    auto increment_with_skiplist = [&]() {
        a++;
        while (skiplist.contains(a)) {
            a++;
        }

        b++;
        while (skiplist.contains(b)) {
            b++;
        }

        if (b <= a) {
            b = a + 1;
        }
    };

    Direction dir = get_direction(levels, a, b);

    while (b < levels.size()) {
        if (!are_indexes_safe(levels, dir, a, b)) {
            skiplist.insert(b);
            b++;
            continue;
        }

        increment_with_skiplist();
    }

    return skiplist.size() <= dampening;
}

int main() {
    // ifstream in("./input/day2", ios::in);
    ifstream in("./input/day2_example", ios::in);

    if (!in) {
        cerr << "Error reading file: " << strerror(errno) << std::endl;
        return 1;
    }
    vector<vector<int>> reports;

    for (std::string line; std::getline(in, line);) {
        vector<int> levels;
        stringstream ss(line);
        string s;
        while (ss >> s) {
            levels.emplace_back(std::stoi(s));
        }

        reports.emplace_back(levels);
    }

    long long result = 0;
    for_each(reports.begin(), reports.end(), [&](vector<int> &levels) {
        if (is_level_safe(levels, 0)) {
            result++;
        }
    });

    cout << "PART 1 RESULT: " << result << std::endl;

    result = 0;
    for_each(reports.begin(), reports.end(), [&](vector<int> &levels) {
        if (is_level_safe(levels, 1)) {
            result++;
        }
    });

    cout << "PART 2 RESULT: " << result << std::endl;

    return 0;
}
