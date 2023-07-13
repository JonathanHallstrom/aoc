#include <array>
#include <cassert>
#include <complex>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <queue>
#include <string>
#include <vector>

int part_one(std::vector<std::string> heightmap) {
    const int n = (int) heightmap.size(), m = (int) heightmap[0].size();

    using position = std::complex<int>;
    auto is_valid = [n, m](position p) {
        return 0 <= p.real() && p.real() < n && 0 <= p.imag() && p.imag() < m;
    };

    std::queue<std::pair<position, int>> q;
    std::vector<std::vector<bool>> reached(n, std::vector<bool>(m, false));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (heightmap[i][j] == 'S') {
                reached[i][j] = true;
                heightmap[i][j] = 'a';
                q.push({{i, j}, 0});
            }
        }
    }

    while (!q.empty()) {
        auto [c, dist] = q.front();
        q.pop();

        constexpr position directions[] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

        for (auto delta: directions) {
            position new_pos = c + delta;
            int new_dist = dist + 1;
            if (!is_valid(new_pos))
                continue;
            if (heightmap[new_pos.real()][new_pos.imag()] == 'E' && heightmap[c.real()][c.imag()] >= 'z' - 1) {
                return new_dist;
            }
            if (heightmap[new_pos.real()][new_pos.imag()] <= heightmap[c.real()][c.imag()] + 1 && !reached[new_pos.real()][new_pos.imag()]) {
                reached[new_pos.real()][new_pos.imag()] = true;
                q.push({new_pos, new_dist});
            }
        }
    }

    return -1;
}

int part_two(std::vector<std::string> const &heightmap) {
    const int n = (int) heightmap.size(), m = (int) heightmap[0].size();

    using position = std::complex<int>;
    auto is_valid = [n, m](position p) {
        return 0 <= p.real() && p.real() < n && 0 <= p.imag() && p.imag() < m;
    };

    std::queue<std::pair<position, int>> q;
    std::vector<std::vector<bool>> reached(n, std::vector<bool>(m, false));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (heightmap[i][j] == 'a') {
                reached[i][j] = true;
                q.push({{i, j}, 0});
            }
        }
    }

    while (!q.empty()) {
        auto [c, dist] = q.front();
        q.pop();

        constexpr position directions[] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

        for (auto delta: directions) {
            position new_pos = c + delta;
            int new_dist = dist + 1;
            if (!is_valid(new_pos))
                continue;
            if (heightmap[new_pos.real()][new_pos.imag()] == 'E' && heightmap[c.real()][c.imag()] >= 'z' - 1) {
                return new_dist;
            }
            if (heightmap[new_pos.real()][new_pos.imag()] <= heightmap[c.real()][c.imag()] + 1 && !reached[new_pos.real()][new_pos.imag()]) {
                reached[new_pos.real()][new_pos.imag()] = true;
                q.push({new_pos, new_dist});
            }
        }
    }

    return -1;
}

int main() {
    std::vector<std::string> heightmap;

    std::string inp;
    while (std::cin >> inp) {
        heightmap.push_back(inp);
    }

    std::cout << "Part one: " << part_one(heightmap) << '\n';
    std::cout << "Part two: " << part_two(heightmap) << '\n';
}