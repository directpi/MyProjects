#include <iostream>
using namespace std;

bool get(int nmb, int x) { return (x & (1 << nmb)) != 0; }
int main() {
    const int inf = 1E9, NMAX = 16;
    int n, i, j, k, m, temp, ans, d[NMAX][NMAX], t[1 << NMAX][NMAX];
    cin >> n;
    for (i = 0; i < n; ++i)
        for (j = 0; j < n; ++j) cin >> d[i][j];
    t[1][0] = 0;
    m = 1 << n;
    for (i = 1; i < m; i += 2)
        for (j = (i == 1) ? 1 : 0; j < n; ++j) {
            t[i][j] = inf;
            if (j > 0 && get(j, i)) {
                temp = i ^ (1 << j);
                for (k = 0; k < n; ++k)
                    if (get(k, i) && d[k][j] > 0) t[i][j] = min(t[i][j], t[temp][k] + d[k][j]);
            }
        }
    for (j = 1, ans = inf; j < n; ++j)
        if (d[j][0] > 0) ans = min(ans, t[m - 1][j] + d[j][0]);
    if (ans == inf)
        cout << -1;
    else
        cout << ans;
}