#include <stdio.h>
#include <stdlib.h>

int play_game(int* numbers, size_t len, int max_turn)
{
    int* map = (int*)calloc(max_turn, sizeof(int));

    int turn = 1;
    int spoken = 0;
    while (turn <= max_turn) {
        int prev = spoken;

        if (turn <= len) {
            spoken = numbers[turn - 1];
        } else {
            if (map[spoken] == 0) {
                spoken = 0;
            } else {
                spoken = (turn - 1) - map[spoken];
            }
        }

        map[prev] = turn++ - 1;
    }

    free(map);

    return spoken;
}

int main(int argc, char* argv[])
{
    enum { LEN = 7 };
    int numbers[LEN] = {7, 14, 0, 17, 11, 1, 2};

    printf("2020: %d\n", play_game(numbers, LEN, 2020));
    printf("30000000: %d\n", play_game(numbers, LEN, 30000000));
    getc(stdin);

    return 0;
}