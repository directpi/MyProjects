#include <iostream>
#include <vector>

int main() {
    size_t m, n, x, y, k;
    int counter;
    std::cin >> m >> n >> k;

    std::vector<std::vector<int>> matrix(m, std::vector<int>(n));
    // if (m > 1 && m <= 100 && n > 1 && n <= 100 && k > 0 && k <= m * n) {
    for (size_t i = 0; i != k; ++i) {
        std::cin >> x >> y;
        // if (x <= m && y <= n) {
        matrix[x - 1][y - 1] = -1;
        // } else {
        //     exit(0);
        // }
    }

    for (size_t i = 0; i != m; ++i) {
        for (size_t j = 0; j != n; ++j) {
            counter = 0;
            if (matrix[i][j] != -1) {
                if (i != 0 && j != 0 && matrix[i - 1][j - 1] == -1) {
                    ++counter;
                }
                if (i != 0 && matrix[i - 1][j] == -1) {
                    ++counter;
                }
                if (i != 0 && j + 1 != n && matrix[i - 1][j + 1] == -1) {
                    ++counter;
                }
                if (j != 0 && matrix[i][j - 1] == -1) {
                    ++counter;
                }
                if (j != n - 1 && matrix[i][j + 1] == -1) {
                    ++counter;
                }
                if (i != m - 1 && j != 0 && matrix[i + 1][j - 1] == -1) {
                    ++counter;
                }
                if (i != m - 1 && matrix[i + 1][j] == -1) {
                    ++counter;
                }
                if (i != m - 1 && j + 1 != n && matrix[i + 1][j + 1] == -1) {
                    ++counter;
                }
                matrix[i][j] = counter;
            }
        }
    }
    for (size_t i = 0; i != m; ++i) {
        for (size_t j = 0; j != n; ++j) {
            if (matrix[i][j] == -1) {
                std::cout << "*";
                if (j + 1 != n) std::cout << " ";
            } else {
                std::cout << matrix[i][j];
                if (j + 1 != n) std::cout << " ";
            }
        }
        std::cout << "\n";
    }
    // }
    // else {
    //     exit(0);
    // }
}