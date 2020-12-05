#include <stdio.h>

enum
{
    K_WIDTH = 31
};

int sample(char *buffer, int width, int x, int y)
{
    if (buffer[y * width + x % width] == '#')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int tree_count(char *buffer, int width, int height, int vx, int vy)
{
    int trees = 0;
    int px = 0, py = 0;
    while (py < height)
    {
        if (sample(buffer, K_WIDTH, px, py))
        {
            ++trees;
        }
        px += vx;
        py += vy;
    }
    return trees;
}

int main(int argc, char *argv[])
{
    FILE *file = fopen("input_02.txt", "r");
    if (!file)
    {
        printf("Failed to open input file.\n");
        return 0;
    }

    int line = 0;
    char buffer[500][K_WIDTH] = {0};

    while (fscanf(file, "%s\n", &buffer[line][0]) == 1)
    {
        line++;
    }

    int n[5] = {
        tree_count(&buffer[0][0], K_WIDTH, line, 1, 1),
        tree_count(&buffer[0][0], K_WIDTH, line, 3, 1),
        tree_count(&buffer[0][0], K_WIDTH, line, 5, 1),
        tree_count(&buffer[0][0], K_WIDTH, line, 7, 1),
        tree_count(&buffer[0][0], K_WIDTH, line, 1, 2),
    };

    int result = n[0];
    for (int i = 1; i < 5; ++i)
    {
        result *= n[i];
    }

    printf("\nResult: %d\n", result);

    return 0;
}
