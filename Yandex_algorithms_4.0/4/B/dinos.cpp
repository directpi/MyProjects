#include <iostream>
#include <vector>
using namespace std;

int is_mat(vector<vector<int>>& arr, int raw, int column, int n) {
    for (int i = 0; i < raw; ++i) {
        if (arr[i][column] == 1) {
            return 0;
        }
    }
    for (int i = raw, j = column; i >= 0 && j >= 0; --i, --j) {
        if (arr[i][j] == 1) {
            return 0;
        }
    }
    for (int i = raw, j = column; i >= 0 && j < n; --i, ++j) {
        if (arr[i][j] == 1) {
            return 0;
        }
    }
    return 1;
}
int count_din(vector<vector<int>>& arr, int raw, int n, int& count) {
    if (raw == n) {
        ++count;
        return count;
    }
    for (int i = 0; i < n; ++i) {
        if (is_mat(arr, raw, i, n)) {
            arr[raw][i] = 1;
            count_din(arr, raw + 1, n, count);
            arr[raw][i] = 0;
        }
    }
    return count;
}
int main() {
    int n;
    int count = 0;
    cin >> n;
    vector<vector<int>> arr(n, vector<int>(n));

    int res = count_din(arr, 0, n, count);
    cout << res;
}
