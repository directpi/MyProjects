[К оглавлению](../../README.md)

## C. Быстрый алгоритм Дейкстры

Ограничение времени	5 секунд
Ограничение памяти	512Mb
Ввод	стандартный ввод или input.txt
Вывод	стандартный вывод или output.txt

Вам дано описание дорожной сети страны. Ваша задача – найти длину кратчайшего пути между городами А и B.

Формат ввода
Сеть дорог задана во входном файле следующим образом: первая строка содержит числа N и K (1 ≤ N ≤ 100000, 0 ≤ K ≤ 300000), где K – количество дорог. Каждая из следующих K строк содержит описание дороги с двусторонним движением – три целых числа ai, bi и li (1 ≤ ai, bi ≤ N, 1 ≤ li ≤ 106). Это означает, что имеется дорога длины li, которая ведет из города ai в город bi. В последней строке находятся два числа А и В – номера городов, между которыми надо посчитать кратчайшее расстояние (1 ≤ A, B ≤ N)

Формат вывода
Выведите одно число – расстояние между нужными городами. Если по дорогам от города А до города В доехать невозможно, выведите –1.

```cpp
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
```