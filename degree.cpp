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
    double arr[100];
    int n = 0;
    int s = 0;

    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
        s += arr[i];
    }
    // std::cout << s << "\n";
    double d = degree(s);
    if ((pow(2, d)) < s) {
        std::cout << d + 1 << "\n";
        // std::cout << pow(2, d + 1);
    } else {
        std::cout << d << "\n";
        // std::cout << pow(2, d);
    }
}
