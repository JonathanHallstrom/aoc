#include <algorithm>
#include <array>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

using namespace std;

bool is_safe(vector<int> report) {

    bool result = true;
    result &= is_sorted(begin(report), end(report)) or is_sorted(rbegin(report), rend(report));
    int last = report[0] + 1;
    for (auto i: report) {
        int d = abs(i - last);
        result &= 1 <= d and d <= 3;
        last = i;
    }
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<vector<int>> reports;
    string line;
    while (getline(cin, line)) {
        istringstream iss(line);
        vector<int> report;
        for (int i; iss >> i;) report.push_back(i);
        reports.push_back(report);
    }
    int ans1 = 0;
    for (auto report: reports) {
        ans1 += is_safe(report);
    }
    cout << ans1 << '\n';
    int ans2 = 0;
    for (auto report: reports) {
        bool safe = is_safe(report);

        for (int to_remove = 0; to_remove < report.size(); ++to_remove) {
            auto reduced_report = report;
            reduced_report.erase(reduced_report.begin() + to_remove);
            safe |= is_safe(reduced_report);
        }
        ans2 += safe;
    }
    cout << ans2 << '\n';
}
