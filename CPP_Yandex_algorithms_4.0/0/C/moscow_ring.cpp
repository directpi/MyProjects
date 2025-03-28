// #include <math.h>
// #include <stdio.h>

// int main() {
//     int x1, y1, x2, y2;
//     scanf("%d%d%d%d", &x1, &y1, &x2, &y2);

//     double r1 = sqrt((double)x1 * x1 + (double)y1 * y1);
//     double r2 = sqrt((double)x2 * x2 + (double)y2 * y2);

//     double line_move = r1 + r2;

//     if (!((x1 || y1) && (x2 || y2))) {
//         printf("%f", line_move);

//     } else {
//         double rotate = atan2(((double)x2 * y1 - (double)x1 * y2), ((double)x1 * x2 + (double)y1 * y2));
//         double rotate_move = fabs(r1 - r2) + fabs(rotate) * (r2 < r1 ? r2 : r1);

//         line_move < rotate_move ? printf("%f", line_move) : printf("%f", rotate_move);
//     }

//     return 0;
// }

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