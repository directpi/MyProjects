#include <iostream>
#include <string>
#include <vector>
std::string_view Temp(const std::string_view str1, const std::string_view str2) {
    size_t i = 0;
    while (i != str1.size() && i != str2.size() && str1[i] == str2[i]) {
        ++i;
    }
    return str1.substr(0, i);
}

std::string CommonPrefix(const std::vector<std::string>& words) {
    if (!words.empty()) {
        std::string_view pref = words[0];
        for (size_t i = 0; i != words.size() && !pref.empty(); ++i) {
            pref = Temp(pref, words[i]);
        }
        return std::string(pref);
    } else {
        return "";
    }
}

int main() {
    std::vector<std::string> strings;
    std::string str;

    while (std::getline(std::cin, str)) {
        int count = 0;
        if (str.empty()) {
            break;
        } else {
            ++count;
            strings.push_back(str);
            strings.reserve(count);
        }
    }

    std::cout << CommonPrefix(strings);
}