#include <iostream>
#include <vector>
using namespace std;
int main() {
    int n;
    cin >> n;
    vector<size_t> a(n + 1);
    vector<size_t> b(n + 1);
    vector<size_t> c;
    size_t i = 1, j = 1;

    for (int k = 0; k <= n; ++k) {
        a[k] = k * k;
        b[k] = k * k * k;
    }
    int count = 1;
    int d = 1;
    c.push_back(1);
    while (count < n) {
        if (i * i <= j * j * j) {
            if (!c.empty() && i * i != c[count - 1]) {
                c.push_back(i * i);
                ++count;
            }
            ++i;
        } else {
            if (!c.empty() && j * j * j != c[count - 1] && a[j * d] != b[j]) {
                c.push_back(j * j * j);
                ++count;
            }
            if (a[j * d] == b[j]) ++d;
            ++j;
        }
    }
    int tmp = c.back();
    cout << tmp;
}