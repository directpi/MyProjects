#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> lines;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        } else {
            lines.push_back(line);
        }
    }

    std::sort(lines.rbegin(), lines.rend());
    std::cout << lines.size() << "\n";
    for (size_t i = 0; i != lines.size(); ++i) {
        std::cout << lines[i] << "\n";
    }
}