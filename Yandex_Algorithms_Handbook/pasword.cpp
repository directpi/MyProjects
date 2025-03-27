#include <iostream>
#include <string>

int main() {
    int c1 = 0, c2 = 0, c3 = 0, c4 = 0;
    std::string password;
    std::getline(std::cin, password);
    for (char c : password) {
        if (c == 32) {
            c1 = 0;
            c2 = 0;
            c3 = 0;
            c4 = 0;
            break;
        }
        if ((static_cast<int>(c) >= 33 && static_cast<int>(c) <= 47) ||
            (static_cast<int>(c) >= 58 && static_cast<int>(c) <= 64) ||
            (static_cast<int>(c) >= 91 && static_cast<int>(c) <= 96) ||
            (static_cast<int>(c) >= 123 && static_cast<int>(c) <= 126)) {
            ++c1;
        } else if (static_cast<int>(c) >= 48 && static_cast<int>(c) <= 57) {
            ++c2;
        } else if (static_cast<int>(c) >= 65 && static_cast<int>(c) <= 90) {
            ++c3;
        } else if (static_cast<int>(c) >= 97 && static_cast<int>(c) <= 122) {
            ++c4;
        }
    }
    if (((c1 >= 1 && c2 >= 1 && c3 >= 1) || (c1 >= 1 && c2 >= 1 && c4 >= 1) ||
         (c1 >= 1 && c3 >= 1 && c4 >= 1) || (c2 >= 1 && c3 >= 1 && c4 >= 1)) &&
        password.size() >= 8 && password.size() <= 14) {
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }
}