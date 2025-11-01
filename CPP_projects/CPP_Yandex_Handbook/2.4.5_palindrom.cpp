#include <iostream>
#include <string>

int main() {
    std::string str;
    int f = 0;
    std::getline(std::cin, str);
    for (size_t i = 0; i < str.size() - 1; ++i) {
        if (str[i] == ' ') {
            str.erase(i, 1);
            i--;
        }
    }
    size_t j = str.size() - 1;
    for (size_t i = 0; i != str.size(); ++i) {
        if (str[i] == str[j]) {
            --j;
        } else {
            f = 1;
            break;
        }
    }
    if (f == 0) {
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }
}