#include <iostream>

int main() {
    std::string str1, str2;
    std::getline(std::cin, str1);
    std::getline(std::cin, str2);
    if (str1.length() > str2.length()) {
        int k = str1.length() - str2.length();
        for (int i = 0; i < k; ++i) {
            str2.insert(0, "0");
        }
        str2.resize(str1.size());
    } else if (str1.length() < str2.length()) {
        int k = str2.length() - str1.length();
        for (int i = 0; i < k; ++i) {
            str1.insert(0, "0");
        }
        str1.resize(str2.size());
    }
    for (size_t i = 0; i < str1.length(); ++i) {
        if ((str2[i] > str1[i])) {
            str1[i] = str2[i];
            std::cout << str1[i];
        } else {
            std::cout << str1[i];
        }
    }
}