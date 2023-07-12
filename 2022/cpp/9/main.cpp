#include <array>
#include <bitset>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <vector>

int part_one(std::vector<std::array<int, 3>> const &moves) {
    std::array<int, 2> h{}, t{};

    auto handle_tail = [&h, &t]() {
        double dist = std::hypot(h[0] - t[0], h[1] - t[1]);
        auto sign = [](int x) {
            return (x > 0) - (x < 0);
        };
        if (dist >= 2) {
            t[0] += sign(h[0] - t[0]);
            t[1] += sign(h[1] - t[1]);
        }
    };

    std::map<std::array<int, 2>, bool> visited;
    for (auto [dj, di, steps]: moves) {
        while (steps--) {
            h[0] += di;
            h[1] += dj;
            handle_tail();
            visited[t] = true;
        }
    }
    int left = 1e9, right = -1e9, top = -1e9, bottom = 1e9;
    for (auto [pos, _]: visited) {
        auto [ver, hor] = pos;
        left = std::min(left, hor);
        right = std::max(right, hor);
        top = std::max(top, ver);
        bottom = std::min(bottom, ver);
    }

    std::cout << '\n';
    for (int i = top; i >= bottom; --i) {
        for (int j = left; j <= right; ++j) {
            std::cout << (visited[{i, j}] ? '#' : '.');
        }
        std::cout << '\n';
    }

    int res = 0;
    for (auto [_, vis]: visited) {
        res += vis;
    }
    return res;
}

int part_two(std::vector<std::array<int, 3>> const &moves) {

    auto handle_tail = [](std::array<int, 2> h, std::array<int, 2> &t) {
        double dist = std::hypot(h[0] - t[0], h[1] - t[1]);
        auto sign = [](int x) {
            return (x > 0) - (x < 0);
        };
        if (dist >= 2) {
            t[0] += sign(h[0] - t[0]);
            t[1] += sign(h[1] - t[1]);
        }
    };

    std::array<std::array<int, 2>, 10> rope_sections{};

    std::map<std::array<int, 2>, bool> visited;

    for (auto [dj, di, steps]: moves) {
        while (steps--) {
            rope_sections[0][0] += di;
            rope_sections[0][1] += dj;
            for (int i = 0; i + 1 < 10; ++i) {
                handle_tail(rope_sections[i], rope_sections[i + 1]);
            }
            visited[rope_sections.back()] = true;
        }
    }
    int left = 1e9, right = -1e9, top = -1e9, bottom = 1e9;
    for (auto [pos, _]: visited) {
        auto [ver, hor] = pos;
        left = std::min(left, hor);
        right = std::max(right, hor);
        top = std::max(top, ver);
        bottom = std::min(bottom, ver);
    }
    std::cout << '\n';
    for (int i = top; i >= bottom; --i) {
        for (int j = left; j <= right; ++j) {
            std::cout << (visited[{i, j}] ? '#' : '.');
        }
        std::cout << '\n';
    }

    int res = 0;
    for (auto [_, vis]: visited) {
        res += vis;
    }
    return res;
}

int main() {
    //          dx dy steps
    std::vector<std::array<int, 3>> moves;

    char c;
    int steps;
    while (std::cin >> c >> steps) {
        switch (c) {
            case 'U':
                moves.push_back({0, 1, steps});
                break;
            case 'R':
                moves.push_back({1, 0, steps});
                break;
            case 'D':
                moves.push_back({0, -1, steps});
                break;
            case 'L':
                moves.push_back({-1, 0, steps});
                break;
        }
    }

    std::cout << "Part one: " << part_one(moves) << '\n';
    std::cout << "Part two: " << part_two(moves) << '\n';
}