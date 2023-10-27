#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> Split(const std::string& str, char delimiter) {
    std::vector<std::string> strings;
    size_t j = 0;
    for (size_t i = 0; i != str.size(); ++i) {
        if (str[i] == delimiter) {
            strings.push_back(str.substr(j, i - j));
            j = i + 1;
        }
    }
    strings.push_back(str.substr(j));
    return strings;
}
int main() {
    std::string str;
    char del;
    std::getline(std::cin, str);
    std::cin >> del;
    for (size_t i = 0; i != Split(str, del).size(); ++i) {
        std::cout << Split(str, del)[i] << "\n";
    }
}