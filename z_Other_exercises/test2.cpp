#include <iostream>
#include <vector>

int main() {
    const size_t m = 1000000;
    const size_t n = 10000;
    std::vector<std::string> v(m);
    for (auto& row : v) {  // обычная ссылка
        row.resize(n, '@');
    }

    size_t sum = 0;
    for (const auto& row : v) {  // константная ссылка
        sum += row.size();
    }
    std::cout << sum << "\n";
}