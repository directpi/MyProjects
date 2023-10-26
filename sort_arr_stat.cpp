#include <iostream>
int summ_digit(int);
int main() {
    int n, array[100];
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::cin >> array[i];
    }
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            // std::cout << array[i] << "\t" << array[j] << "\t" << summ_digit(array[i]) << "\t"
            //           << summ_digit(array[j]) << "\n";
            if (summ_digit(array[i]) > summ_digit(array[j])) {
                int tmp = array[i];
                array[i] = array[j];
                array[j] = tmp;
            }
        }
    }
    for (int i = 0; i < n; ++i) {
        std::cout << array[i];
        if (i < n - 1) {
            std::cout << " ";
        }
    }
}
int summ_digit(int m) {
    int res = 0;
    while (abs(m) > 0) {
        res += m % 10;
        m = m / 10;
    }
    return abs(res);
}