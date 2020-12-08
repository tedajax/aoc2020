#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *file = fopen("input_05.txt", "r");
    if (!file)
    {
        printf("Failed to open input file.\n");
        return 0;
    }

    int found[1024] = {0};

    int max_id = 0;
    char buffer[12] = {0};
    while (fscanf(file, "%s\n", &buffer) == 1)
    {
        int row = 0;
        int col = 0;
        for (int i = 0; i < 7; ++i)
        {
            if (buffer[i] == 'B')
            {
                row |= (1 << (6 - i));
            }
        }
        for (int i = 0; i < 3; ++i)
        {
            if (buffer[i + 7] == 'R')
            {
                col |= (1 << (2 - i));
            }
        }
        int id = row * 8 + col;
        if (id > max_id)
        {
            max_id = id;
        }

        found[id] = 1;

        printf("str: %s row: %d col: %d id: %d\n", buffer, row, col, id);
    }

    printf("Highest Id: %d\n", max_id);

    printf("Empty Seats:\n");
    for (int i = 0; i < 1024; ++i)
    {
        if (!found[i])
        {
            printf("%d\n", i);
        }
    }

    fclose(file);
    return 0;
}
