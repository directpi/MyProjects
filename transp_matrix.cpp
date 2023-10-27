#include <iostream>
#include <vector>

std::vector<std::vector<int>> Transpose(const std::vector<std::vector<int>>& matrix) {
    size_t m = matrix.size();
    size_t n = matrix[0].size();
    std::vector<std::vector<int>> matrix2(n);
    for (size_t j = 0; j != n; ++j) {
        matrix2[j].resize(m);
        for (size_t i = 0; i < m; ++i) {
            matrix2[j][i] = matrix[i][j];
        }
    }
    return matrix2;
}
int main() {
    size_t m = 0, n = 0;
    std::cin >> m >> n;
    std::vector<std::vector<int>> matrix(m, std::vector<int>(n));
    std::vector<std::vector<int>> matrix2(m, std::vector<int>(n));
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            std::cin >> matrix[i][j];
        }
    }
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            std::cout << Transpose(matrix)[i][j] << " ";
        }
        std::cout << "\n";
    }
}