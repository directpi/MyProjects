#include <iostream>
#include <vector>
using namespace std;
int main() {
    int n, s, f;
    cin >> n >> s >> f;
    vector<bool> visited(n + 1);
    vector<long> dist(n + 1);
    vector<int> prev(n + 1);
    int matrix[n + 1][n + 1];
    for (int i = 0; i <= n; ++i) {
        dist[i] = __LONG_MAX__;
        visited[i] = false;
        prev[i] = 0;
        for (int j = 0; j <= n; ++j) {
            matrix[i][j] = 0;
        }
    }
    dist[s] = 0;
    prev[1] = -1;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            cin >> matrix[i][j];
        }
    }

    for (int i = s; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (visited[i] == false) {
                if (matrix[i][j] != 0 && matrix[i][j] != (-1)) {
                    if (dist[j] > matrix[i][j] && j != s) {
                        dist[j] = matrix[i][j];
                        prev[j] = i;
                    }
                }
            }
        }
        visited[i] = true;
    }
    if (s != 1) {
        for (int i = 1; i <= s - 1; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (visited[i] == false) {
                    if (i != j && matrix[i][j] != (-1)) {
                        if (dist[j] > matrix[i][j] && j != s) {
                            dist[j] = matrix[i][j];
                            prev[j] = i;
                        }
                    }
                }
            }
            visited[i] = true;
        }
    }
    int i = f;
    int res = 0;
    while (prev[i] != 0) {
        if (dist[i] != 0) {
            res += dist[i];
            i = prev[i];
        }
    }
    if (s == f) {
        res = 0;
        cout << res;
    } else if (res == 0) {
        cout << "-1";
    } else {
        cout << res;
    }
}