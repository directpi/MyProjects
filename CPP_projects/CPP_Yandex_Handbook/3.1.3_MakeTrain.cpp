#include <deque>
#include <iostream>
#include <string>

void MakeTrain() {
    std::deque<int> d;
    std::string str;
    int n;

    while (std::cin >> str >> n) {
        if (str == "+left") {
            d.push_front(n);
        }
        if (str == "+right") {
            d.push_back(n);
        }
        if (str == "-left") {
            for (int i = 0; i < n; ++i) {
                if (!d.empty()) {
                    d.pop_front();
                }
            }
        }
        if (str == "-right") {
            for (int i = 0; i < n; ++i) {
                if (!d.empty()) {
                    d.pop_back();
                }
            }
        }
    }
    size_t ii = 0;
    for (int i : d) {
        std::cout << i;
        if (ii < d.size() - 1) {
            std::cout << " ";
        }
        ++ii;
    }
    std::cout << "\n";
}
int main() { MakeTrain(); }

// +left W — добавить вагон с номером W слева
// +right W — добавить вагон с номером W справа
// -left N — отцепить и убрать N вагонов слева
// -right N — отцепить и убрать N вагонов справа