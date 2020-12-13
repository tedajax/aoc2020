#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    FILE* file = fopen("input_13.txt", "r");
    int arrival;
    char schedule_str[256] = {0};
    fscanf(file, "%d\n%s\n", &arrival, schedule_str);
    fclose(file);

    int buses[9] = {29, 37, 631, 13, 19, 23, 383, 41, 17};

    int min = 0x7FFFFFFF;
    int ret = -1;
    for (int i = 0; i < 9; i++) {
        int bus = buses[i];
        int n0 = (arrival / bus) + 1;
        int n1 = n0 * bus;
        if (n1 < min) {
            min = n1;
            ret = bus * (min - arrival);
        }
    }

    printf("%d\n", ret);
    getc(stdin);

    return 0;
}