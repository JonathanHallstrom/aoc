#include <array>
#include <bitset>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <vector>

int part_one(std::vector<std::string> const &trees) {
    std::vector<std::vector<bool>> visible(trees.size(), std::vector<bool>(trees[0].size(), false));

    // visible from the top
    for (std::size_t i = 0; i < trees[0].size(); ++i) {
        char tallest = trees[0][i];
        visible[0][i] = true;
        for (std::size_t j = 1; j < trees.size(); ++j) {
            if (trees[j][i] > tallest)
                visible[j][i] = true, tallest = trees[j][i];
        }
    }

    // visible from the bottom
    for (std::size_t i = 0; i < trees[0].size(); ++i) {
        char tallest = trees[trees.size() - 1][i];
        visible[trees.size() - 1][i] = true;
        for (std::size_t j = trees.size() - 1; j-- > 0;) {
            if (trees[j][i] > tallest)
                visible[j][i] = true, tallest = trees[j][i];
        }
    }

    // visible from the left
    for (std::size_t i = 0; i < trees.size(); ++i) {
        char tallest = trees[i][0];
        visible[i][0] = true;
        for (std::size_t j = 1; j < trees[0].size(); ++j) {
            if (trees[i][j] > tallest)
                visible[i][j] = true, tallest = trees[i][j];
        }
    }

    // visible from the right
    for (std::size_t i = 0; i < trees.size(); ++i) {
        char tallest = trees[i][trees[0].size() - 1];
        visible[i][trees[0].size() - 1] = true;
        for (std::size_t j = trees[0].size() - 1; j-- > 0;) {
            if (trees[i][j] > tallest)
                visible[i][j] = true, tallest = trees[i][j];
        }
    }

    int num_visible = 0;
    for (auto v: visible)
        for (auto j: v)
            if (j)
                num_visible += 1;
    return num_visible;
}

int part_two(std::vector<std::string> const &trees) {
    auto is_valid = [&trees](int i, int j) {
        return 0 <= i && i < trees.size() && 0 <= j && j < trees[0].size();
    };
    auto get_dist = [&trees, is_valid](int i, int j, int di, int dj) {
        char start_height = trees[i][j];
        int res = 0;
        while (is_valid(i + di, j + dj) && trees[i + di][j + dj] < start_height) {
            ++res;
            i += di, j += dj;
        }
        return res + is_valid(i + di, j + dj);
    };

    int ans = 0;

    for (int i = 0; i < trees.size(); ++i) {
        for (int j = 0; j < trees[0].size(); ++j) {
            int scenic_score = get_dist(i, j, 0, 1) * get_dist(i, j, 0, -1) *
                               get_dist(i, j, 1, 0) * get_dist(i, j, -1, 0);
            if (ans < scenic_score)
                ans = scenic_score;
        }
    }

    return ans;
}

int main() {
    std::vector<std::string> trees;

    std::string line;
    while (std::cin >> line) {
        trees.push_back(std::move(line));
    }

    std::cout << "Part one: " << part_one(trees) << '\n';
    std::cout << "Part two: " << part_two(trees) << '\n';
}