#include <iostream>

int main() {
    int n = 0, s = 0;
    std::cin >> n;
    while (n > 0) {
        s += n % 10;
        n = n / 10;
    }
    std::cout << s;
}