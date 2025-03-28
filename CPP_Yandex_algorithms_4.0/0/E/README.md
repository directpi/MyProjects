[К оглавлению](../../README.md)

## E. Средний уровень

В группе учатся n студентов, каждый из которых имеет свой рейтинг ai. Им нужно выбрать старосту; для этого студенты хотят выбрать старосту таким образом чтобы суммарный уровень недовольства группы был минимальный. Если выбрать j-го старостой, то уровень недовольства i-го студента равен ∣∣ai−aj∣∣. Например, если в группе есть три студента с рейтингами 1, 3 и 4 и в качестве старосты выбирают второго, то уровень недовольства группы будет равен |1−3|+|3−3|+|4−3|=3. Вычислите уровень недовольства группы при выборе каждого из студентов старостой.

Формат ввода
В первой строке дано единственное целое число n (1≤n≤105) — количество студентов в группе. Во второй строке даны n целых чисел a1,a2,…,an, идущих по неубыванию (0≤a1≤a2≤…≤an≤104) — рейтинги студентов.

Формат вывода
Выведите n чисел через пробел, i-е из которых будет обозначать уровень недовольства группы при выборе i-го студента старостой.

```cpp
#include <iostream>
#include <vector>

int main() {
    int n;
    std::cin >> n;
    std::vector<int> a(n);
    int sum_arr = 0;
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
        sum_arr += a[i];
    }
    int tmp_r = 0;
    int tmp_l = 0;
    for (int i = 0; i < n; ++i) {
        int sum = 0;
        int sum_l = 0;
        int sum_r = 0;
        tmp_r += a[i];
        sum_r = sum_arr - tmp_r - (a[i] * (n - 1 - i));
        sum_l += (a[i] * i - tmp_l);
        tmp_l += a[i];
        sum += sum_r + sum_l;
        std::cout << sum << " ";
    }
}
```