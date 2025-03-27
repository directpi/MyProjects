[К оглавлению](../../README.md)

## A. Дейкстра

Ограничение времени	1 секунда
Ограничение памяти	64Mb
Ввод	стандартный ввод или input.txt
Вывод	стандартный вывод или output.txt

Дан ориентированный взвешенный граф. Найдите кратчайшее расстояние от одной заданной вершины до другой.

Формат ввода
В первой строке содержатся три числа: N, S и F (1≤ N ≤ 100, 1 ≤ S, F ≤ N), где N — количество вершин графа, S — начальная вершина, а F — конечная. В следующих N строках вводится по N чисел, не превосходящих 100, – матрица смежности графа, где -1 означает что ребра между вершинами нет, а любое неотрицательное число — наличие ребра данного веса. На главной диагонали матрицы записаны нули.

Формат вывода
Выведите искомое расстояние или -1, если пути между указанными вершинами не существует.

```cpp
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
    int res = dist[f];

    if (s == f) {
        res = 0;
        cout << res;
    } else if (res == 0) {
        cout << "-1";
    } else {
        cout << res;
    }
}
```