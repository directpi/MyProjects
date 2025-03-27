#include <iostream>
// #include <vector>
int *input(int *);

int main(void) {
    int counter = 0;
    int *array = input(&counter);

    if (array != NULL) {
        for (int i = 0; i < counter; ++i) {
            std::cout << array[i];
        }
        free(array);
    } else {
        std::cout << "n/a";
    }
    return 0;
}

int *input(int *counter) {
    int *array = NULL;
    char c;
    int m = 0;
    do {
        if ((std::cin >> m >> c) && c == 32) {
            if (m > 0) {
                ++*counter;
                array = array ? (int *)realloc(array, *counter * sizeof(int))
                              : (int *)malloc(*counter * sizeof(int));
                array[*counter - 1] = m;
            } else if (m < 0) {
                m = -1;
                break;
            }
        } else {
            break;
        }
    } while (c != '\n');
    if ((m > 0) && (c == '\n')) {
        if (*counter != 0) {
            array[*counter] = m;
            array = array ? (int *)realloc(array, *counter * sizeof(int)) : array;
        } else {
            *counter = 1;
            array = (int *)(malloc(*counter * sizeof(int)));
            array[*counter - 1] = m;
            *counter = 0;
        }
    } else {
        return 0;
    }
    return array;
}