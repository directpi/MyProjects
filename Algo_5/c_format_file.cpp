#include <iostream>
#include <vector>

using namespace std;

int main() {
    int n = 0;
    long x = 0;
    cin >> n;
    vector<int> spases(n);
    for (int i = 0; i < n; ++i) {
        cin >> spases[i];
    }
    for (int i = 0; i < n; ++i) {
        if (spases[i] != 0) {
            if (spases[i] == 1 || spases[i] == 4) {
                ++x;
            } else if (spases[i] % 4 == 0) {
                x += abs(spases[i]) / 4;
            } else if (spases[i] == 3) {
                x += 2;
            } else {
                x += abs(spases[i]) / 4;
                if (abs(spases[i]) % 4 == 3) {
                    x += 2;
                } else {
                    x += abs(spases[i]) % 4;
                }
            }
        }
    }
    cout << x << "\n";
}
