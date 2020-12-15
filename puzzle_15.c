#include <stdio.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

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
    int max_turn = 30000000;
    while (turn <= max_turn) {
        if (turn <= LEN) {
            spoken = numbers[turn - 1];
        } else {
            if (hmgeti(map[1], spoken) < 0) {
                spoken = 0;
            } else {
                spoken = hmget(map[0], spoken) - hmget(map[1], spoken);
            }
        }

        if (hmgeti(map[0], spoken) >= 0) {
            hmput(map[1], spoken, hmget(map[0], spoken));
        }
        hmput(map[0], spoken, turn);

        if (max_turn / 100 > 0) {
            if (turn % (max_turn / 100) == 0) {
                printf(
                    " processing........................... %2.0f%%\r", turn * 100.0f / max_turn);
            }
        }

        ++turn;
    }

    printf("\n%d\n", spoken);
    getc(stdin);

    hmfree(map[0]);
    hmfree(map[1]);

    return 0;
}