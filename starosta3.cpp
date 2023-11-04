#include <iostream>
#include <vector>

int main() {
    int n;
    std::cin >> n;
    std::vector<int> a(n);
    int sum_arr = 0;
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
        sum_arr += a[i];
    }
    int tmp_r = 0;
    int tmp_l = 0;
    for (int i = 0; i < n; ++i) {
        int sum = 0;
        int sum_l = 0;
        int sum_r = 0;
        tmp_r += a[i];
        sum_r = sum_arr - tmp_r - (a[i] * (n - 1 - i));
        sum_l += (a[i] * i - tmp_l);
        tmp_l += a[i];
        sum += sum_r + sum_l;
        std::cout << sum << " ";
    }
}