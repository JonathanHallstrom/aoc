#include <algorithm>
#include <complex>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

using position = std::complex<int>;

struct hash {
    size_t operator()(position const &a) const {
        return std::hash<int>{}(a.real()) * 31 ^ std::hash<int>{}(a.imag());
    }
};

using map = std::unordered_map<position, bool, hash>;

int part_one(map board) {
    position lowest_pos = board.begin()->first;
    for (auto [p, _]: board)
        if (p.real() > lowest_pos.real())
            lowest_pos = p;

    const position down_left{1, -1}, down{1, 0}, down_right{1, 1};

    for (int sand_count = 0;; ++sand_count) {
        position sand_pos{0, 500};
        while (sand_pos.real() <= lowest_pos.real()) {
            if (board[sand_pos + down]) {
                if (board[sand_pos + down_left]) {
                    if (board[sand_pos + down_right]) {
                        break;
                    } else {
                        sand_pos += down_right;
                    }
                } else {
                    sand_pos += down_left;
                }
            } else {
                sand_pos += down;
            }
        }
        if (sand_pos.real() > lowest_pos.real()) {
            return sand_count;
        } else {
            board[sand_pos] = true;
        }
    }
    return -1;
}

int part_two(map board) {
    const position down_left{1, -1}, down{1, 0}, down_right{1, 1};
    position lowest_pos = board.begin()->first;
    for (auto [p, _]: board)
        if (p.real() > lowest_pos.real())
            lowest_pos = p;
    lowest_pos += down;

    for (int sand_count = 0;; ++sand_count) {
        position sand_pos{0, 500};
        if (board[sand_pos])
            return sand_count;
        while (sand_pos.real() < lowest_pos.real()) {
            if (board[sand_pos + down]) {
                if (board[sand_pos + down_left]) {
                    if (board[sand_pos + down_right]) {
                        break;
                    } else {
                        sand_pos += down_right;
                    }
                } else {
                    sand_pos += down_left;
                }
            } else {
                sand_pos += down;
            }
        }
        board[sand_pos] = true;
    }
    return -1;
}

int main() {
    map board;

    std::string s;
    while (std::getline(std::cin, s)) {
        std::istringstream iss(s);
        char comma;
        int a, b;
        iss >> a >> comma >> b;
        position start{b, a};
        board[start] = true;
        std::string dummy;
        while (iss >> dummy >> a >> comma >> b) {
            position p{b, a};
            position diff = p - start;
            diff /= std::hypot(diff.real(), diff.imag());

            while (start != p) {
                start += diff;
                board[start] = true;
            }
        }
    }

    std::cout << "Part one: " << part_one(board) << '\n';
    std::cout << "Part two: " << part_two(board) << '\n';
}