#include <iostream>
#include <vector>
void q_sort(std::vector<int>& array, int index1, int index2) {
    if (index1 > index2) {
        return;
    }
    int p = array[(index1 + index2) / 2];  // array[rand % array.size()]
    int left = index1;
    int right = index2;

    while (left <= right) {
        while (array[left] < p) {
            ++left;
        }
        while (array[right] > p) {
            --right;
        }
        if (left <= right) {
            if (left != right) {
                int tmp = 0;
                tmp = array[left];
                array[left] = array[right];
                array[right] = tmp;
            }
            ++left;
            --right;
        }
    }
    q_sort(array, index1, right);
    q_sort(array, left, index2);
}
int main() {
    // const size_t m = 1000000;
    // srand(time(MULL))
    int n, l, r;
    std::vector<int> array(n);
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::cin >> array[i];
    }
    // array.resize(n);

    l = 0;
    r = n - 1;
    q_sort(array, l, r);
    for (int i = 0; i < n; ++i) {
        std::cout << array[i] << " ";
    }
}