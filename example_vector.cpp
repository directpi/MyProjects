#include <iostream>
#include <vector>
 
int main() {
    std::vector<int> data = {1, 2};
    std::cout << data.size() << "\t" << data.capacity() << "\n";
 
    data.push_back(3);
    std::cout << data.size() << "\t" << data.capacity() << "\n";
 
    data.push_back(4);
    std::cout << data.size() << "\t" << data.capacity() << "\n";
 
    data.push_back(5);
    std::cout << data.size() << "\t" << data.capacity() << "\n";
}