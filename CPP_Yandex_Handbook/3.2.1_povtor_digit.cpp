#include <iostream>
#include <set>
 
int main() {
    int num;
    std::set<int> nums;
while (std::cin >> num) {
       if (!nums.insert(num).second) {
           std::cout << "YES\n";
       } else {
           std::cout << "NO\n";
       }
    }
}