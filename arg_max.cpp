#include <iostream>
#include <vector>

std::pair<size_t, size_t> MatrixArgMax(const std::vector<std::vector<int>>& matrix, int n, int k) {
    std::pair<int, int> p;
    int max = matrix[0][0];
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < k; ++j) {
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
    std::vector<std::vector<int>> matrix;

    std::cin >> n >> k;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < k; ++j) {
            std::cin >> matrix[i][j];
        }
    }
    std::cout << MatrixArgMax(matrix, n, k).first << MatrixArgMax(matrix, n, k).second;
}