#include <iostream>
#include <vector>

int main() {
    long int k, n;
    std::cin >> k;
    std::cin >> n;
    std::vector<long int> array(n);
    long long int sum = 0;
    for (long int i = 0; i < n; ++i) {
        std::cin >> array[i];
        sum += array[i];
    }
    unsigned long long int res = 0;
    long int dt = 0;
    long int dp = 0;
    long int i = n - 1;
    while (sum >= 0) {
        if (array[i] != 0) {
            // int tmp = 0;
            // int z = 0;
            if (dt != 0 && array[i] >= dt) {
                array[i] -= dt;
                // z = dt;
                dt = 0;
            } else if (array[i] < dt) {
                dt -= array[i];
                array[i] -= array[i];
            }
            // if (dp != 0) {  // изменить здесь
            //     array[i] += dp;
            //     z = dp * (-1);
            //     dp = 0;
            // }
            if (array[i] != 0) {
                if (array[i] % k != 0 && array[i] > k) {
                    dp = (array[i] - k * (array[i] / k));
                }
                if (array[i] < k && array[i] != 0) {
                    dt = k - array[i];  // свободное место в лифте
                } else if (dp < k && dp != 0 && n > 1) {
                    dt = k - dp;  // люди, которые уехали в свободном лифте рейса верхнего этажа
                    dp = 0;
                }
                int dop = 0;
                if (array[i] % k != 0 && array[i] > k) {  //
                    dop = 1;
                } else if (array[i] / k == 0) {
                    dop = 1;
                }
                res += ((array[i] / k) + dop) * (i + 1) * 2;

                if (dop != 0 && dp != 0) {
                }
                sum -= array[i] + dt;
                i--;
            } else {
                --i;
            }

        } else {
            --i;
            // dt = 0;
        }
    }

    // res += (sum / k + tmp) * (n + 1);
    std::cout << res;
}