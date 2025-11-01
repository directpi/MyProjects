#include <iostream>
#include <list>
#include <string>
#include <utility>

int main() {
    std::list<std::string> v;
    std::string str, bufer;

    getline(std::cin, str);
    while (str != "") {
        v.push_back(str);
        getline(std::cin, str);
    }
    auto cursor = v.begin();
    if (str == "") {
        while (getline(std::cin, str)) {
            if (str == "Down" && cursor != v.end()) ++cursor;
            if (str == "Up" && cursor != v.begin()) --cursor;
            if (str == "Ctrl+X") {
                if (cursor != v.end()) {
                    bufer = std::move(*cursor);
                    cursor = v.erase(cursor);
                }
            }
            if (str == "Ctrl+V") {
                if (bufer != "") {
                    v.insert(cursor, bufer);
                }
            }
        }
    }
    for (auto &i : v) {
        std::cout << i << "\n";
    }
}