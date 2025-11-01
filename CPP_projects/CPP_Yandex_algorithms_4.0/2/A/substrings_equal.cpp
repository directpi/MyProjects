#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
int main() {
    string str;
    int q;
    size_t p = pow(10, 9) + 7;
    int x_ = 257;  // 10
    getline(cin, str);
    str = " " + str;
    int n = str.length();
    cin >> q;
    vector<vector<int>> arr(q, vector<int>(q));
    vector<size_t> h;
    vector<size_t> x;

    for (int i = 0; i < q; ++i) {
        for (int j = 0; j < 3; ++j) {
            int tmp;
            cin >> tmp;
            arr[i][j] = tmp;
        }
    }
    h.push_back(0);
    x.push_back(1);
    for (int i = 1; i < n + 1; i++) {
        h.push_back((h[i - 1] * x_ + str[i] - 49 + 1) % p);
        x.push_back((x[i - 1] * x_) % p);
    }

    int from1 = 0, from2 = 0, len = 0;

    for (int k = 0; k < q; ++k) {
        if (arr[k][1] > arr[k][2]) {
            from1 = arr[k][2];
            from2 = arr[k][1];
        } else {
            from2 = arr[k][2];
            from1 = arr[k][1];
        }
        len = arr[k][0];
        if ((h[from1 + len] + h[from2] * x[len]) % p == (h[from2 + len] + h[from1] * x[len]) % p) {
            cout << "yes"
                 << "\n";
        } else {
            cout << "no"
                 << "\n";
        }
    }
}