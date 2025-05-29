#include <stdio.h>
int main() {
    int rub = 0, kop = 0, rr = 0, pp = 0, kk = 0, rest = 0;
    scanf("%d%d", &rub, &kop);

    // if ((rub-1)==0 && )
    for (int i = 0; i < rub; ++i) {
        ++rr;
    }
    printf("%d\n", rr);
    if (rr * 5 > kop) {
        rr--;
        kop += 100;
    }
    for (int i = 0; i < rr; ++i) {
        kop = kop - 5;
        ++pp;
    }
    printf("%d\n", pp);
    for (int i = 0; i < pp; ++i) {
        kop = kop - 1;
        ++kk;
    }
    printf("%d\n", kk);

    if (rr == pp && rr == kk) {
        rest = kop;
        printf("%d %d", rr, rest);
    }
}