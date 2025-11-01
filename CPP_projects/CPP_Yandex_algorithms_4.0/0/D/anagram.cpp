#include <algorithm>
#include <iostream>

int main() {
    std::string str1;
    std::string str2;

    std::getline(std::cin, str1);
    std::getline(std::cin, str2);
    std::sort(str1.begin(), str1.end());
    std::sort(str2.begin(), str2.end());
    if (str1 == str2) {
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }
}