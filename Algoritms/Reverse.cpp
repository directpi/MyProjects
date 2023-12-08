#include <iostream>
#include <vector>

int main() {
    int n;

    std::cin >> n;
    std::vector<int> num_seat(n);
    for (int guest_nom = 1; guest_nom <= n; ++guest_nom) {
        int s;
        std::cin >> s;
        num_seat[s - 1] = guest_nom;
    }
    for (int i = 0; i < n; ++i) {
        std::cout << num_seat[i] << " ";
    }
    std::cout << "\n";
}