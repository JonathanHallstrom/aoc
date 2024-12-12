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

bool loops(vector<string> const &board, int start_i, int start_j) {
    int n = board.size();
    int m = board[0].size();
    int di = -1, dj = 0;
    int ci = start_i, cj = start_j;
    int idx = 0;

    while (idx++ < (1 << 15)) {
        ci += di, cj += dj;
        // exited area
        if (!(0 <= ci and ci < n and 0 <= cj and cj < m)) break;
        if (board[ci][cj] == '#') {
            ci -= di, cj -= dj;
            int ndi = dj, ndj = -di;
            di = ndi, dj = ndj;
            continue;
        }
    }
    return idx >= (1 << 15);
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string line;
    vector<string> board;
    int start_i, start_j;
    while (cin >> line) {
        size_t idx = line.find('^');
        if (idx != string::npos) {
            start_i = board.size();
            start_j = idx;
        }
        if (line.size() > 0) {
            board.push_back(line);
        }
    }
    int n = board.size();
    int m = board[0].size();


    int di = -1, dj = 0;
    int ci = start_i, cj = start_j;
    auto board_copy = board;
    while (true) {
        board_copy[ci][cj] = '%';
        ci += di, cj += dj;
        // exited area
        if (!(0 <= ci and ci < n and 0 <= cj and cj < m)) break;
        if (board_copy[ci][cj] == '#') {
            ci -= di, cj -= dj;
            int ndi = dj, ndj = -di;
            di = ndi, dj = ndj;
            continue;
        }
    }

    int ans1 = 0;
    for (auto row: board_copy) {
        // cout << row << '\n';
        ans1 += count(begin(row), end(row), '%');
    }

    cout << ans1 << '\n';
    board_copy = board;
    int ans2 = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (board[i][j] == '#') continue;
            board[i][j] = '#';
            ans2 += loops(board, start_i, start_j);
            board[i][j] = '.';
        }
    }
    for (auto row: board_copy) {
        // cout << row << '\n';
    }
    cout << ans2 << '\n';
}
