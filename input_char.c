#include <stdio.h>
#include <stdlib.h>
char *input(void);

int main(void) {
    char *s = input();
    if (s) {
        printf("%s", s);
    } else {
        printf("NULL\n");
    }
    if (s != NULL) {
        free(s);
    }
    return 0;
}

char *input(void) {
    char *s = NULL;
    char c;
    int counter = 0;
    do {
        scanf("%c", &c);
        if (c != '\n') {
            ++counter;
            s = s ? realloc(s, counter * sizeof(char)) : malloc(counter * sizeof(char));
            s[counter - 1] = c;
        }
    } while (c != '\n');
    s = s ? realloc(s, counter * sizeof(char)) : s;
    s[counter] = '\0';
    return s;
}
