#include <cmath>
#include <iostream>

double degree(int s) {
    int counter = 0;
    while (s != 1) {
        s = s / 2;
        ++counter;
    }
    return (counter);
}
int main() {
    int n = 0;
    std::cin >> n;
    if (n > 0) {
        double arr[100];
        int s = 0;
        for (int i = 0; i < n; ++i) {
            std::cin >> arr[i];
            s += arr[i];
        }
        double d = degree(s);
        if ((pow(2, d)) < s) {
            std::cout << d + 1 << "\n";
        } else {
            std::cout << d << "\n";
        }
    } else {
        return 1;
    }
}