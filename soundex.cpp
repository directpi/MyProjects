#include <iostream>
#include <string>
int main() {
    std::string str1, str2;
    // int count = 0;
    std::getline(std::cin, str1);
    // std::getline(std::cin, str2);
    for (size_t i = 0; i != str1.size(); ++i) {
        char c = str1[i];
        if (i != 0 &&
            (c == 'a' || c == 'e' || c == 'h' || c == 'i' || c == 'o' || c == 'u' || c == 'w' || c == 'y')) {
            std::cout << i << "\t" << str1[i] << "\t";
            str1.erase(i, 1);
            //     ++d;
            // } else {
            //     d = 0;
            // }
            // if (d > 0) {
            //     --count;
            // } else {
            //     ++count;
        }
        std::cout << i << "_\n";
    }
    std::cout << "\n";
    // count = 0;
    // for (char c : str2) {
    //     if (count != 0 &&
    //         (c == 'a' || c == 'e' || c == 'h' || c == 'i' || c == 'o' || c == 'u' || c == 'w' || c == 'y'))
    //         { str2.erase(count, 1); std::cout << count << "\t";
    //         ++count;
    //     }
    // }
    std::cout << str1 << "\n";
    // std::cout << str2 << "\n";
}