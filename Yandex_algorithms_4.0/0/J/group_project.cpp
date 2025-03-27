#include <iostream>
#include <tuple>
#include <vector>
using namespace std;

int main() {
    int t;
    long n, a, b;

    cin >> t;
    vector<tuple<long, long, long>> arr;
    for (int i = 0; i < t; ++i) {
        cin >> n >> a >> b;
        arr.push_back(make_tuple(n, a, b));
    }

    for (int j = 0; j < t; ++j) {
        bool tmp = false;
        if (get<0>(arr[j]) % get<1>(arr[j]) <=
            (get<0>(arr[j]) / get<1>(arr[j])) * (get<2>(arr[j]) - get<1>(arr[j]))) {
            tmp = true;
        }
        // for (int k = 0; k < (get<2>(arr[j]) - get<1>(arr[j])); ++k) {
        //     if ((get<0>(arr[j]) % (get<1>(arr[j]) + k)) == 0) {
        //         tmp = true;
        //     }
        //     if ((get<0>(arr[j]) - (get<1>(arr[j]) + k)) / 2 <= get<2>(arr[j]) == 0) {
        //         tmp = true;
        //     }
        // }
        // if (tmp == false) {
        //     for (int p = 1; p <= 2; ++p) {
        //         for (int m = 1; m < 10; ++m) {
        //             for (int n = 0; n < 99; ++n) {
        //                 if (get<0>(arr[j]) == get<1>(arr[j]) * m + (get<2>(arr[j]) - n) * p &&
        //                     (get<2>(arr[j]) - n) >= get<1>(arr[j])) {
        //                     tmp = true;
        //                 }
        //             }
        //         }
        //     }
        // }
        // if (get<0>(arr[j]) % get<1>(arr[j]) == 0 && get<1>(arr[j]) == get<2>(arr[j])) {
        //     tmp = true;
        // }
        // if (get<0>(arr[j]) == get<1>(arr[j]) || get<0>(arr[j]) == get<2>(arr[j])) {
        //     tmp = true;
        // }
        // if (get<1>(arr[j]) == 1 || get<2>(arr[j]) == 1) {
        //     tmp = true;
        // }
        if (tmp) {
            cout << "YES";
        } else {
            cout << "NO";
        }
        cout << "\n";
    }
}