#include <stdio.h>
#include <stdlib.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

int main(int argc, char *argv[])
{
    FILE *file = fopen("input_00.txt", "r");
    if (!file) {
        printf("Failed to open input file.\n");
        return 0;
    }

    int *numbers = NULL;
    arrsetcap(numbers, 1000);

    int num = 0;

    while (fscanf(file, "%d\n", &num) == 1) {
        arrput(numbers, num);
    }

    fclose(file);

    size_t len = arrlen(numbers);
    int result = 0;

    for (int x = 0; x < len - 1 && result == 0; ++x) {
        int a = numbers[x];

        for (int y = x + 1; y < len && result == 0; ++y) {
            int b = numbers[y];
            if (a + b == 2020) {
                result = a * b;
            }
        }
    }

    if (result > 0) {
        printf("Result: %d\n", result);
    } else {
        printf("No result found for input.\n");
    }

    getc(stdin);

    return 0;
}