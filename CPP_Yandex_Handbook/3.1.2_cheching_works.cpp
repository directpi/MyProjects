#include <deque>
#include <iostream>
#include <string>
#include <vector>

int main() {
    int n, m;
    std::cin >> n;
    std::deque<std::string> d;
    std::string str1, str2;
    for (int i = 0; i < n; ++i) {
        std::cin >> str1 >> str2;
        if (str2 == "top") (d.push_front(str1));
        if (str2 == "bottom") (d.push_back(str1));
    }
    std::cin >> m;
    std::vector<int> numbers(m);
    for (int i = 0; i < m; ++i) {
        std::cin >> numbers[i];
    }
    for (size_t j = 0; j < numbers.size(); ++j) {
        std::cout << d[numbers[j] - 1] << "\n";
    }
}