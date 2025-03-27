#include <cstdint>
#include <iostream>

std::uint64_t Factorial(std::uint64_t n) {
    if (n == 0) {
        return 1;
    }
    return n * Factorial(n - 1);  // рекурсивный вызов
}

int main() {
    std::cout << Factorial(10) << "\n";  // 120
}