#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> strings;
    std::string str;
    while (std::getline(std::cin, str)) {
        // if (str.empty()) {
        //     break;
        // } else {
            strings.push_back(str);
        // }
    }
    std::sort(strings.rbegin(), strings.rend());
    for (size_t i = 0; i != strings.size(); ++i) {
        std::cout << strings[i] << "\n";
    }
}