#include <iostream>
#include <string>
#include <vector>

std::string CommonPrefix(const std::vector<std::string>& words) {
    if (words.empty()) {
        return "";
    }
    size_t lmin = words[0].size();
    for (const auto& str : words) {
        lmin = std::min(lmin, str.size());
    }
    for (size_t i = 0; i < lmin; ++i) {
        char c = words[0][i];
        for (const auto& str : words) {
            if (str[i] != c) return str.substr(0, i);
        }
    }

    return words[0].substr(0, lmin);
}

int main() {
    std::vector<std::string> strings;
    std::string str;
    int count = 0;
    while (std::getline(std::cin, str)) {
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