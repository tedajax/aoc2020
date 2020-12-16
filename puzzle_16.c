#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define STRPOOL_U32 uint32_t
#define STRPOOL_U64 uint64_t
#define STRPOOL_IMPLEMENTATION
#include "strpool.h"

enum { FIELD_COUNT = 20 };

struct rule {
    uint64_t key;
    struct {
        int min;
        int max;
    } ranges[2];
}* rules = NULL;
int** tickets = NULL;
int** valid_tickets = NULL;

int passes_rule(struct rule* r, int t)
{
    return (t >= r->ranges[0].min && t <= r->ranges[0].max)
           || (t >= r->ranges[1].min && t <= r->ranges[1].max);
}

struct field {
    int idx;
    uint32_t mask;
};

int count_bits(uint32_t mask)
{
    int bits = 0;
    for (int i = 0; i < FIELD_COUNT; ++i) {
        if ((mask & (1ul << i)) != 0) {
            ++bits;
        }
    }
    return bits;
}

int bit_count_sort_cmp(const void* a, const void* b)
{
    int abits = count_bits(((struct field*)a)->mask);
    int bbits = count_bits(((struct field*)b)->mask);
    if (abits < bbits) {
        return -1;
    } else if (abits > bbits) {
        return 1;
    } else {
        return 0;
    }
}

int lowest_bit(uint32_t mask, uint32_t avail)
{
    for (int i = 0; i < FIELD_COUNT; ++i) {
        if ((mask & (1ul << i)) != 0 && (avail & (1ul << i)) != 0) {
            return i;
        }
    }
    return -1;
}

void field_mask_str(uint32_t mask, char dest[FIELD_COUNT + 1])
{
    int idx = 0;
    for (int i = 0; i < FIELD_COUNT; ++i) {
        dest[idx++] = ((mask & (1ul << i)) != 0) ? '1' : '0';
    }
    dest[idx] = 0;
}

int main(int argc, char* argv[])
{
    strpool_t pool;
    strpool_init(&pool, NULL);

    arrsetcap(rules, FIELD_COUNT);

    FILE* file = fopen("input_16.txt", "r");
    char line[256];
    int read_mode = 0;
    while (fgets(line, 256, file)) {
        if (read_mode == 0) {
            if (memcmp(line, "tickets:", 8) == 0) {
                read_mode = 1;
            } else {
                char name[128];
                int nums[4];
                if (sscanf(
                        line, "%s %d-%d or %d-%d\n", &name, &nums[0], &nums[1], &nums[2], &nums[3])
                    != 5) {
                    printf("failure\n");
                    return 1;
                }
                arrput(
                    rules,
                    ((struct rule){
                        .key = strpool_inject(&pool, name, strlen(name)),
                        .ranges =
                            {
                                [0] = {.min = nums[0], .max = nums[1]},
                                [1] = {.min = nums[2], .max = nums[3]},
                            },
                    }));
            }
        } else if (read_mode == 1) {
            int* ticket = NULL;
            arrsetcap(ticket, FIELD_COUNT);
            arrput(tickets, ticket);
            char* token = strtok(line, ",");
            while (token) {
                arrput(ticket, strtol(token, NULL, 10));
                token = strtok(NULL, ",");
            }
        }
    }
    fclose(file);

    for (int i = 0; i < arrlen(rules); ++i) {
        printf(
            "%s %d-%d or %d-%d\n",
            strpool_cstr(&pool, rules[i].key),
            rules[i].ranges[0].min,
            rules[i].ranges[0].max,
            rules[i].ranges[1].min,
            rules[i].ranges[1].max);
    }

    for (int i = 0; i < arrlen(tickets); ++i) {
        for (int j = 0; j < arrlen(tickets[i]); ++j) {
            printf("%d%c", tickets[i][j], (j == (arrlen(tickets[i]) - 1)) ? ' ' : ',');
        }
        printf("\n");
    }

    for (int i = 1; i < arrlen(tickets); ++i) {
        int valid_ticket = 1;
        for (int j = 0; j < arrlen(tickets[i]); ++j) {
            int valid = 0;
            int t = tickets[i][j];
            for (int k = 0; k < arrlen(rules); ++k) {
                struct rule* r = &rules[k];
                if (passes_rule(r, t)) {
                    valid = 1;
                    break;
                }
            }
            if (!valid) {
                valid_ticket = 0;
                break;
            }
        }
        if (valid_ticket) {
            arrput(valid_tickets, tickets[i]);
        }
    }

    printf("%d\n", arrlen(rules));

    uint32_t mask = 0;
    for (int j = 0; j < FIELD_COUNT; ++j) {
        mask |= (1ul << j);
    }
    struct field fields[FIELD_COUNT];
    for (int i = 0; i < FIELD_COUNT; ++i) {
        fields[i] = (struct field){
            .idx = i,
            .mask = mask,
        };
    }

    for (int i = 0; i < arrlen(valid_tickets); ++i) {
        for (int j = 0; j < arrlen(valid_tickets[i]); ++j) {
            int field = valid_tickets[i][j];
            for (int k = 0; k < arrlen(rules); ++k) {
                if (!passes_rule(&rules[k], field)) {
                    fields[j].mask &= ~(1ul << k);
                }
            }
        }
    }

    qsort(fields, FIELD_COUNT, sizeof(struct field), bit_count_sort_cmp);

    uint32_t available = 0xFFFFF;
    int* ids = NULL;

    for (int i = 0; i < FIELD_COUNT; ++i) {
        int bit = lowest_bit(fields[i].mask, available);
        available &= ~(1ul << bit);
        char field_str[FIELD_COUNT + 1];
        field_mask_str(fields[i].mask, field_str);
        printf(
            "%d %d %05x %s %d %s\n",
            count_bits(fields[i].mask),
            fields[i].idx,
            fields[i].mask,
            field_str,
            bit,
            strpool_cstr(&pool, rules[bit].key));

        if (memcmp(strpool_cstr(&pool, rules[bit].key), "departure", 9) == 0) {
            arrput(ids, fields[i].idx);
        }
    }

    long long product = 1;
    for (int i = 0; i < arrlen(ids); ++i) {
        printf("%d %d %d\n", i, ids[i], tickets[0][ids[i]]);
        product *= tickets[0][ids[i]];
    }
    printf("product: %lld\n", product);

    getc(stdin);

    for (int i = 0; i < arrlen(tickets); ++i) {
        arrfree(tickets[i]);
    }
    arrfree(tickets);
    arrfree(rules);

    strpool_term(&pool);

    return 0;
}