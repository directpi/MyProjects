#include <iostream>

struct Zapros {
    int l = 0;
    int r = 0;
};

int main() {
    int n, m;
    int arr1[100];
    Zapros arr2[1000];

    std::cin >> n >> m;
    for (int i = 0; i < n; ++i) {
        std::cin >> arr1[i];
    }
    for (int i = 0; i < m; ++i) {
        std::cin >> arr2[i].l >> arr2[i].r;
    }
    for (int j = 0; j < m; ++j) {
        int min = arr1[arr2[j].l];
        for (int i = arr2[j].l; i <= arr2[j].r - 1; ++i) {
            if (arr1[i] > arr1[i + 1]) {
                min = arr1[i + 1];
            }
        }
        int count = 0, k = 0;
        for (int i = arr2[j].l; i <= arr2[j].r; ++i) {
            if (arr1[i] != min) {
                ++count;
                k = i;
                break;
            }
        }
        if (count != 0) {
            std::cout << arr1[k] << "\n";
        } else {
            std::cout << "NOT FOUND"
                      << "\n";
        }
    }
}
