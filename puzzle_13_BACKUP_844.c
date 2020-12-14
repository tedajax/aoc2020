#include <stdint.h>
#include <stdio.h>

int main(void)
{
    int64_t buses[78] = {29, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  -1,  -1, -1,
                         -1, -1, -1, -1, -1, -1, -1, 37, -1, -1, -1, -1, -1,  631, -1, -1,
                         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  -1,  -1, 13,
                         19, -1, -1, -1, 23, -1, -1, -1, -1, -1, -1, -1, 383, -1,  -1, -1,
                         -1, -1, -1, -1, -1, -1, 41, -1, -1, -1, -1, -1, -1,  17};

    int64_t n[9];
    int64_t a[9];

    int bi = 0;
    for (int i = 0; i < 78; ++i) {
        if (buses[i] >= 0) {
            n[bi] = buses[i];
            a[bi] = i;
            bi++;
        }
    }

    int64_t pos = 0, increment = n[0];
    for (int i = 1; i < bi; ++i) {
        int64_t offset = a[i];
        int64_t time = n[i];
        while ((pos + offset) % time != 0) {
            pos += increment;
        }
        increment *= time;
    }

    printf("%lld\n", pos);

    getc(stdin);

    return 0;
}