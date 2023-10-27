#include <iostream>
#include <utility>
#include <vector>
#include <cmath>
int main() {
    size_t n;
    std::pair<int, int> tmp_pair;
    std::vector<std::pair<int, int>> arr_pairs;
    std::cin >> n;
    for (size_t i = 0; i < n; ++i) {
        std::cin >> arr_pairs[i].first >> arr_pairs[i].second;
    }
    for (size_t i = 0; i < n - 1; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            if (   (pow(arr_pairs[i].first,2) + pow(arr_pairs[i].second,2)) >
                (pow(arr_pairs[j].first,2) + pow(arr_pairs[j].second,2))) {
                tmp_pair.first = arr_pairs[i].first;
                tmp_pair.second = arr_pairs[i].second;
                arr_pairs[i].first = arr_pairs[j].first;
                arr_pairs[i].second = arr_pairs[j].second;
                arr_pairs[j].first = tmp_pair.first;
                arr_pairs[j].second = tmp_pair.second;
            }
        }
    }
    for (size_t i = 0; i < n; ++i) {
        std::cout << arr_pairs[i].first << " " << arr_pairs[i].second;
        if (i < n - 1) {
            std::cout << "\n";
        }
    }
}