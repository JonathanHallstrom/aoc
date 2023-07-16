#include <array>
#include <bitset>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

struct range {
    int start, end;

    bool contains(int point) const {
        return start <= point && point <= end;
    }

    bool contains(range other) const {
        return contains(other.start) && contains(other.end);
    }

    bool overlaps(range other) const {
        return other.contains(start) || other.contains(end);
    }
};

int part_one(std::vector<std::pair<range, range>> const &assignment_pairs) {
    int result = 0;
    for (auto [f, s]: assignment_pairs) {
        result += f.contains(s) || s.contains(f);
    }
    return result;
}

int part_two(std::vector<std::pair<range, range>> const &assignment_pairs) {
    int result = 0;
    for (auto [f, s]: assignment_pairs) {
        result += f.overlaps(s) || s.overlaps(f);
    }
    return result;
}

int main() {
    std::vector<std::pair<range, range>> assignment_pairs;
    std::string rucksack;
    range first, second;
    char unused;
    while (std::cin >> first.start >> unused >> first.end >> unused >> second.start >> unused >> second.end) {
        assignment_pairs.emplace_back(first, second);
    }

    std::cout << "Part one: " << part_one(assignment_pairs) << '\n';
    std::cout << "Part two: " << part_two(assignment_pairs) << '\n';
}