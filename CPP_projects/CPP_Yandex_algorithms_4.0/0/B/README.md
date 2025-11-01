[К оглавлению](../../README.md)

## B. Сложить две дроби
Даны две рациональные дроби: a/b и c/d. Сложите их и результат представьте в виде несократимой дроби m/n.

Формат ввода
Программа получает на вход 4 натуральных числа a, b, c, d, каждое из которых не больше 100.

Формат вывода
Программа должна вывести два натуральных числа m и n такие, что m/n=a/b+c/d и дробь m/n – несократима.

```cpp
#include <iostream>

long long znam(long long b, long long d) {
    long long res = 0;
    if (b == d) {
        res = b;
    } else {
        int i = 2;
        long long tmp_b = b;
        long long tmp_d = d;
        while (tmp_b != tmp_d) {
            tmp_b = b * i;
            if ((tmp_b % d) == 0 || (tmp_b == d)) {
                res = tmp_b;
                tmp_d = tmp_b;
            }
            ++i;
        }
    }
    return res;
}

int main() {
    long long a, b, c, d;

    std::cin >> a >> b >> c >> d;
    if (b != 0 && d != 0 && a <= 100 && b <= 100 && c <= 100 && d <= 100) {
        long long n = znam(b, d);
        long long m = 0;
        if (n == b && d == n) {
            m = a + c;
        } else {
            m = (a * n / b) + (c * n / d);
        }
        if (m != n) {
            while (n % m == 0 && m != 1) {
                n = n / m;
                m = m / m;
            }
            for (int j = 100; j > 1; --j) {
                while (n % j == 0 && m % j == 0) {
                    n = n / j;
                    m = m / j;
                }
            }
            std::cout << m << " " << n << "\n";
        } else {
            m = m / m;
            n = n / n;
            for (int j = 100; j > 1; --j) {
                while (n % j == 0 && m % j == 0) {
                    n = n / j;
                    m = m / j;
                }
            }
            std::cout << m << " " << n;
        }
    } else {
        return 1;
    }
}
```