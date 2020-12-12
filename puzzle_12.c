#include <stdio.h>
#include <stdlib.h>

struct command {
    char action;
    int value;
};

int main(int argc, char* argv[])
{
    struct command commands[1024];
    int count = 0;

    FILE* file = fopen("input_12.txt", "r");
    char ina;
    int inv;
    while (fscanf(file, "%c%d\n", &ina, &inv) == 2) {
        commands[count] = (struct command){
            .action = ina,
            .value = inv,
        };
        count++;
    }
    fclose(file);

    int sx = 0, sy = 0;
    int wx = 10, wy = 1;

    char cc[4] = {'E', 'N', 'W', 'S'};

    for (int i = 0; i < count; ++i) {
        struct command cmd = commands[i];

        switch (cmd.action) {
        case 'N':
            wy += cmd.value;
            break;
        case 'S':
            wy -= cmd.value;
            break;
        case 'E':
            wx += cmd.value;
            break;
        case 'W':
            wx -= cmd.value;
            break;
        case 'R':
            for (int i = 0; i < cmd.value / 90; ++i) {
                int x0 = wy;
                int y0 = -wx;
                wx = x0;
                wy = y0;
            }
            break;
        case 'L':
            for (int i = 0; i < cmd.value / 90; ++i) {
                int x0 = -wy;
                int y0 = wx;
                wx = x0;
                wy = y0;
            }
            break;
        case 'F':
            sx += wx * cmd.value;
            sy += wy * cmd.value;
        default:
            break;
        }
    }

    printf("%d\n", abs(sx) + abs(sy));
    getc(stdin);

    return 0;
}