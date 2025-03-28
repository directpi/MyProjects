[К оглавлению](../../README.md)

## B. Основание строки

Ограничение времени	1 секунда
Ограничение памяти	256Mb
Ввод	стандартный ввод или input.txt
Вывод	стандартный вывод или output.txt

Строка S была записана много раз подряд, после чего от получившейся строки взяли префикс и дали вам. Ваша задача определить минимально возможную длину исходной строки S.

Формат ввода
В первой и единственной строке входного файла записана строка, которая содержит только латинские буквы, длина строки не превышает 50000 символов.

Формат вывода
Выведите ответ на задачу.

```cpp
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

void z_function(string str) {
    int n = (int)str.length();
    vector<int> z(n);
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        if (i <= r) z[i] = min(r - i + 1, z[i - l]);

        while (i + z[i] < n && str[z[i]] == str[i + z[i]]) ++z[i];
        if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
    }

    int j = 1;
    int res = (int)str.length();
    for (j = 1; j < (int)z.size(); ++j) {
        if (j + z[j] == (int)str.length()) {
            res = j;
            break;
        }
    }


    cout << res;  //  << "\n";

}

int main() {
    string str;
    getline(cin, str);
    z_function(str);
}
```