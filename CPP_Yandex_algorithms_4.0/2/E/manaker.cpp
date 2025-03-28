#include <iostream>
#include <vector>
using namespace std;
int main() {
    string s1;
    cin >> s1;
    int n = s1.length();
    vector<int> d1(n);
    int l = 0, r = -1;
    int count = 0;
    for (int i = 0; i < n; ++i) {
        int k = i > r ? 1 : min(d1[l + r - i], r - i + 1);
        while (i + k < n && i - k >= 0 && s1[i + k] == s1[i - k]) {
            ++k;
        }
        d1[i] = k;
        count += k;
        if (i + k - 1 > r) l = i - k + 1, r = i + k - 1;
    }
    vector<int> d2(n);
    l = 0, r = -1;
    for (int i = 0; i < n; ++i) {
        int k = i > r ? 0 : min(d2[l + r - i + 1], r - i + 1);
        while (i + k < n && i - k - 1 >= 0 && s1[i + k] == s1[i - k - 1]) ++k;
        d2[i] = k;
        count += k;
        if (i + k - 1 > r) l = i - k, r = i + k - 1;
    }
    cout << count;
}

