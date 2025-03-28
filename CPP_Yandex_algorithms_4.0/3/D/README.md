[К оглавлению](../../README.md)

## D. Автобусы в Васюках

Все языки	Python 3.6
Ограничение времени	3 секунды	5 секунд
Ограничение памяти	512Mb	256Mb
Ввод	стандартный ввод или input.txt
Вывод	стандартный вывод или output.txt

Между некоторыми деревнями края Васюки ходят автобусы. Поскольку пассажиропотоки здесь не очень большие, то автобусы ходят всего несколько раз в день.

Марии Ивановне требуется добраться из деревни d в деревню v как можно быстрее (считается, что в момент времени 0 она находится в деревне d).

Формат ввода
Сначала вводится число N – общее число деревень (1 ≤ N ≤ 100), затем номера деревень d и v, за ними следует количество автобусных рейсов R (0 ≤ R ≤ 10000). Далее идут описания автобусных рейсов. Каждый рейс задается номером деревни отправления, временем отправления, деревней назначения и временем прибытия (все времена – целые от 0 до 10000). Если в момент t пассажир приезжает в какую-то деревню, то уехать из нее он может в любой момент времени, начиная с t.

Формат вывода
Выведите минимальное время, когда Мария Ивановна может оказаться в деревне v. Если она не сможет с помощью указанных автобусных рейсов добраться из d в v, выведите -1.

```cpp
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
```