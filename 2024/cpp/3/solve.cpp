#include <algorithm>
#include <ios>
#include <iostream>

using namespace std;


char buf[1 << 20];
size_t buf_start = 0, buf_end = 0;

int gc() {
    if (buf_start == buf_end) {
        buf_start = 0;
        buf_end = fread(buf, 1, sizeof(buf), stdin);
        if (feof(stdin) and buf_end == 0) return EOF;
    }
    return buf[buf_start++];
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string input_str;
    for (int c; (c = gc()) != EOF;) {
        input_str.push_back(c);
    }
    string_view input = input_str;

    int ans1 = 0;
    for (size_t idx = input.find("mul("); idx != string::npos; idx = input.find("mul(", idx + 1)) {
        size_t paren = input.find(')', idx);
        auto to_consider = input.substr(idx, paren - idx + 1);

        int i = 4;
        int first = 0;
        int second = 0;
        for (; '0' <= to_consider[i] and to_consider[i] <= '9'; ++i) {
            first = first * 10 + to_consider[i] - '0';
        }
        if (to_consider[i] != ',') continue;
        ++i;
        for (; '0' <= to_consider[i] and to_consider[i] <= '9'; ++i) {
            second = second * 10 + to_consider[i] - '0';
        }
        if (to_consider[i] != ')') continue;
        ans1 += first * second;
    }
    cout << ans1 << '\n';
    int ans2 = 0;
    int enabled = 1;
    for (size_t idx = min({input.find("mul("), input.find("do()"), input.find("don't()")}); idx != string::npos; idx = min({input.find("mul(", idx + 1), input.find("do()", idx + 1), input.find("don't()", idx + 1)})) {
        size_t paren = input.find(')', idx);
        auto to_consider = input.substr(idx, paren - idx + 1);

        if (to_consider == "do()") {
            enabled = 1;
            continue;
        }

        if (to_consider == "don't()") {
            enabled = 0;
            continue;
        }

        int i = 4;
        int first = 0;
        int second = 0;
        for (; '0' <= to_consider[i] and to_consider[i] <= '9'; ++i) {
            first = first * 10 + to_consider[i] - '0';
        }
        if (to_consider[i] != ',') continue;
        ++i;
        for (; '0' <= to_consider[i] and to_consider[i] <= '9'; ++i) {
            second = second * 10 + to_consider[i] - '0';
        }
        if (to_consider[i] != ')') continue;
        ans2 += first * second * enabled;
    }
    cout << ans2 << '\n';
}
