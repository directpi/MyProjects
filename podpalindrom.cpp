#include <string.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

// size_t isEqual(size_t from1, size_t from2, size_t len, vector<size_t>& h, vector<size_t>& h2,
//                vector<size_t>& x, size_t p) {
//     if ((h[from1 + len - 1] + h2[from2 - 1] * x[len]) % p ==
//         (h2[from2 + len - 1] + h[from1 - 1] * x[len]) % p)
//         return 1;
//     else
//         return 2;
// }

int main() {
    vector<size_t> h;
    vector<size_t> h2;
    vector<size_t> x;

    string str;
    cin >> str;

    string str2;
    str2 = str;
    reverse(str2.begin(), str2.end());
    size_t n = str.length();
    size_t p = pow(10, 9) + 7;
    int x_ = 257;
    if (str.length() == 1)
        cout << 1;
    else {
        x.push_back(1);
        h.push_back(0);
        h2.push_back(0);
        for (size_t i = 1; i < n + 1; i++) {
            h.push_back((h[i - 1] * x_ + (str[i - 1])) % p);
            h2.push_back((h2[i - 1] * x_ + (str2[i - 1])) % p);
            x.push_back((x[i - 1] * x_) % p);
        }
        size_t count = n;
        for (size_t ii = 2; ii < n + 1; ii++) {
            for (size_t j = 0; j < n - ii + 1; j++) {
                // size_t res = isEqual(j + 1, n - j - ii + 1, ii, h, h2, x, p);
                if ((h[j + 1 + ii - 1] + h2[n - j - ii + 1 - 1] * x[ii]) % p ==
                    (h2[n - j - ii + 1 + ii - 1] + h[j + 1 - 1] * x[ii]) % p) {
                    count++;
                }
            }
        }
        cout << count;
    }
    return 0;
}