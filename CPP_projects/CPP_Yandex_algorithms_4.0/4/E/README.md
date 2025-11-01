[К оглавлению](../../README.md)

## E. Генерация правильных скобочных последовательностей - 2

Ограничение времени	2 секунды
Ограничение памяти	64Mb
Ввод	стандартный ввод или brackets2.in
Вывод	стандартный вывод или brackets2.out

По данному числу n выведите все правильные скобочные последовательности из круглых и квадратных скобок длины n в лексикографическом порядке.

Формат ввода
Одно целое число n (0 ≤ n ≤ 16).

Формат вывода
Выведите все правильные скобочные последовательности из круглых и квадратных скобок длины n в лексикографическом порядке. Каждая последовательность должна выводиться в новой строке.

```cpp
#include <iostream>
#include <stack>
#include <string>
#include <vector>
using namespace std;

char get_pair(char openBracket, string opened, string closed) {
    int pos = opened.find(openBracket);
    return closed[pos];
}
void permutation(int pos, int sum_open_brackets, int n, string& result, string opened, string closed,
                 stack<char>& st) {
    if (pos == n) {
        cout << result << "\n";
        return;
    }
    if (n - 1 - pos > sum_open_brackets) {
        for (size_t i = 0; i < opened.size(); i++) {
            result[pos] = opened[i];
            st.push(opened[i]);

            permutation(pos + 1, sum_open_brackets + 1, n, result, opened, closed, st);

            st.pop();
            result[pos] = ' ';
        }
    }
    if (!st.empty()) {
        char current_open = st.top();
        result[pos] = get_pair(current_open, opened, closed);
        st.pop();

        permutation(pos + 1, sum_open_brackets - 1, n, result, opened, closed, st);

        st.push(current_open);
        result[pos] = ' ';
    }
}
int main() {
    int n;
    string opened = "([";
    string closed = ")]";
    stack<char> st;
    string result;
    cin >> n;
    result.resize(n);
    permutation(0, 0, n, result, opened, closed, st);
    return 0;
}
```
