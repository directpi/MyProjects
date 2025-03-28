#include <iostream>
#include <vector>

int main() {
    const size_t d = 1000000;
    int n;  //, l, r;

    std::vector<int> ar1(d);
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::cin >> ar1[i];
    }
    ar1.resize(n);
    int m;
    std::vector<int> ar2(d);
    std::cin >> m;
    for (int i = 0; i < m; ++i) {
        std::cin >> ar2[i];
    }
    ar2.resize(m);
    std::vector<int> res(ar1.size() + ar2.size());
    size_t index1 = 0, index2 = 0;
    for (size_t i = 0; i != res.size(); ++i) {
        if (ar1[index1] < ar2[index2] && index1 < ar1.size()) {
            res[i] = ar1[index1];
            ++index1;
        } else if (index2 < ar2.size() && index1 >= ar1.size()) {
            res[i] = ar2[index2];
            index2++;
        } else if (ar1[index1] > ar2[index2] && index2 < ar2.size()) {
            res[i] = ar2[index2];
            index2++;
        } else if (index1 < ar1.size() && index2 >= ar2.size()) {
            res[i] = ar1[index1];
            index1++;
        } else if (ar1[index1] == ar2[index2]) {
            res[i] = ar1[index1];
            if (index1 < ar1.size()) {
                ++index1;
            }
        }
    }
    for (size_t i = 0; i != res.size(); ++i) {
        std::cout << res[i] << " ";
    }
}