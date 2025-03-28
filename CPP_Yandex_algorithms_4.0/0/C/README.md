[К оглавлению](../../README.md)

## C. Путешествие по Москве

Мэрия Москвы основательно подготовилась к празднованию тысячелетия города в 2147 году, построив под столицей бесконечную асфальтированную площадку, чтобы заменить все существующие в городе автомобильные дороги. В память о кольцевых и радиальных дорогах разрешили двигаться по площадке только двумя способами:

В сторону точки начала координат или от неё. При этом из точки начала координат разрешено двигаться в любом направлении.
Вдоль окружности с центром в начале координат и радиусом, который равен текущему расстоянию до начала координат. Двигаться вдоль такой окружности разрешается в любом направлении (по или против часовой стрелки). Вам, как ведущему программисту ответственной инстанции поручено разработать модуль, который будет определять кратчайший путь из точки A, с координатами (xA, yA) в точку B с координатами (xB, yB). Считайте, что менять направление движения можно произвольное количество раз, но оно должно всегда соответствовать одному из двух описанных выше вариантов.
Формат ввода
В первой строке ввода заданы четыре целых числа xA, yA, xB и yB, по модулю не превосходящие 106.

Формат вывода
Выведите одно число — минимальное расстояние, которое придётся преодолеть по пути из точки A в точку B, если не нарушать правил дорожного движения. Ваш ответ будет принят, если его абсолютная или относительная погрешность не превосходит 10-6.

```cpp
#include <cmath>
#include <iostream>
#include <iomanip>
int main() {
    int xa, ya, xb, yb;
    double res;
    std::cin >> xa >> ya >> xb >> yb;

    double ra = xa != 0 || ya != 0 ? std::sqrt((double)xa * xa + (double)ya * ya) : 0;
    double rb = xb != 0 || yb != 0 ? std::sqrt((double)xb * xb + (double)yb * yb) : 0;
    double alpha = ra != 0 && rb != 0
                       ? std::atan2(((double)xb * ya - (double)xa * yb), ((double)xa * xb + (double)ya * yb))
                       : 0;

    res = alpha > 2 ? ra + rb : std::abs(ra - rb) + std::min(ra, rb) * std::abs(alpha);

    std::cout << std::setprecision(12) << std::fixed << res;
}
```