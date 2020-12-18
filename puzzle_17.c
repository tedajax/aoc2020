#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#include "xxhash.h"

const char* input = ".#..####"
                    ".#.#...#"
                    "#..#.#.#"
                    "###..##."
                    "..##...#"
                    "..##.###"
                    "#.....#."
                    "..##..##";

const XXH64_hash_t K_SEED = 0xf00d;

enum { K_DIM = 4 };

typedef int32_t point[K_DIM];

struct cell {
    uint64_t key;
    bool value;
    point pos;
};

struct world_state {
    struct bounds {
        point min;
        point max;
    } bounds;

    struct cell* cell_map;
};

struct world_state state[2];
int active_state = 0;

void grow_bounds(struct world_state* world, point pos)
{
    for (int i = 0; i < K_DIM; ++i) {
        if (pos[i] < world->bounds.min[i]) {
            world->bounds.min[i] = pos[i];
        }
        if (pos[i] > world->bounds.max[i]) {
            world->bounds.max[i] = pos[i];
        }
    }
}

void set_cell(struct world_state* world, point pos, bool value)
{
    grow_bounds(world, pos);
    XXH64_hash_t hash = XXH64(pos, sizeof(point), K_SEED);
    struct cell c = (struct cell){
        .key = hash,
        .value = value,
    };
    memcpy(&c.pos, pos, sizeof(point));
    hmputs(world->cell_map, c);
}

bool try_get_cell(struct world_state* world, point pos, bool* out)
{
    XXH64_hash_t hash = XXH64(pos, sizeof(point), K_SEED);
    if (hmgeti(world->cell_map, hash) >= 0) {
        if (out) *out = hmgets(world->cell_map, hash).value;
        return true;
    }
    return false;
}

void populate_neighbors(struct world_state* world)
{
    struct world_state new = {0};

    for (int i = 0; i < hmlen(world->cell_map); ++i) {
        set_cell(&new, world->cell_map[i].pos, world->cell_map[i].value);
    }

    for (int i = 0; i < hmlen(world->cell_map); ++i) {
        point pos;
        memcpy(pos, world->cell_map[i].pos, sizeof(point));
        bool val = false;
        point offset = {0};

        if (try_get_cell(world, pos, &val) && val) {
            for (int32_t x = -1; x <= 1; ++x) {
                for (int32_t y = -1; y <= 1; ++y) {
                    for (int32_t z = -1; z <= 1; ++z) {
                        for (int32_t w = -1; w <= 1; ++w) {
                            if (x == 0 && y == 0 && z == 0 && w == 0) {
                                continue;
                            }
                            point check = {pos[0] + x, pos[1] + y, pos[2] + z, pos[3] + w};
                            if (!try_get_cell(world, check, NULL)) {
                                set_cell(&new, check, false);
                            }
                        }
                    }
                }
            }
        }
    }

    hmfree(world->cell_map);
    memcpy(world, &new, sizeof(struct world_state));
}

int count_neighbors(struct world_state* world, point pos)
{
    int count = 0;
    for (int32_t x = -1; x <= 1; ++x) {
        for (int32_t y = -1; y <= 1; ++y) {
            for (int32_t z = -1; z <= 1; ++z) {
                for (int32_t w = -1; w <= 1; ++w) {
                    if (x == 0 && y == 0 && z == 0 && w == 0) {
                        continue;
                    }
                    point check = {pos[0] + x, pos[1] + y, pos[2] + z, pos[3] + w};
                    bool val = false;
                    if (try_get_cell(world, check, &val) && val) ++count;
                }
            }
        }
    }
    return count;
}

void clear_state(struct world_state* world)
{
    memset(&world->bounds, 0, sizeof(point) * 2);
    hmfree(world->cell_map);
}

int count_cubes(struct world_state* world)
{
    int count = 0;
    for (int i = 0; i < hmlen(world->cell_map); ++i) {
        if (world->cell_map[i].value) count++;
    }
    return count;
}

int main(int argc, char* argv[])
{
    char* c = (char*)input;
    int32_t x = 0, y = 0;
    while (*c) {
        if (x >= 8) {
            ++y;
            x = 0;
        }

        point p = {0};
        p[0] = x;
        p[1] = y;
        set_cell(&state[0], p, (*c == '#'));
        c++;
        x++;
    }

    for (int i = 0; i < 6; ++i) {
        int next_state = (active_state + 1) % 2;
        populate_neighbors(&state[active_state]);
        clear_state(&state[next_state]);
        for (int i = 0; i < hmlen(state[active_state].cell_map); ++i) {
            point pos;
            memcpy(pos, state[active_state].cell_map[i].pos, sizeof(point));
            int count = count_neighbors(&state[active_state], pos);
            bool val = false;
            if (try_get_cell(&state[active_state], pos, &val) && val) {
                set_cell(&state[next_state], pos, count == 2 || count == 3);
            } else {
                set_cell(&state[next_state], pos, count == 3);
            }
        }
        active_state = (active_state + 1) % 2;
    }

    printf("%d\n", count_cubes(&state[active_state]));

    getc(stdin);

    return 0;
}