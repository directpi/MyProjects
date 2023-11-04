#include <iostream>
#include <vector>

int main() {
    int n;
    // const int d = 100000;
    std::cin >> n;
    std::vector<int> a(n);
    int sum_arr = 0;
    int sum = 0;
    // int tmp = 0;
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
        sum_arr += a[i];
    }
    std::cout << sum_arr << "\n";
    // a.resize(n);
    // int sum_l = 0;
    // int sum_r = 0;
    for (int i = 0; i < n; ++i) {
        int sum_l = 0;
        int sum_r = 0;
        sum_r = sum_arr - a[i] - (a[i] * (n - 1 - i));
        std::cout << sum_r << "\n";
        sum_l += (a[i] * i - sum_l);
        std::cout << sum_l << "\n";
        sum += sum_r + sum_l;
        std::cout << sum << " "
                  << "\n";
    }
}