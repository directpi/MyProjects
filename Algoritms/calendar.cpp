#include <iostream>

int main() {
    int n, k, d;
    std::cin >> n >> k;

    int i = 1;
    for (i = 1; i < n; ++i) {
        std::cout << "   ";
    }
    d = n;
    for (int j = 1; j <= k; ++j) {
        if (j < 10) {
            std::cout << " ";
        }
        std::cout << j;

        if (d == 7) {
            std::cout << "\n";
            d = 1;
        } else {
            std::cout << " ";
            d += 1;
        }
    }
    if (d != 1) {
        std::cout << "\n";
    }
}