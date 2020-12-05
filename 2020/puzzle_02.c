#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *file = fopen("input_01.txt", "r");
    if (!file)
    {
        printf("Failed to open input file.\n");
        return 0;
    }

    int valid = 0;

    int nmin, nmax;
    char reqc;
    char passwd[256];
    while (fscanf(file, "%d-%d %c: %s\n", &nmin, &nmax, &reqc, &passwd) == 4)
    {
        if (passwd[nmin - 1] == reqc ^ passwd[nmax - 1] == reqc)
            valid++;
    }

    printf("\nResult: %d\n", valid);

    return 0;
}
