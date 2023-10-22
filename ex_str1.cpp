#include <iostream>
#include <string>
 
int main() {
    std::string phrase;
    std::getline(std::cin, phrase);
 
    if (phrase.starts_with("hello")) {
        std::cout << "Greeting\n";
    }
 
    if (phrase.ends_with("bye")) {
        std::cout << "Farewell\n";
    }
}