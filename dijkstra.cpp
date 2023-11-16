#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
int min(vector<long>& dist, vector<bool>& visited, int n) {
    int k = 1, res = 0;
    long tmp = __LONG_MAX__;
    while (k < n + 1) {
        if (visited[k] == false && dist[k] < tmp) {
            tmp = dist[k];
            res = k;
        }
        ++k;
    }
    // int res = min_element(dist[1], dist[n]);
    return res;
}
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
    prev[s] = -1;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            cin >> matrix[i][j];
        }
    }
    // int pov = f - s;
    for (int i = 1; i <= n; ++i) {
        int ind = min(dist, visited, n);
        if (ind == 0) {
            break;
        }
        for (int j = 1; j <= n; ++j) {
            if (visited[ind] == false) {
                if (matrix[ind][j] != 0 && matrix[ind][j] != (-1)) {
                    if (dist[j] > (matrix[ind][j] + dist[ind]) && j != s) {
                        dist[j] = matrix[ind][j] + dist[ind];
                        prev[j] = ind;
                    }
                }
            }
        }
        visited[ind] = true;
    }
    // int i = f;
    int res = dist[f];
    // int cnt = 0;
    // while (dist[i] != 0 || dist[prev[i]] != __LONG_MAX__) {
    //     if (dist[i] != 0) {
    //         res += dist[i] - dist[prev[i]];
    //         i = prev[i];
    //         ++cnt;
    //     }
    // }
    if (s == f) {
        res = 0;
        cout << res;
    } else if (res == 0) {
        cout << "-1";
    } else {
        cout << res;
    }
}