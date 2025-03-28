#include <iostream>
int main() {
    int n, x;
    int array[1000000];

    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::cin >> array[i];
    }
    std::cin >> x;
    int res1 = 0, res2 = 0;
    for (int i = 0; i < n; ++i) {
        if (array[i] < x) {
            ++res1;
        } else {
            ++res2;
        }
    }
    std::cout << res1 << "\n" << res2;
}