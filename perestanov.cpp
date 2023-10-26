#include <iostream>
int factorial(int n) {
    if (n == 0) {
        return 1;
    }
    return n * factorial(n - 1);
}
int main() {
    int n = 0;
    std::cin >> n;
    std::cout << factorial(n);
}