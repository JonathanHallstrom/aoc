#include <array>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

// the parsing is very inefficient but the input is tiny (< 2KB)

std::vector<std::int64_t> parse_items(std::string const &s) {
    std::istringstream iss(s);
    std::string buf;
    iss >> buf >> buf;

    std::vector<std::int64_t> res;
    int n;
    while (iss >> n) {
        res.push_back(n);
        iss.ignore();
    }
    return res;
}

struct operation {
    int constant;
    bool is_addition;

    int64_t operator()(int64_t old) const {
        int64_t left = old, right = constant ? constant : old;
        if (is_addition)
            return left + right;
        else
            return left * right;
    }
};

operation parse_operation(std::string const &s) {
    operation result;

    std::istringstream iss(s);
    std::string buf;
    iss >> buf >> buf >> buf >> buf;

    char op;
    iss >> op;

    result.is_addition = op == '+';

    iss >> buf;
    if (buf == "old")
        result.constant = 0;
    else
        result.constant = std::stoi(buf);
    return result;
}

struct test {
    int divisor;
    int if_true, if_false;
};

int parse_divisor(std::string const &s) {
    std::istringstream iss(s);
    std::string buf;
    iss >> buf >> buf >> buf;
    int result;
    iss >> result;
    return result;
}

int parse_destination(std::string const &s) {
    std::istringstream iss(s);
    std::string buf;
    iss >> buf >> buf >> buf >> buf >> buf;
    int result;
    iss >> result;
    return result;
}

test parse_test(std::string const &op, std::string const &branch, std::string const &if_true, std::string const &if_false) {
    test result;
    result.divisor = parse_divisor(branch);
    result.if_true = parse_destination(if_true);
    result.if_false = parse_destination(if_false);
    return result;
}

struct monkey {
    int idx;
    std::vector<std::int64_t> items;
    operation op;
    test t;
};

monkey parse_monkey(std::array<std::string, 6> const &s) {
    std::istringstream iss(s[0]);
    std::string dummy;
    iss >> dummy;
    int idx;
    iss >> idx;
    monkey result;
    result.idx = idx;
    result.items = parse_items(s[1]);
    result.op = parse_operation(s[2]);
    result.t = parse_test(s[2], s[3], s[4], s[5]);
    return result;
}

int part_one(std::vector<monkey> monkeys) {
    std::vector<int> counts(monkeys.size());
    for (int round = 0; round < 20; ++round) {
        for (auto &monkey: monkeys) {
            for (auto item: monkey.items) {
                item = monkey.op(item);
                item /= 3;
                if (item % monkey.t.divisor == 0) {
                    monkeys[monkey.t.if_true].items.push_back(item);
                } else {
                    monkeys[monkey.t.if_false].items.push_back(item);
                }
            }
            counts[monkey.idx] += monkey.items.size();
            monkey.items.clear();
        }
    }
    std::cout << '\n';
    for (int i = 0; i < monkeys.size(); ++i) {
        std::cout << i << ": " << counts[i] << '\n';
    }

    std::sort(std::begin(counts), std::end(counts), std::greater{});

    return counts[0] * counts[1];
}

std::int64_t part_two(std::vector<monkey> monkeys) {
    std::int64_t mod = 1;
    for (auto monkey: monkeys)
        mod *= monkey.t.divisor;

    std::vector<int64_t> counts(monkeys.size());
    for (int round = 0; round < 10000; ++round) {
        for (auto &monkey: monkeys) {
            for (auto item: monkey.items) {
                item = monkey.op(item) % mod;
                if (item % monkey.t.divisor == 0) {
                    monkeys[monkey.t.if_true].items.push_back(item);
                } else {
                    monkeys[monkey.t.if_false].items.push_back(item);
                }
            }
            counts[monkey.idx] += monkey.items.size();
            monkey.items.clear();
        }
    }
    std::cout << '\n';
    for (int i = 0; i < monkeys.size(); ++i) {
        std::cout << i << ": " << counts[i] << '\n';
    }

    std::sort(std::begin(counts), std::end(counts), std::greater{});

    return counts[0] * counts[1];
}

int main() {
    std::vector<monkey> monkeys;

    std::array<std::string, 6> s;
    while (std::getline(std::cin, s[0]) &&
           std::getline(std::cin, s[1]) &&
           std::getline(std::cin, s[2]) &&
           std::getline(std::cin, s[3]) &&
           std::getline(std::cin, s[4]) &&
           std::getline(std::cin, s[5])) {
        monkeys.push_back(parse_monkey(s));
        std::cin.ignore();
    }

    for (auto monkey: monkeys) {
        std::cout << "monkey: " << monkey.idx << '\n';
        std::cout << "items: ";
        for (auto item: monkey.items)
            std::cout << item << ' ';
        std::cout << '\n';
        std::cout << "operation: new = old " << (monkey.op.is_addition ? '*' : '*') << ' ' << (monkey.op.constant ? std::to_string(monkey.op.constant) : "old") << '\n';
        std::cout << "Test: divisible by " << monkey.t.divisor << '\n';
        std::cout << " if true: " << monkey.t.if_true << '\n';
        std::cout << " if false: " << monkey.t.if_false << '\n';
        std::cout << '\n';
    }

    std::cout << "Part one: " << part_one(monkeys) << '\n';
    std::cout << "Part two: " << part_two(monkeys) << '\n';
}