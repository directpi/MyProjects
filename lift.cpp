#include <iostream>
#include <vector>

int main() {
    size_t k, n;
    std::cin >> k;
    std::cin >> n;
    std::vector<int> array(n);
    // int sum = 0;
    for (size_t i = 0; i < n; ++i) {
        std::cin >> array[i];
        // sum += array[i];
    }
    int res = 0;
    for (int i = n; i > 0; --i) {
        int tmp = 0;
        if (array[i] % k != 0) {
            ++tmp;
        }
        res += (array[i] / k + tmp) * i + 1;
    }

    // res += (sum / k + tmp) * (n + 1);
    std::cout << res;
}