#include <algorithm>
#include <cassert>
#include <ios>
#include <iostream>
#include <map>
#include <set>
#include <vector>

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

    string line;
    map<int, set<int>> must_come_after, must_come_before;
    vector<vector<int>> updates;
    while (cin >> line) {

        if (size_t pipe_idx = line.find('|'); pipe_idx != string::npos) {
            int first = stoi(line.substr(0, pipe_idx));
            int second = stoi(line.substr(pipe_idx + 1));
            must_come_after[first].insert(second);
            must_come_before[second].insert(first);
        } else {
            vector<int> update;
            for (size_t i = 0, j = line.find(','); i != string::npos; i = j, j = line.find(',', j + 1)) {
                i += line[i] == ',';
                update.push_back(stoi(line.substr(i, j - i)));
            }
            updates.push_back(update);
            assert(update.size() % 2);
        }
    }

    int ans1 = 0;
    int ans2 = 0;
    for (auto update: updates) {
        int n = update.size();
        bool correct = true;
        for (int i = 0; i < n && correct; ++i) {
            for (int j = 0; j < i && correct; ++j) {
                // j < i
                // if i must come before j then incorrect
                if (must_come_before[update[j]].count(update[i])) {
                    correct = false;
                }
            }
        }

        if (correct) {
            ans1 += update[n / 2];
        } else {
            sort(begin(update), end(update), [&](int lhs, int rhs) {
                return must_come_before[lhs].count(rhs) == 0;
            });
            ans2 += update[n / 2];
        }
    }
    cout << ans1 << '\n';
    cout << ans2 << '\n';
}
