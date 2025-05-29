/**
 * выполняет сортировку массива чисел по возрастанию суммы их цифр.
 */
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>

int summ_digit(int m) {
  int res = 0;
  m = abs(m);
  while (m > 0) {
    res += m % 10;
    m /= 10;
  }
  return res;
}

int main() {
  int n;
  if (!(std::cin >> n)) {
    std::cerr << "Invalid input for n\n";
    return 1;
  }
  if (n <= 0) {
    std::cerr << "n must be positive\n";
    return 1;
  }
  std::vector<int> numbers;
  numbers.reserve(n);

  for (int i = 0; i < n; ++i) {
    int num;
    if (!(std::cin >> num)) {
      std::cerr << "Invalid input for number\n";
    }
    numbers.push_back(num);
  }
  std::sort(numbers.begin(), numbers.end(),
            [](int a, int b) { return summ_digit(a) < summ_digit(b); });

  for (int i = 0; i < numbers.size(); ++i) {
    std::cout << numbers[i];
    if (i < numbers.size() - 1) {
      std::cout << " ";
    }
  }
}
