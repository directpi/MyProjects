#include <iostream>
#include <string>
int main() {
    std::string str1;
    std::getline(std::cin, str1);
    // if (str1.size() > 20) {
    //     exit(1);
    // }
    for (size_t i = 0; i != str1.size(); ++i) {
        char c = str1[i];
        if (i != 0 &&
            (c == 'a' || c == 'e' || c == 'h' || c == 'i' || c == 'o' || c == 'u' || c == 'w' || c == 'y')) {
            str1.erase(i, 1);
            --i;
        }
    }
    // std::cout << str1 << " -> ";
    for (size_t i = 0; i < str1.size(); ++i) {
        char c = str1[i];
        if (i != 0 && (c == 'b' || c == 'f' || c == 'p' || c == 'v')) {
            str1.replace(i, 1, "1");
        } else if (i != 0 && (c == 'c' || c == 'g' || c == 'j' || c == 'k' || c == 'q' || c == 's' ||
                              c == 'x' || c == 'z')) {
            str1.replace(i, 1, "2");
        } else if (i != 0 && (c == 'd' || c == 't')) {
            str1.replace(i, 1, "3");
        } else if (i != 0 && (c == 'l')) {
            str1.replace(i, 1, "4");
        } else if (i != 0 && (c == 'm' || c == 'n')) {
            str1.replace(i, 1, "5");
        } else if (i != 0 && (c == 'r')) {
            str1.replace(i, 1, "6");
        }
    }
    // std::cout << str1 << " -> ";
    for (size_t i = 0; i < str1.size() - 1; ++i) {
        if (str1[i] == str1[i + 1]) {
            str1.erase(i + 1, 1);
            i--;
        }
    }
    // std::cout << str1 << " -> ";
    if (str1.size() < 4) {
        for (size_t i = str1.size(); i < 4; ++i) {
            str1.insert(i, "0");
        }
    } else {
        str1.resize(4);
    }
    std::cout << str1;
}