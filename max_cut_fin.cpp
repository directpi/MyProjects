// #include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
// auto start = chrono::steady_clock::now();

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, cnt_res;
    ifstream file("input.txt");
    if (file.is_open()) {
        string line;
        getline(file, line);
        n = atoi(line.c_str());
        vector<vector<pair<int, int>>> p(n + 1);
        long e = pow(2, n);
        vector<vector<int>> mask(e / 2, vector<int>(n));
        vector<int> sum_e_v(n + 1);

        for (int i = 0; i < e / 2; ++i) {
            for (int j = 0; j <= n; ++j) {
                mask[i][n - j - 1] = ((i >> j) & 1) == 1;
            }
        }
        int mt = 0, j = 1;
        while (getline(file, line)) {
            int i = 1;
            istringstream iss(line);
            string token;

            p[j].push_back(make_pair(0, 0));
            while (getline(iss, token, ' ')) {
                mt = atoi(token.c_str());
                if (mt != 0) {
                    p[j].push_back(make_pair(mt, i));
                    sum_e_v[j] += mt;
                } else {
                    p[j].push_back(make_pair(0, 0));
                }
                ++i;
            }
            ++j;
        }
        int res = 0;
        cnt_res = 0;

        vector<int> st;
        for (int k = 1; k < e / 2; ++k) {  //  int k = e / 2 - 1; k > 0; --k
            int tmp = 0;
            for (int i = 0; i < n; ++i) {
                if (mask[k][i] == 1) {
                    st.push_back(i + 1);
                    tmp += sum_e_v[i + 1];
                }
            }
            while (!st.empty()) {
                for (size_t m = 1; m < st.size(); ++m) {
                    tmp -= 2 * p[st[0]][st[m]].first;
                    if (tmp < res) {
                        break;
                    }
                }
                if (!st.empty()) st.erase(st.begin());
            }
            if (res < tmp) {
                res = tmp;
                cnt_res = k;
            }
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
    } else {
        cout << "Не удалось открыть файл." << endl;
    }
    // ofstream out("output.txt");
    // auto end = chrono::steady_clock::now();
    // auto diff = end - start;
    // cout << chrono::duration<double, milli>(diff).count() << " ms" << endl;
    file.close();
    // out.close();
}
