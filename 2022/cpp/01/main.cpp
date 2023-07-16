#include <array>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

int part_one(std::vector<std::vector<int>> const &elves) {
    int best_elf{};
    for (auto elf: elves) {
        int current_elf = std::accumulate(std::begin(elf), std::end(elf), 0);
        best_elf = std::max(best_elf, current_elf);
    }
    return best_elf;
}

int part_two(std::vector<std::vector<int>> const &elves) {
    std::array<int, 4> best_elves{};
    for (auto elf: elves) {
        int current_elf = std::accumulate(std::begin(elf), std::end(elf), 0);
        best_elves[0] = current_elf;
        std::sort(std::begin(best_elves), std::end(best_elves));
    }
    return std::accumulate(std::begin(best_elves) + 1, std::end(best_elves), 0);
}

int main() {
    std::string input;

    std::vector<int> curr_elf;
    std::vector<std::vector<int>> elves;
    while (std::getline(std::cin, input)) {
        if (input == "") {
            if (!curr_elf.empty()) {
                elves.push_back(std::move(curr_elf));
            }
        } else {
            curr_elf.push_back(std::stoi(input));
        }
    }

    std::cout << "Part one: " << part_one(elves) << '\n';
    std::cout << "Part two: " << part_two(elves) << '\n';
}