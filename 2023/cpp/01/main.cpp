#include <cctype>
#include <iostream>
#include <optional>

using namespace std;

int main() {
    string s;
    int ans = 0;
    auto is_digit = [](string s) -> optional<int> {
        if (isdigit(s.back())) return s.back() - '0';
        if (s.size() >= 3 && s.substr(s.size() - 3) == "one") return 1;
        if (s.size() >= 3 && s.substr(s.size() - 3) == "two") return 2;
        if (s.size() >= 5 && s.substr(s.size() - 5) == "three") return 3;
        if (s.size() >= 4 && s.substr(s.size() - 4) == "four") return 4;
        if (s.size() >= 4 && s.substr(s.size() - 4) == "five") return 5;
        if (s.size() >= 3 && s.substr(s.size() - 3) == "six") return 6;
        if (s.size() >= 5 && s.substr(s.size() - 5) == "seven") return 7;
        if (s.size() >= 5 && s.substr(s.size() - 5) == "eight") return 8;
        if (s.size() >= 4 && s.substr(s.size() - 4) == "nine") return 9;
        cerr << "fail: " << s << '\n';
        return nullopt;
    };
    while (cin >> s) {
        int a;
        string st;
        for (auto i: s) {
            st.push_back(i);
            if (auto o = is_digit(st); o) {
                ans += o.value() * 10;
                st.clear();
                break;
            }
            if (st.size() > 5) st = st.substr(1);
        }
        for (auto i: s) {
            st.push_back(i);
            if (auto o = is_digit(st); o) {
                cerr << "success: " << st << '\n';
                a = o.value();
            }
            if (st.size() > 5) st = st.substr(1);
        }
        ans += a;
    }
    cout << ans << '\n';
}
