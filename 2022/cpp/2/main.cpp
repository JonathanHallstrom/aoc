#include <array>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

int part_one(std::vector<std::pair<char, char>> const &strategy_guide) {
    int result = 0;
    for (auto [f, s]: strategy_guide) {
        bool won = (f - 'A' + 1) % 3 == (s - 'X');
        bool lost = (f - 'A') == (s - 'X' + 1) % 3;
        int choice_score = (s - 'X' + 1);
        int round_end_score = 3 + 3 * won - 3 * lost;
        result += choice_score + round_end_score; 
    }
    return result;
}

int part_two(std::vector<std::pair<char, char>> const &strategy_guide) {
    int result = 0;
    for (auto [f, s]: strategy_guide) {
        bool won = s == 'Z';
        bool lost = s == 'X';
        int choice_score = won ? (f - 'A' + 1) % 3 + 1 : lost ? (f - 'A' + 2) % 3 + 1 : (f - 'A') + 1;
        int round_end_score = 3 + 3 * won - 3 * lost;
        result += choice_score + round_end_score; 
    }
    return result;
}

int main() {
    std::vector<std::pair<char, char>> strategy_guide;
    char first, second;
    while (std::cin >> first >> second) {
        strategy_guide.emplace_back(first, second);
    }

    std::cout << "Part one: " << part_one(strategy_guide) << '\n';
    std::cout << "Part two: " << part_two(strategy_guide) << '\n';
}