#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

enum optype {
    optype_mask,
    optype_mem,
};

typedef struct operation {
    enum optype optype;
    union {
        struct {
            uint64_t mask;
            uint64_t val;
        } mask;
        struct {
            uint64_t addr;
            uint64_t value;
        } mem;
    };
} operation;

uint64_t calc_masked(uint64_t val, uint64_t mask, uint64_t maskval)
{
    uint64_t ret = val;
    for (int i = 0; i < 36; ++i) {
        if ((mask & (1llu << i)) != 0) {
            if ((maskval & (1llu << i)) != 0) {
                ret |= (1llu << i);
            } else {
                ret &= ~(1llu << i);
            }
        }
    }
    return ret;
}

struct mem_block {
    uint64_t key;
    uint64_t val;
};

int mem_write_count = 0;
int mem_alloc_count = 0;
struct mem_block* mem;

void write(struct mem_block* mem, uint64_t addr, uint64_t val)
{
    if (hmgeti(mem, addr) < 0) {
        mem_alloc_count++;
    }
    mem_write_count++;
    hmputs(mem, ((struct mem_block){.key = addr, .val = val}));
}

void write_addrs(struct mem_block* mem, uint64_t addr, uint64_t val, uint64_t mask, uint64_t maskv, int i)
{
    if (i >= 36) {
        write(mem, addr, val);
    } else {
        uint64_t flag = 1llu << i;
        if ((mask & flag) != 0) {
            if ((maskv & flag) == 0) {
                write_addrs(mem, addr, val, mask, maskv, i + 1);
            } else {
                write_addrs(mem, addr | flag, val, mask, maskv, i + 1);
            }
        } else {
            write_addrs(mem, addr & ~flag, val, mask, maskv, i + 1);
            write_addrs(mem, addr | flag, val, mask, maskv, i + 1);
        }
    }
}

int main(int argc, char* argv[])
{
    struct operation ops[1024];
    int op_count = 0;

    arrsetcap(mem, 70000);

    FILE* file = fopen("input_14.txt", "r");

    char line[64] = {0};
    while (fgets(line, 64, file)) {
        // printf(line);
        if (line[1] == 'e') {
            uint64_t addr, value;
            sscanf(line, "mem[%llu] = %llu", &addr, &value);
            ops[op_count++] = (struct operation){
                .optype = optype_mem,
                .mem.addr = addr,
                .mem.value = value,
            };
        } else {
            char maskbuf[37];
            sscanf(line, "mask = %s", maskbuf);

            uint64_t mask = 0;
            uint64_t val = 0;
            for (int i = 0; i < 36; ++i) {
                char c = maskbuf[35 - i];
                if (c != 'X') {
                    mask |= (1llu << i);
                    if (c == '1') {
                        val |= (1llu << i);
                    }
                }
            }

            ops[op_count++] = (operation){
                .optype = optype_mask,
                .mask.mask = mask,
                .mask.val = val,
            };
        }
    }
    fclose(file);

    uint64_t sys_mask = 0, sys_maskv = 0;
    for (int i = 0; i < op_count; ++i) {
        operation op = ops[i];

        switch (op.optype) {
        case optype_mem:
            write_addrs(op.mem.addr, op.mem.value, sys_mask, sys_maskv, 0);
            break;

        case optype_mask:
            sys_mask = op.mask.mask;
            sys_maskv = op.mask.val;
            break;
        }
    }

    uint64_t sum = 0;
    for (int i = 0; i < hmlen(mem); ++i) {
        sum += mem[i].val;
    }

    printf("sum: %llu\n", sum);

    printf("writes: %d\nallocs: %d\n", mem_write_count, mem_alloc_count);

    getc(stdin);

    return 0;
}