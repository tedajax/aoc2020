#include <stdbool.h>
#include <stdio.h>
#include <string.h>

enum {
    WIDTH = 90,
    HEIGHT = 98,
};

typedef char seatmap[HEIGHT][WIDTH + 1];

seatmap seats = {0};

bool valid(int x, int y)
{
    return !(x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT);
}

// dir 0-7 starts 1, 0, moves counter clockwise
int vel[16] = {1, 0, 1, -1, 0, -1, -1, -1, -1, 0, -1, 1, 0, 1, 1, 1};
bool cast(int x, int y, int dir)
{
    int vx = vel[dir * 2];
    int vy = vel[dir * 2 + 1];

    int px = x, py = y;
    while (valid(px, py)) {
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
            int seat_score = score(x, y);
            if (seats[y][x] == 'L' && seat_score == 0) {
                new[y][x] = '#';
                changed = true;
            } else if (seats[y][x] == '#' && seat_score >= 5) {
                new[y][x] = 'L';
                changed = true;
            } else {
                new[y][x] = seats[y][x];
            }
        }
    }

    memcpy(seats, new, sizeof(seatmap));

    return changed;
}

void print_seats(void)
{
    for (int y = 0; y < HEIGHT; ++y) {
        printf("%s\n", seats[y]);
    }
}

int main(int argc, char* argv[])
{
    int row = 0;
    FILE* file = fopen("input_11.txt", "r");
    while (fscanf(file, "%s\n", &seats[row++]) == 1) {
        printf("%s\n", seats[row - 1]);
    }
    fclose(file);

    while (next()) {
    }

    int count = 0;
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (seats[y][x] == '#') count++;
        }
    }

    print_seats();
    printf("%d\n", count);

    getc(stdin);

    return 0;
}