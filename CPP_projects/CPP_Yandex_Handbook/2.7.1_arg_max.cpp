#include <iostream>
#include <utility>
#include <vector>

std::pair<size_t, size_t> MatrixArgMax(const std::vector<std::vector<int>>& matrix) {
    std::pair<size_t, size_t> p = {0, 0};
    int max = matrix[0][0];
    for (size_t i = 0; i != matrix.size(); ++i) {
        for (size_t j = 0; j != matrix[i].size(); ++j) {
            if (matrix[i][j] > max) {
                max = matrix[i][j];
                p.first = i;
                p.second = j;
            }
        }
    }
    return p;
}

int main() {
    int n, k;
    std::cin >> n >> k;
    std::vector<std::vector<int>> matrix(n, std::vector<int>(k));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < k; ++j) {
            std::cin >> matrix[i][j];
        }
    }
    std::cout << MatrixArgMax(matrix).first << " " << MatrixArgMax(matrix).second;
}