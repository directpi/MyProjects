[К оглавлению](../../README.md)

## C. Максимальный разрез
Ограничение времени	1 секунда
Ограничение памяти	64Mb
Ввод	стандартный ввод или input.txt
Вывод	стандартный вывод или output.txt
Взвешенный неориентированный граф без петель задан матрицей смежности. Распределите вершины по двум долям так, чтобы сумма весов рёбер, соединяющих вершины из разных долей, была максимальна.

Формат ввода
Вводится число N (2 ≤ N ≤ 20) — количество вершин в графе.

В следующих N строках, содержащих по N целых чисел от 0 до 1000, задаётся матрица смежности. 0 означает отсутствие ребра.

Формат вывода
В первой строке выведите суммарный вес рёбер, соединяющих вершины из разных долей.

Во второй строке выведите N чисел 1 или 2 — номера долей для каждой из вершин графа.

```cpp
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, cnt_res;
    ifstream file("input.txt");
    if (file.is_open()) {
        string line;
        getline(file, line);
        n = atoi(line.c_str());
        vector<vector<pair<int, int>>> p(n + 1);
        long e = pow(2, n);
        vector<vector<int>> mask(e / 2, vector<int>(n));
        vector<int> sum_e_v(n + 1);

        for (int i = 0; i < e / 2; ++i) {
            for (int j = 0; j <= n; ++j) {
                mask[i][n - j - 1] = ((i >> j) & 1) == 1;
            }
        }
        int mt = 0, j = 1;
        while (getline(file, line)) {
            int i = 1;
            istringstream iss(line);
            string token;

            p[j].push_back(make_pair(0, 0));
            while (getline(iss, token, ' ')) {
                mt = atoi(token.c_str());
                if (mt != 0) {
                    p[j].push_back(make_pair(mt, i));
                    sum_e_v[j] += mt;
                } else {
                    p[j].push_back(make_pair(0, 0));
                }
                ++i;
            }
            ++j;
        }
        int res = 0;
        cnt_res = 0;

        vector<int> st;
        for (int k = 1; k < e / 2; ++k) {  //  int k = e / 2 - 1; k > 0; --k
            int tmp = 0;
            for (int i = 0; i < n; ++i) {
                if (mask[k][i] == 1) {
                    st.push_back(i + 1);
                    tmp += sum_e_v[i + 1];
                }
            }
            while (!st.empty()) {
                for (size_t m = 1; m < st.size(); ++m) {
                    tmp -= 2 * p[st[0]][st[m]].first;
                    if (tmp < res) {
                        break;
                    }
                }
                if (!st.empty()) st.erase(st.begin());
            }
            if (res < tmp) {
                res = tmp;
                cnt_res = k;
            }
        }
        cout << res << "\n";
        for (int i = 0; i < n; ++i) {
            if (mask[cnt_res][i] == 0) {
                cout << "2 ";
            } else {
                cout << "1 ";
            }
        }
        cout << "\n";
    } else {
        cout << "Не удалось открыть файл." << endl;
    }
    file.close();
}
```
