#include <array>
#include <bitset>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

int idx(char c) {
    // return (-('a' <= c && c <= 'z') & (c - 'a')) | (-!('a' <= c && c <= 'z') & (c - 'A' + 26));
    if ('a' <= c && c <= 'z') {
        return c - 'a';
    } else {
        return c - 'A' + 26;
    }
}

int part_one(std::vector<std::string> const &rucksacks) {
    int result = 0;
    for (auto &rucksack: rucksacks) {
        bool left[52]{}, right[52]{};
        for (std::size_t i = 0; i < rucksack.size() / 2; ++i) {
            left[idx(rucksack[i])] = true;
            right[idx(rucksack[i + rucksack.size() / 2])] = true;
        }
        for (int i = 0; i < 52; ++i) {
            // result += -(left[i] & right[i]) & i + 1;
            if (left[i] && right[i]) {
                result += i + 1;
            }
        }
    }
    return result;
}

int part_two(std::vector<std::string> const &rucksacks) {
    int result = 0;
    for (std::size_t i = 0; i + 2 < rucksacks.size(); i += 3) {
        std::bitset<52> contained[3]{};

        for (std::size_t j = 0; j < 3; ++j) {
            for (auto c: rucksacks[i + j]) {
                contained[j][idx(c)] = true;
            }
        }

        std::bitset<52> contained_in_all = contained[0] & contained[1] & contained[2];

        for (int i = 0; i < 52; ++i) {
            // result += -contained_in_all[i] & i + 1;
            if (contained_in_all[i]) {
                result += i + 1;
            }
        }
    }
    return result;
}

int main() {
    std::vector<std::string> rucksacks;
    std::string rucksack;
    while (std::cin >> rucksack) {
        rucksacks.emplace_back(std::move(rucksack));
    }

    std::cout << "Part one: " << part_one(rucksacks) << '\n';
    std::cout << "Part two: " << part_two(rucksacks) << '\n';
}