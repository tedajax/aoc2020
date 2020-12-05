#include <stdio.h>
#include <stdlib.h>

#define STB_DS_IMPLEMENTATION
#include "stb/stb_ds.h"

int main(int argc, char *argv[])
{
    FILE *file = fopen("input_00.txt", "r");
    if (!file)
    {
        printf("Failed to open input file.\n");
        return 0;
    }

    int *numbers = NULL;
    arrsetcap(numbers, 1000);

    int num = 0;

    while (fscanf(file, "%d\n", &num) == 1)
    {
        arrput(numbers, num);
    }

    fclose(file);

    size_t len = arrlen(numbers);
    int result = 0;

    for (int x = 0; x < len - 2 && result == 0; ++x)
    {
        int a = numbers[x];

        for (int y = x + 1; y < len - 1 && result == 0; ++y)
        {
            int b = numbers[y];

            for (int z = y + 1; z < len && result == 0; ++z)
            {
                int c = numbers[z];
                if (a + b + c == 2020)
                {
                    result = a * b * c;
                }
            }
        }
    }

    if (result > 0)
    {
        printf("Result: %d\n", result);
    }
    else
    {
        printf("No result found for input.\n");
    }

    return 0;
}