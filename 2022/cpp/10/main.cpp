#include <array>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

int part_one(std::vector<int> const &instructions) {
    int ip = -1;
    int cycle = 0;
    int x = 1;
    int add_value = 0;
    int cycles_remaining = 1;

    int result = 0;
    while (ip < (int) instructions.size()) {
        ++cycle;
        int new_cycles_remaining = cycles_remaining - 1, new_add_value = add_value;
        if (new_cycles_remaining == 0) {
            if (add_value != 0x7fffffff) {
                x += add_value;
            }
            new_add_value = instructions[++ip];
            new_cycles_remaining = new_add_value == 0x7fffffff ? 1 : 2;
        }
        if (cycle % 40 == 20 && cycle <= 220) {
            result += x * cycle;
        }
        cycles_remaining = new_cycles_remaining;
        add_value = new_add_value;
    }
    while (cycles_remaining) {
        ++cycle;
        if (cycle % 40 == 20 && cycle <= 220) {
            std::cout << cycle << ' ' << x << '\n';
            result += x * cycle;
        }

        if (--cycles_remaining == 0) {
            if (add_value != 0x7fffffff) {
                x += add_value;
            }
        }
    }
    return result;
}

std::string part_two(std::vector<int> const &instructions) {
    int ip = -1;
    int cycle = 0;
    int x = 1;
    int add_value = 0;
    int cycles_remaining = 1;

    std::string result = "\n";
    while (++cycle <= 240) {
        int new_cycles_remaining = cycles_remaining - 1, new_add_value = add_value;
        if (new_cycles_remaining == 0) {
            if (add_value != 0x7fffffff) {
                x += add_value;
            }
            new_add_value = instructions[++ip];
            new_cycles_remaining = new_add_value == 0x7fffffff ? 1 : 2;
        }

        int row_idx = (cycle - 1) % 40;
        result.push_back(x - 1 <= row_idx && row_idx <= x + 1 ? '#' : '.');

        if (cycle % 40 == 0)
            result.push_back('\n');

        cycles_remaining = new_cycles_remaining;
        add_value = new_add_value;
    }
    return result;
}

int main() {
    std::vector<int> instructions;

    std::string op;
    while (std::cin >> op) {
        if (op == "addx") {
            int v;
            std::cin >> v;
            instructions.push_back(v);
        } else {
            instructions.push_back(0x7fffffff);
        }
    }

    // add noops to the end in case we overrun the buffer with an ADDX instruction
    instructions.push_back(0x7fffffff);
    instructions.push_back(0x7fffffff);

    std::cout << "Part one: " << part_one(instructions) << '\n';
    std::cout << "Part two: " << part_two(instructions) << '\n';
}