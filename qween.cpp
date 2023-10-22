#include <iostream>
int main(){
    int x1, y1, x2, y2;
    std::cin >> y1 >> x1 >> y2 >>x2;
    if ((x1==x2 || y1==y2) || (x1==y1 && x2==y2) || (x1==y2 && x2==y1) || (y1+x2==y2+x1) || (x2-y1==x1-y2)){
        std::cout << "Yes";
    } else{
        std::cout << "No";
    }
}