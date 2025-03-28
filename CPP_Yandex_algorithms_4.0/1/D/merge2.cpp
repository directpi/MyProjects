#include <iostream>
#include <vector>
void merge(std::vector<int>& ar1, size_t index1, size_t index2) {
    size_t left = index1;
    size_t mid = index1 + (index2 - index1) / 2;
    size_t right = mid + 1;
    size_t count = 0;
    std::vector<int> res(ar1.size());
    while (left < mid && right <= index2) {
        if (ar1[left] <= ar1[right]) {
            res[count] = ar1[left];
            ++left;
        } else {
            res[count] = ar1[right];
            ++right;
        }
        ++count;
    }
    while (left <= mid) {
        res[count] = ar1[left];
        ++left;
        ++count;
    }
    while (right <= index2) {
        res[count] = ar1[right];
        ++right;
        ++count;
    }
    for (size_t i = 0; i != res.size(); ++i) {
        ar1[index1 + 1] = res[i];
    }
}
// void merge(std::vector<int>& ar1, std::vector<int>& ar2, std::vector<int>& res) {
//     size_t index1 = 0, index2 = 0;
//     for (size_t i = 0; i != res.size(); ++i) {
//         if (ar1[index1] < ar2[index2] && index1 < ar1.size()) {
//             res[i] = ar1[index1];
//             ++index1;
//         } else if (index2 < ar2.size() && index1 >= ar1.size()) {
//             res[i] = ar2[index2];
//             index2++;
//         } else if (ar1[index1] > ar2[index2] && index2 < ar2.size()) {
//             res[i] = ar2[index2];
//             index2++;
//         } else if (index1 < ar1.size() && index2 >= ar2.size()) {
//             res[i] = ar1[index1];
//             index1++;
//         } else if (ar1[index1] == ar2[index2]) {
//             res[i] = ar1[index1];
//             if (index1 < ar1.size()) {
//                 ++index1;
//             }
//         }
//     }
// }

void merge_sort(std::vector<int>& array, int index1, int index2) {
    if (index2 <= index1) {
        return;
    }
    if (index1 < index2) {
        if (index2 - index1 == 1) {
            if (array[index1] > array[index2]) {
                int tmp = 0;
                tmp = array[index1];
                array[index1] = array[index2];
                array[index2] = tmp;
            }
        } else {
            merge_sort(array, index1, index1 + (index2 - index1) / 2);
            merge_sort(array, index1 + (index2 - index1) / 2, index2);
            merge(array, index1, index2);
        }
    }

    // while (left <= right) {
    //     while (array[left] < p) {
    //         ++left;
    //     }
    //     while (array[right] > p) {
    //         --right;
    //     }
    //     if (left <= right) {
    //         if (left != right) {
    //             int tmp = 0;
    //             tmp = array[left];
    //             array[left] = array[right];
    //             array[right] = tmp;
    //         }
    //         ++left;
    //         --right;
    //     }
    // }
    // // std::sort(array.begin()+left, array.end());
    // sort(array, index1, right);
    // sort(array, left, index2);
}
int main() {
    const size_t d = 1000000;
    int n;  //, l, r;

    std::vector<int> ar1(d);
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::cin >> ar1[i];
    }
    ar1.resize(n);
    // int m;
    // std::vector<int> ar2(d);
    // std::cin >> m;
    // for (int i = 0; i < m; ++i) {
    //     std::cin >> ar2[i];
    // }
    // ar2.resize(m);

    // std::vector<int> res(ar1.size() + ar2.size());
    merge_sort(ar1, 0, n - 1);

    for (size_t i = 0; i != ar1.size(); ++i) {
        std::cout << ar1[i] << " ";
    }
}