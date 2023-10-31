#include <iostream>
uint64_t znam(uint64_t b, uint64_t d) {
    uint64_t res = 0, tmp_b = 0, tmp_d = 0;
    if (b == d) {
        res = b;
    } else {
        int i = 2;
        tmp_b = b;
        tmp_d = d;
        while (tmp_b != tmp_d) {
            if (tmp_b < tmp_d) {
                tmp_b = b * i;
                if ((tmp_b % d) == b) {
                    res = tmp_b;
                    tmp_d = tmp_b;
                }
            } else {
                tmp_d = d * i;
                if ((tmp_d % b) == d) {
                    res = tmp_d;
                    tmp_b = tmp_d;
                }
            }
            ++i;
        }
    }
    return res;
}

int main() {
    uint64_t a, b, c, d, m, n;

    std::cin >> a >> b >> c >> d;
    if (b != 0 && d != 0) {
        n = znam(b, d);

        if (n == b) {
            m = a + c;
        } else {
            m = (a * n / b) / n + (c * n / d) / n;
        }
        std::cout << m << " " << n << "\n";
        if (m != n) {
            while (n % m == 0 && m != 1) {
                n = n / m;
                m = m / m;
            }
            std::cout << m << " " << n;

        } else {
            m = m / m;
            n = n / n;
            std::cout << m << " " << n;
        }
    } else {
        return 1;
    }
}