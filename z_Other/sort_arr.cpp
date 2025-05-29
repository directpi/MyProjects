#include <iostream>
int summ_digit(int);
int main() {
    int n, array[10];
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::cin >> array[i];
    }
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (summ_digit(array[i]) > summ_digit(array[j])) {
                int tmp = array[i];
                std::cout << array[i] << "\t" << array[j] << "\t" << summ_digit(array[i]) << "\t"
                          << summ_digit(array[j]);
                array[i] = array[j];
                array[j] = tmp;
            }
        }
    }
    for (int i = 0; i < n; ++i) {
        std::cout << array[i] << " ";
    }
}