#include <iostream>
#include <set>
#include <vector>
using namespace std;

int main() {
    int n, k_, s, f, a, b, l;
    cin >> n >> k_;
    vector<size_t> dist(n + 1);
    vector<size_t> prev(n + 1);
    vector<vector<pair<size_t, size_t>>> p(n + 1);
    for (int i = 0; i <= n; ++i) {
        dist[i] = __LONG_MAX__;
        prev[i] = 0;
    }
    for (int j = 1; j <= k_; ++j) {
        cin >> a >> b >> l;
        if (l != 0) {
            p[b].push_back(make_pair(l, a));
            p[a].push_back(make_pair(l, b));
        }
    }
    cin >> s >> f;
    dist[s] = 0;
    set<pair<size_t, size_t>> que;
    que.insert(make_pair(dist[s], s));
    while (!que.empty()) {
        size_t ver = que.begin()->second;
        que.erase(que.begin());

        for (size_t j = 0; j < p[ver].size(); ++j) {
            size_t to = p[ver][j].second;
            size_t len = p[ver][j].first;
            if (dist[ver] + len < dist[to]) {
                que.erase(make_pair(dist[to], to));
                dist[to] = dist[ver] + len;
                prev[to] = ver;
                que.insert(make_pair(dist[to], to));
            }
        }
    }
    long res = dist[f];
    if (s == f) {
        res = 0;
        cout << res;
    } else if (res == 0 || res == __LONG_MAX__) {
        cout << "-1";
    } else {
        cout << res;
    }
}