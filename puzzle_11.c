#include <stdbool.h>
#include <stdio.h>
#include <string.h>

enum {
    WIDTH = 90,
    HEIGHT = 98,
};

typedef char seatmap[HEIGHT][WIDTH + 1];

seatmap seats = {0};

// dir 0-7 starts 1, 0, moves counter clockwise
int vel[16] = {1, 0, 1, -1, 0, -1, -1, -1, -1, 0, -1, 1, 0, 1, 1, 1};
bool cast(int x, int y, int dir)
{
    int vx = vel[dir * 2];
    int vy = vel[dir * 2 + 1];

    int px = x, py = y;
    while (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT) {
        px += vx;
        py += vy;
        if (seats[py][px] == 'L') {
            return false;
        } else if (seats[py][px] == '#') {
            return true;
        }
    }
    return false;
}

int score(int x, int y)
{
    int score = 0;
    for (int dir = 0; dir < 8; ++dir)
        if (cast(x, y, dir)) score++;
    return score;
}

bool next(void)
{
    seatmap new;
    memcpy(new, seats, sizeof(seatmap));

    bool changed = false;

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (seats[y][x] == 'L' && score(x, y) == 0) {
                new[y][x] = '#';
                changed = true;
            } else if (seats[y][x] == '#' && score(x, y) >= 5) {
                new[y][x] = 'L';
                changed = true;
            }
        }
    }

    memcpy(seats, new, sizeof(seatmap));

    return changed;
}

int main(int argc, char* argv[])
{
    int row = 0;
    FILE* file = fopen("input_11.txt", "r");
    while (fscanf(file, "%s\n", &seats[row++]) == 1) {
    }
    fclose(file);

    int iter = 0;
    while (next()) {
        ++iter;
    }

    int count = 0;
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (seats[y][x] == '#') count++;
        }
    }

    printf("equilibrium after %d iterations, %d seats occupied.\n", iter, count);
    getc(stdin);

    return 0;
}