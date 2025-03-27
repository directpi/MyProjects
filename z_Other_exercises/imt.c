#include <stdio.h>


int main()
{
    float m;    // масса, кг
    float h;    // рост, м


    float imt;  // индекс массы тела (формула Кетле)


    // читаем входные данные
    scanf("%f%f", &m, &h);


    // вычисления
    imt = m / (h * h);


    printf("%f\n", imt);


    return 0;
}