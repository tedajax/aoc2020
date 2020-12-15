#include <stdio.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

struct entry {
    int key;
    int last_turn;
};

int main(int argc, char* argv[])
{
    enum { LEN = 7 };
    int numbers[LEN] = {7, 14, 0, 17, 11, 1, 2};
    struct {
        int key;
        int value;
    } * map[2] = {NULL};

    int turn = 1;
    int spoken = 0;
    int was_first = 1;
    for (int i = 0; i < LEN; ++i) {
        spoken = numbers[i];
        hmput(map[0], numbers[i], turn);
        ++turn;
    }

    while (turn <= 30000000) {
        if (hmgeti(map[1], spoken) < 0) {
            spoken = 0;
        } else {
            spoken = hmget(map[0], spoken) - hmget(map[1], spoken);
        }

        if (hmgeti(map[0], spoken) >= 0) {
            hmput(map[1], spoken, hmget(map[0], spoken));
        }
        hmput(map[0], spoken, turn);

        if (turn % 10000 == 0) {
            printf("  %02.2f%%\r", turn / 300000.0f);
        }

        ++turn;
    }

    printf("\n%d\n", spoken);
    getc(stdin);

    hmfree(map[0]);
    hmfree(map[1]);

    return 0;
}