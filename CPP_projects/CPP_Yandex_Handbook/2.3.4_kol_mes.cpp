#include <iostream>

int main() {
    int mes, year, vis = 0;
    std::cin >> mes >> year;
    if (year % 400 == 0) {
        vis = 1;
        // std::cout << "YES";
    } else if (year % 4 == 0 && year % 100 != 0) {
        vis = 1;
        // std::cout << "YES";
    } else {
        vis = 0;
        // std::cout << "NO";
    }
    if (mes == 1 || mes == 3 || mes == 5 || mes == 7 || mes == 8 || mes == 10 || mes == 12) {
        std::cout << 31;
    } else if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
        std::cout << 30;
    } else if (mes == 2 && vis == 0) {
        std::cout << 28;
    } else {
        std::cout << 29;
    }
}