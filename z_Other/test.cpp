#include <iostream>
#include <vector>

int main() {
    // создаём вектор из m строк длины n
    // и искусственно заполняем его:
    const size_t m = 1000000;
    const size_t n = 10000;
    std::vector<std::string> v(m);
    for (size_t i = 0; i != m; ++i) {
        v[i].resize(n, '@');  // кладём в вектор строку из n символов @
    }

    // нам интересен этот цикл:
    size_t sum = 0;
    for (auto row : v) {
        sum += row.size();
    }
    std::cout << sum << "\n";
}