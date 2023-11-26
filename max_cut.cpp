#include <chrono>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;
auto start = chrono::steady_clock::now();
// void max_cat(vector<vector<int>>& mask, int n, vector<vector<pair<int, int>>>& p, int& res, int e,
//              int& cnt_res) {
//     for (int k = 1; k < e / 2; ++k) {
//         int tmp = 0;  //, sum = 0;
//         // for (int ii = 0; ii < n; ++ii) sum += mask[k][ii];
//         vector<int> st;
//         // if (sum != n) {
//         for (int i = 0; i < n; ++i) {
//             if (mask[k][i] == 1) {
//                 st.push_back(i + 1);
//                 for (size_t j = 0; j != p[i + 1].size(); ++j) {
//                     tmp += p[i + 1][j].first;
//                 }
//             }
//         }
//         while (!st.empty()) {
//             for (size_t m = 1; m < st.size(); ++m) {
//                 for (size_t f = 0; f < p[st[0]].size(); ++f) {
//                     if (p[st[0]][f].second == st[m]) {
//                         tmp -= 2 * p[st[0]][f].first;
//                     }
//                 }
//             }
//             if (!st.empty()) st.erase(st.begin());
//         }
//         if (!st.empty()) st.erase(st.begin());
//         if (res < tmp) {
//             res = tmp;
//             cnt_res = k;
//         }
//         // }
//     }
// }
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, cnt_res;
    cin >> n;
    vector<vector<pair<int, int>>> p(n + 1);
    long e = pow(2, n);
    vector<vector<int>> mask(e / 2, vector<int>(n));
    int mt = 0;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            cin >> mt;
            if (mt != 0) {
                p[i].push_back(make_pair(mt, j));
            }
        }
    }
    for (int i = 0; i < e / 2; ++i) {
        for (int j = 0; j <= n; ++j) {
            mask[i][n - j - 1] = ((i >> j) & 1) == 1;
        }
    }
    int res = 0;
    cnt_res = 0;
    // max_cat(mask, n, p, res, e, cnt_res);
    for (int k = 1; k < e / 2; ++k) {
        int tmp = 0;  //, sum = 0;
        // for (int ii = 0; ii < n; ++ii) sum += mask[k][ii];
        vector<int> st;
        // if (sum != n) {
        for (int i = 0; i < n; ++i) {
            if (mask[k][i] == 1) {
                st.push_back(i + 1);
                for (size_t j = 0; j != p[i + 1].size(); ++j) {
                    tmp += p[i + 1][j].first;
                }
            }
        }
        while (!st.empty()) {
            for (size_t m = 1; m < st.size(); ++m) {
                for (size_t f = 0; f < p[st[0]].size(); ++f) {
                    if (p[st[0]][f].second == st[m]) {
                        tmp -= 2 * p[st[0]][f].first;
                    }
                }
            }
            if (!st.empty()) st.erase(st.begin());
        }
        if (!st.empty()) st.erase(st.begin());
        if (res < tmp) {
            res = tmp;
            cnt_res = k;
        }
        // }
    }
    cout << res << "\n";
    for (int i = 0; i < n; ++i) {
        if (mask[cnt_res][i] == 0) {
            cout << "2 ";
        } else {
            cout << "1 ";
        }
    }
    cout << "\n";
    auto end = chrono::steady_clock::now();
    auto diff = end - start;
    cout << chrono::duration<double, milli>(diff).count() << " ms" << endl;
}
