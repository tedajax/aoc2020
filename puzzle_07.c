#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define STRPOOL_U32 uint32_t
#define STRPOOL_U64 uint64_t
#define STRPOOL_IMPLEMENTATION
#include "strpool.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

typedef uint64_t color;

struct color_item
{
    color key;
    int count;
};

struct color_holds
{
    color key;
    struct color_item *holds;
};

struct color_holds *hold_map;
strpool_t pool;

int count_bags_helper(color bag, int count, int depth)
{
    struct color_holds hold = hmgets(hold_map, bag);
    int sum = 1;
    for (int i = 0; i < hmlen(hold.holds); ++i)
    {
        int hc = hold.holds[i].count;
        sum += hc * count_bags_helper(hold.holds[i].key, hc, depth + 1);
    }
    for (int i = 0; i <= depth; ++i)
    {
        if (i < depth)
        {
            printf("|");
        }
        else
        {
            printf("-");
        }
    }
    printf("%s(x%d) %d\t\t\t\t\t = %d\n", strpool_cstr(&pool, bag), count, sum, count * sum);
    return sum;
}

int count_bags(char *search)
{
    color bag = strpool_inject(&pool, search, strlen(search));
    return count_bags_helper(bag, 1, 0);
}

int main(int argc, char *argv[])
{
    strpool_init(&pool, NULL);

    FILE *file = fopen("input_07.txt", "r");

    char buffer[512] = {0};
    while (fgets(buffer, 512, file))
    {
        char *holder_str = strtok(buffer, ":");
        color holder_color = strpool_inject(&pool, holder_str, strlen(holder_str));

        hmputs(hold_map, ((struct color_holds){.key = holder_color}));
        struct color_holds *holder = hmgetp(hold_map, holder_color);

        char *contains = strtok(NULL, ":");
        if (contains[0] != '\n')
        {
            char *curr = strtok(contains, ",");
            while (curr)
            {
                size_t len = strlen(curr);
                if (curr[len - 1] == '\n')
                {
                    len--;
                }
                char *num_end = strstr(curr, " ");
                long count = strtol(curr, &num_end, 10);
                color curr_color = strpool_inject(&pool, num_end + 1, len - (num_end - curr + 1));

                hmputs(holder->holds, ((struct color_item){
                                          .key = curr_color,
                                          .count = count,
                                      }));

                curr = strtok(NULL, ",");
            }
        }
    }

    printf("%d\n", count_bags("shiny gold"));

    // |         striped purple(x2) 1
    // |      posh salmon(x4) 1
    // |      plaid cyan(x4) 1
    // |      clear plum(x3) 1
    // |     vibrant green(x4) 12
    // |     dotted purple(x3) 1
    // |     vibrant turquoise(x1) 1
    // |    vibrant indigo(x3) 55
    // |    vibrant turquoise(x4) 1
    // |   dim coral(x5) 170
    // |     striped purple(x2) 1
    // |      posh salmon(x4) 1
    // |      plaid cyan(x4) 1
    // |      clear plum(x3) 1
    // |     vibrant green(x4) 12
    // |     dotted purple(x3) 1
    // |     vibrant turquoise(x1) 1
    // |    vibrant indigo(x4) 55
    // |     pale blue(x1) 1
    // |     clear plum(x4) 1
    // |     wavy yellow(x5) 1
    // |     dotted purple(x1) 1
    // |    pale crimson(x2) 12
    // |    clear plum(x2) 1
    // |   dull gray(x1) 247
    // |     wavy yellow(x5) 1
    // |    muted brown(x1) 6
    // |   posh tomato(x2) 7
    // |  dotted chartreuse(x1) 1112
    // |  pale blue(x2) 1
    // |   wavy yellow(x5) 1
    // |  muted brown(x3) 6
    // |    wavy yellow(x5) 1
    // |   muted brown(x1) 6
    // |  posh tomato(x1) 7
    // | muted orange(x5) 1140
    // |   posh salmon(x4) 1
    // |   plaid cyan(x4) 1
    // |   clear plum(x3) 1
    // |  vibrant green(x5) 12
    // | faded tan(x2) 61
    // |   dotted purple(x5) 1
    // |   shiny brown(x4) 1
    // |  bright bronze(x4) 10
    // | faded orange(x3) 41
    // |  dotted purple(x5) 1
    // |  vibrant turquoise(x5) 1
    // | dull brown(x1) 11
    // shiny gold(x1) 5957

    getc(stdin);

    return 0;
}