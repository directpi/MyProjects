#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
    int n, m;

    string str, str2;
    cin >> n >> m;
    vector<int> array(n);
    vector<int> res(n);
    vector<int> razryad(n + 1);

    for (int i = 0; i < n; ++i) {
        cin >> array[i];

        str += to_string(array[i]) + "$";
        razryad[i + 1] = str.length();  //  + ceil(log10(array[i] + 1));
    }

    long int p = pow(10, 9) + 7;
    int x_ = 257;

    size_t k = str.length();  //
    cout << str << "\n";
    vector<size_t> h(k + 1);
    vector<size_t> x(k + 1);
    x[0] = 1;
    h[0] = 0;
    cout << "\n"
         << "h="
         << "\t";
    for (size_t i = 1; i < k + 1; ++i) {
        h[i] = ((h[i - 1] * x_ + (int)str[i]) % p);
        cout << h[i] << "\t";
        x[i] = (x[i - 1] * x_) % p;
    }
    //  // }
    // str2 = str.substr(str.size() / 2);
    // razr = str.length() / n;
    cout << "\n"
         << "h1="
         << "\t";
    str2 = str;
    reverse(str.begin(), str.end());
    str2 = str + str2;
    cout << "\n" << str2 << "\n";
    size_t k1 = str2.length();
    vector<size_t> h1(k1 + 1);
    h1[0] = 0;
    for (size_t j = 1; j < k1 + 1; ++j) {
        h1[j] = ((h1[j - 1] * x_ + (int)str2[j]) % p);
        cout << h1[j] << "\t";
    }
    cout << "\n";
    // int index1 = 1, index2 = 1;
    // int r1 = abs((long)(h[index1 + n - 1 - n + 1] - h[index1 - 1] * pow(x[index1], n - n + 1)) % p);
    // int r12 = abs((long)(h1[index2 + n * 2 * razr - 1] - h1[index2 - 1] * pow(x[index2], n * 2 * razr)) %
    // p); cout << r1 << "\t" << r12 << "\n"; cout << r12 - r1 << "\t" << r1 - r12; while (index2 >= 3) {
    //     if (h1[index2] - h1[index1] == h[index2 - index1])
    //         ;
    // }
    // string str3;
    // if (n == 1) {
    //     res[0] = n;
    // }

    // for (size_t i = 0; i < n + 1; ++i) {
    //     str3 = " " + str2.substr((str2.size() / 2) - i, n * razr);

    //     if (str3 == str) {
    //         res[i] = str2.size() / (2 * razr);
    //     }
    //     str2.erase(str2.size() / 2 - 1, razr);
    //     str2.erase(str2.size() / 2, razr);
    //     if ((int)str2.size() < n) {
    //         break;
    //     }
    // }
    // for (int k = n - 1; k >= 0; --k) {
    //     if (res[k] != 0) {
    //         cout << res[k];
    //         if (k >= 1) {
    //             cout << " ";
    //         }
    //     }
    // }
}