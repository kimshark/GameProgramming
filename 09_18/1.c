#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void selection_sort(int r[], int n);
int main(void)
{
    int i, j, lotto[6];
    srand(time(NULL));
    for (i = 0; i <= 5; i++)
    {
        lotto[i] = rand() % 45 + 1;
        for (j = 0; j < i; j++)
        {
            if (lotto[i] == lotto[j])
            {
                i--;
                break;
            }
        }
    }
    selection_sort(lotto, 6);
    return 0;
}
