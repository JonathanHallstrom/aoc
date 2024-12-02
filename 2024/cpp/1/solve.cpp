#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    map<int, int> leftm, rightm;
    vector<int> left, right;
    for (int l, r; cin >> l >> r;) {
        left.push_back(l);
        leftm[l]++;
        right.push_back(r);
        rightm[r]++;
    }

    sort(begin(left), end(left));
    sort(begin(right), end(right));
    int ans1 = 0;
    for (int i = 0; i < left.size(); ++i)
        ans1 += abs(left[i] - right[i]);
    cout << ans1 << '\n';
    int ans2 = 0;
    for (auto i: left) ans2 += i * rightm[i];
    cout << ans2 << '\n';
}
