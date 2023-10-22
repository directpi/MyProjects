#include <iostream>
int main() {
    int n = 0;
    double s = 0, z = 1;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        s += z * 1 / (1 + i);
        z *= -1;
    }
    std::cout << s;
}