#include <stdio.h>
#include <string.h>
#define N 1000
int main(void)
{
    char buf[N + 1] = "", *p1 = buf, *p2;

    scanf("%1000[^\n]", buf);
    for (p2 = buf + strlen(buf) - 1; p1 < p2 && *p1 == *p2; p1++, p2--)
        ;

    printf("%s\n", (p1 < p2) ? "No" : "Yes");
    return 0;
}
