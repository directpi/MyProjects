#include <stdio.h>
#include <stdlib.h>
int *input(int *);

int main(void)
{
    int counter = 0;
    int *arr = input(&counter);

    if (arr != NULL)
    {
        for (int i = 0; i <= counter; i++)
        {
            printf("%d_", arr[i]);
        }
        free(arr);
    }
    else
    {
        printf("n/a");
    }
    return 0;
}

int *input(int *counter)
{
    int *arr = NULL;
    char c;

    int m = 0;
    do
    {
        if (scanf("%d%c", &m, &c) && c == 32)
        {
            if ( m > 0 ) //&& (c > 32 && c < 127)
            {
                ++*counter;
                arr = arr ? realloc(arr, *counter * sizeof(int)) : malloc(*counter * sizeof(int));
                arr[*counter - 1] = m;
            }
            else if (m < 0)
            {
                m = -1;
                break;
            }
        }
        else
        {
            break;
        }
    } while (c != '\n');
    if ((m > 0) && (c == '\n'))
    {
        if (*counter != 0)
        {
            arr[*counter] = m;
            arr = arr ? realloc(arr, *counter * sizeof(int)) : arr;
        }
        else 
        {
            *counter = 1;
            arr = malloc(*counter * sizeof(int));
            arr[*counter - 1] = m;
            *counter = 0;
        }
    }
    else
    {
        return 0;
    }
    return arr;
}
