#include <stdio.h>
#include <string.h>

int map[30000000];

int play_game(int max_turn)
{
    enum { LEN = 7 };
    int numbers[LEN] = {7, 14, 0, 17, 11, 1, 2};

    memset(map, 0, sizeof(map));

    for (size_t i = 0; i < LEN; ++i) {
        map[numbers[i]] = i + 1;
    }

    int turn = LEN;
    int spoken = numbers[LEN - 1];
    while (turn < max_turn) {
        int last = map[spoken];
        map[spoken] = turn;
        spoken = last ? turn - last : 0;
        turn++;
    }

    return spoken;
}

int main(int argc, char* argv[])
{
    printf("2020: %d\n", play_game(2020));
    printf("30000000: %d\n", play_game(30000000));
    getc(stdin);

    return 0;
}