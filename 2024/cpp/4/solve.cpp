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
    int n = 0;
    for (int c; (c = gc()) != EOF;) {
        input_str.push_back(c);
        n += c == '\n';
    }
    string_view input = input_str;

    int m = input.size() / n;
    int ans1 = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            for (int di = -1; di <= 1; ++di) {
                for (int dj = -1; dj <= 1; ++dj) {
                    string buf;
                    if (!di && !dj) continue;
                    int ni = i, nj = j;
                    for (int k = 0; k < 4; ++k) {
                        if (0 <= ni && ni < n && 0 <= nj && nj < m) {
                            buf.push_back(input[ni * m + nj]);
                        }
                        ni += di, nj += dj;
                    }
                    ans1 += buf == "XMAS";
                }
            }
        }
    }
    cout << ans1 << '\n';
    int ans2 = 0;
    for (int i = 1; i + 1 < n; ++i) {
        for (int j = 1; j + 1 < m; ++j) {
            if (input_str[i  * m + j] == 'A') {
                char tl = input_str[(i - 1) * m + j - 1];
                char tr = input_str[(i - 1) * m + j + 1];
                char bl = input_str[(i + 1) * m + j - 1];
                char br = input_str[(i + 1) * m + j + 1];
                if (tl < br) swap(tl, br);
                if (bl < tr) swap(bl, tr);
                ans2 += tl == 'S' && br == 'M' && bl == 'S' && tr == 'M';
            }
        }
    }
    cout << ans2 << '\n';
}
