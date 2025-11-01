#include <iostream>
#include <list>
#include <string>

int main() {
    std::list<std::string> v;
    std::list<std::string> v_buf;
    bool flag_shift = false;
    int count = 0;

    while (true) {
        std::string str;
        std::getline(std::cin, str);

        if (str.empty()) {
            break;
        }
        v.push_back(str);
    }
    auto cursor = v.begin();
    auto shift = v.begin();
    std::string cmmd;

    while (getline(std::cin, cmmd)) {
        if (cmmd == "Down" && cursor != v.end()) {
            if (cursor == v.end()) {
                continue;
            }
            ++cursor;
            if (flag_shift) {
                ++count;
            } else {
                shift = cursor;
            }
        }
        if (cmmd == "Up" && cursor != v.begin()) {
            if (cursor == v.begin()) {
                continue;
            }
            --cursor;
            if (flag_shift) {
                --count;
            } else {
                shift = cursor;
            }
        }
        if (cmmd == "Ctrl+X") {
            if (shift == cursor && cursor == v.end()) {
                continue;
            }
            v_buf.clear();
            if (shift == cursor) {
                auto ts = cursor;
                cursor = std::next(cursor);
                v_buf.splice(v_buf.begin(), v, ts);
            } else if (count > 0) {
                v_buf.splice(v_buf.begin(), v, shift, cursor);
            } else {
                v_buf.splice(v_buf.begin(), v, cursor, shift);
                cursor = shift;
            }
            shift = cursor;
            flag_shift = false;
            count = 0;
        }
        if (cmmd == "Ctrl+V") {
            if (v_buf.empty()) {
                continue;
            } else {
                if (count > 0) {
                    cursor = v.erase(shift, cursor);
                } else if (count < 0) {
                    cursor = v.erase(cursor, shift);
                }
                v.insert(cursor, v_buf.begin(), v_buf.end());
                flag_shift = false;
                shift = cursor;
                count = 0;
            }
        }
        if (cmmd == "Shift") {
            flag_shift = true;
        }
    }
    for (const auto &i : v) {
        std::cout << i << "\n";
    }
}