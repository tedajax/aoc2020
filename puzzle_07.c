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
};

struct color_held
{
    color key;
    struct color_item *held_by;
};

struct color_held *held_map;

int main(int argc, char *argv[])
{
    strpool_t pool;
    strpool_init(&pool, NULL);

    FILE *file = fopen("input_07.txt", "r");

    char *search_str = "shiny gold";
    color search = strpool_inject(&pool, search_str, strlen(search_str));

    char buffer[512] = {0};
    while (fgets(buffer, 512, file))
    {
        char *holder = strtok(buffer, ":");
        color holder_color = strpool_inject(&pool, holder, strlen(holder));

        if (search == holder_color)
        {
            int p = 0;
        }
        printf("%s\n", holder);

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
                //long value = strtol(curr, &num_end, 10);
                color curr_color = strpool_inject(&pool, num_end + 1, len - (num_end - curr + 1));
                if (hmgeti(held_map, curr_color) < 0)
                {
                    hmputs(held_map, ((struct color_held){.key = curr_color}));
                }
                struct color_held *held = hmgetp(held_map, curr_color);
                hmputs(held->held_by, ((struct color_item){.key = holder_color}));
                printf("%s held by %s\n", strpool_cstr(&pool, curr_color), holder);
                curr = strtok(NULL, ",");
            }
        }
    }

    color *stack = NULL;
    struct color_item *visited = NULL;
    arrput(stack, strpool_inject(&pool, "shiny gold", strlen("shiny gold")));
    int count = -1;
    while (arrlen(stack) > 0)
    {
        color curr = arrpop(stack);
        printf("%s\n", strpool_cstr(&pool, curr));
        ++count;
        if (hmgeti(held_map, curr) >= 0)
        {
            struct color_item *held_by = hmgets(held_map, curr).held_by;
            printf("%s held by", strpool_cstr(&pool, curr));
            for (int i = 0; i < hmlen(held_by); ++i)
            {
                if (hmgeti(visited, held_by[i].key) < 0)
                {
                    arrput(stack, held_by[i].key);
                    hmputs(visited, ((struct color_item){.key = held_by[i].key}));
                    printf(" %s", strpool_cstr(&pool, held_by[i].key));
                }
            }
            printf("\n");
        }
    }

    printf("%d\n", count);

    getc(stdin);

    return 0;
}