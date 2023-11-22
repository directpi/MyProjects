#include <iostream>
using namespace std;

bool number(int num, int num2) { return (num2 & (1 << num)) != 0; }
int main() {
    const int bescon = 1E9;  // NMAX = 10;
    int n, i, j, k, m, tmp, res;
    cin >> n;
    int matrix[n][n], arr[1 << n][n];
    for (i = 0; i < n; ++i)
        for (j = 0; j < n; ++j) cin >> matrix[i][j];
    arr[1][0] = 0;
    m = 1 << n;
    for (i = 1; i < m; i += 2)
        for (j = (i == 1) ? 1 : 0; j < n; ++j) {
            arr[i][j] = bescon;
            if (j > 0 && number(j, i)) {
                tmp = i ^ (1 << j);
                for (k = 0; k < n; ++k)
                    if (number(k, i) && matrix[k][j] > 0)
                        arr[i][j] = min(arr[i][j], arr[tmp][k] + matrix[k][j]);
            }
        }
    for (j = 1, res = bescon; j < n; ++j)
        if (matrix[j][0] > 0) res = min(res, arr[m - 1][j] + matrix[j][0]);
    if (n == 1) res = 0;
    if (res == bescon)
        cout << -1;
    else
        cout << res;
}