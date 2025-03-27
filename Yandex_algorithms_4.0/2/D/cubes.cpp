#include <algorithm>
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
    int razr = 0;
    for (int i = 0; i < n; ++i) {
        cin >> array[i];
        str += to_string(array[i]);
    }
    // for (int i = n - 1; i >= 0; --i) {
    //     str2 += to_string(array[i]);
    // }
    razr = str.length() / n;
    str2 = str + str;
    // cout << "\n" << str2;
    string str3;
    if (n == 1) {
        res[0] = n;
    }
    // int f = 0;
    // if (str.substr(0, n / 2) == str.substr(str.size() / 2, n / 2)) {
    //     f = 1;
    // }
    for (int i = 0; i <= n - 1; ++i) {
        str3 = str2.substr((str2.size() / 2) - i * razr, n * razr);
        if (str3 == str) {  //&& f == 0
            res[i] = str2.size() / (2 * razr);
        }
        str2.erase(str2.size() / 2 - 1, razr);
        str2.erase(str2.size() / 2, razr);
        if ((int)str2.size() < n) {
            break;
        }
    }
    for (int k = n - 1; k >= 0; --k) {
        if (res[k] != 0) {
            cout << res[k];
            if (k >= 1) {
                cout << " ";
            }
        }
    }
}