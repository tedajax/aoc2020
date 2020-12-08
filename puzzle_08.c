#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum opcode
{
    opcode_nop,
    opcode_acc,
    opcode_jmp,
};

struct instr
{
    enum opcode opcode;
    int value;
};

enum
{
    K_MAX_INSTR = 1024
};

int main(int argc, char *argv[])
{
    FILE *file = fopen("input_08.txt", "r");

    struct instr instructions[K_MAX_INSTR] = {0};
    int instr_count = 0;

    char opbuf[4] = {0};
    int opval = 0;
    while (fscanf(file, "%s %d\n", &opbuf, &opval) == 2)
    {
        enum opcode code = opcode_nop;
        if (strcmp(opbuf, "acc") == 0)
        {
            code = opcode_acc;
        }
        else if (strcmp(opbuf, "jmp") == 0)
        {
            code = opcode_jmp;
        }

        instructions[instr_count++] = (struct instr){
            .opcode = code,
            .value = opval,
        };
    }

    fclose(file);

    int change_idx = 0;

    while (change_idx < instr_count)
    {
        char run[K_MAX_INSTR] = {0};

        if (instructions[change_idx].opcode == opcode_nop)
        {
            instructions[change_idx].opcode = opcode_jmp;
        }
        else if (instructions[change_idx].opcode == opcode_jmp)
        {
            instructions[change_idx].opcode = opcode_nop;
        }
        else
        {
            ++change_idx;
            continue;
        }

        int iptr = 0;
        int acc = 0;
        int cycles = 0;
        int fail = 0;
        while (iptr < instr_count)
        {
            struct instr instr = instructions[iptr];
            if (!run[iptr])
            {
                run[iptr] = 1;
            }
            else
            {
                fail = 1;
                break;
            }
            switch (instr.opcode)
            {
            case opcode_acc:
                acc += instr.value;
            default:
            case opcode_nop:
                iptr++;
                break;
            case opcode_jmp:
                iptr += instr.value;
            }
            ++cycles;
        }

        if (fail)
        {
            printf("HALT : IPTR = %d, ACC = %d, CYC = %d\n", iptr, acc, cycles);
        }
        else
        {
            printf("DONE : ACC = %d, CYC = %d, CID = %d\n", acc, cycles, change_idx);
            break;
        }

        if (instructions[change_idx].opcode == opcode_nop)
        {
            instructions[change_idx].opcode = opcode_jmp;
        }
        else if (instructions[change_idx].opcode == opcode_jmp)
        {
            instructions[change_idx].opcode = opcode_nop;
        }

        ++change_idx;
    }

    getc(stdin);

    return 0;
}
