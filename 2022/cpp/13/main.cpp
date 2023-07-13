#include <array>
#include <cassert>
#include <complex>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <queue>
#include <string>
#include <vector>

struct mixed_list {
    int value = -1;
    std::vector<mixed_list> children;

    static mixed_list parse(std::string const &s) {
        std::vector<mixed_list> stack;
        mixed_list result;
        int currently_parsed_int = -1;
        for (auto i: s) {
            switch (i) {
                case '[':
                    stack.emplace_back();
                    break;
                case ']':
                    if (currently_parsed_int != -1) {
                        stack[stack.size() - 1].children.push_back({currently_parsed_int});
                        currently_parsed_int = -1;
                    }
                    if (stack.size() == 1) {
                        result = stack[stack.size() - 1];
                    } else {
                        stack[stack.size() - 2].children.push_back(std::move(stack[stack.size() - 1]));
                    }
                    stack.pop_back();
                    break;
                case ',':
                    if (currently_parsed_int != -1) {
                        stack.back().children.push_back({currently_parsed_int});
                        currently_parsed_int = -1;
                    }
                    break;
                default:
                    if (currently_parsed_int == -1) {
                        currently_parsed_int = 0;
                    }
                    currently_parsed_int = currently_parsed_int * 10 + i - '0';
                    break;
            }
        }
        return result;
    }

    std::string to_string() const {
        if (is_int()) {
            return std::to_string(value);
        } else {
            std::string result = "[";
            bool first = true;
            for (auto c: children) {
                if (!first) {
                    result.push_back(',');
                }
                first = false;
                result.append(c.to_string());
            }
            result.push_back(']');
            return result;
        }
    };
    bool operator<(mixed_list const &other) const {
        auto result = helper(other);
        // std::cout << this->to_string() << " < " << other.to_string() << " == " << result << '\n';
        return result;
    }

    bool helper(mixed_list const &other) const {
        if (is_int() && other.is_int()) {
            return value < other.value;
        } else if (is_int() && !other.is_int()) {
            return mixed_list{-1, std::vector<mixed_list>{mixed_list{value}}} < other;
        } else if (!is_int() && other.is_int()) {
            return *this < mixed_list{-1, std::vector<mixed_list>{mixed_list{other.value}}};
        } else {
            for (std::size_t i = 0; i < std::min(children.size(), other.children.size()); ++i) {
                if (children[i] < other.children[i])
                    return true;
                if (other.children[i] < children[i])
                    return false;
            }

            return children.size() < other.children.size();
        }
    }

    bool is_int() const {
        return value != -1;
    }
};

int part_one(std::vector<mixed_list> const &packets) {
    int result = 0;
    for (int i = 0; i < packets.size(); i += 2) {
        if (packets[i] < packets[i + 1]) {
            result += i / 2 + 1;
        }
    }
    return result;
}

int part_two(std::vector<mixed_list> const &packets) {
    mixed_list two = mixed_list::parse("[[2]]");
    mixed_list six = mixed_list::parse("[[6]]");
    int less_than_two = 0, less_than_six = 1; // two < six
    for (auto i: packets) {
        less_than_two += i < two;
        less_than_six += i < six;
    }
    return (less_than_two + 1) * (less_than_six + 1);
}

int main() {
    {
        std::string test_strings[] = {
                "[1]",
                "[1,1]",
                "[1,[1]]",
                "[[],[]]",
                "[[],[8],[1],[[[4,10,8,1]],8]]",
        };
        for (auto s: test_strings) {
            if (mixed_list::parse(s).to_string() != s) {
                std::cout << s << ' ' << mixed_list::parse(s).to_string() << '\n';
            }
        }
    }
    std::vector<mixed_list> packets;

    std::string s;
    while (std::cin >> s) {
        assert(s == mixed_list::parse(s).to_string());
        packets.push_back(mixed_list::parse(s));
    }

    std::cout << "Part one: " << part_one(packets) << '\n';
    std::cout << "Part two: " << part_two(packets) << '\n';
}