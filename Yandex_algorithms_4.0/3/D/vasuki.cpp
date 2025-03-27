#include <iostream>
#include <set>
#include <tuple>
#include <vector>
using namespace std;

int main() {
    int n, k_, s, f, a, b, t1, t2;
    cin >> n >> s >> f >> k_;
    vector<long> dist(n + 1);
    vector<long> prev(n + 1);
    vector<vector<tuple<long, long, long, long>>> p(n + 1);
    for (int i = 0; i <= n; ++i) {
        dist[i] = __LONG_MAX__;
        prev[i] = 0;
    }
    for (int j = 1; j <= k_; ++j) {
        cin >> a >> t1 >> b >> t2;
        p[a].push_back(make_tuple(a, t1, b, t2));
    }
    dist[s] = 0;
    set<pair<long, long>> que;
    que.insert(make_pair(dist[s], s));
    while (!que.empty()) {
        long ver = que.begin()->second;
        que.erase(que.begin());
        for (size_t j = 0; j < p[ver].size(); ++j) {
            long to = get<2>(p[ver][j]);
            long len = get<3>(p[ver][j]);
            if (len < dist[to] && dist[ver] <= get<1>(p[ver][j])) {
                que.erase(make_pair(dist[to], to));
                dist[to] = len;
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