#include <iostream>
#include <vector>
using namespace std;
int main() {
    int g1 = 0, g2 = 0, g3 = 0, g4 = 0, n = 0, x = 0;
    int sum1 = 0, sum2 = 0;
    char c;
    cin >> g1 >> c >> g2;
    cin >> g3 >> c >> g4;
    cin >> n;
    sum1 = g1 + g3;
    sum2 = g2 + g4;

    if (sum1 > sum2) {
        x = 0;
    } else if (sum1 == sum2 && n == 2) {
        if (g1 > g4) {
            x = 0;
        } else {
            ++x;
        }
    } else if (sum1 == sum2 && n == 1) {
        if (g3 > g2) {
            x = 0;
        } else {
            ++x;
        }
    } else {
        if (n == 2) {
            x = sum2 - sum1;
            if ( g1 <= g2) {
                ++x;
            }
        } else if (n == 1) {
            x = sum2 - sum1;
            if ((x == g2 && g1 == g4) || (x + g3 < g4) ) {
                ++x;
            }
        }
    }
    cout << x << "\n";
}