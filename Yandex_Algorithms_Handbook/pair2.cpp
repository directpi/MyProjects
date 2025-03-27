#include <cmath>
#include <iostream>
#include <vector>
struct Mystruct {
    // int value;
    int x;
    int y;
};

int main() {
    size_t n;
    Mystruct tmp_pair;
    std::cin >> n;
    std::vector<Mystruct> arr(n);

    for (size_t i = 0; i < n; ++i) {
        std::cin >> arr[i].x >> arr[i].y;
    }
    for (size_t i = 0; i < n - 1; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            if ((pow(arr[i].x, 2) + pow(arr[i].y, 2)) > (pow(arr[j].x, 2) + pow(arr[j].y, 2))) {
                tmp_pair.x = arr[i].x;
                tmp_pair.y = arr[i].y;
                arr[i].x = arr[j].x;
                arr[i].y = arr[j].y;
                arr[j].x = tmp_pair.x;
                arr[j].y = tmp_pair.y;
            }
        }
    }
    for (size_t i = 0; i < n; ++i) {
        std::cout << arr[i].x << " " << arr[i].y;
        if (i < n - 1) {
            std::cout << "\n";
        }
    }
}