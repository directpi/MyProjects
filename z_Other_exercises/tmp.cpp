#include <cmath>
#include <iostream>
using namespace std;
int main() {
    int n, res;
    cin >> n;
    res = ceil(log10(n + 1));
    cout << res;
}