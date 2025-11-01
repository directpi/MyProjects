#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

void z_function(string str) {
    int n = (int)str.length();
    vector<int> z(n);
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        if (i <= r) z[i] = min(r - i + 1, z[i - l]);

        while (i + z[i] < n && str[z[i]] == str[i + z[i]]) ++z[i];
        if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
    }
    for (int k = 0; k < n; ++k) {
        cout << z[k] << "\n";
    }
}

int main() {
    string str;
    vector<int> z(string str);
    getline(cin, str);
    z_function(str);
}