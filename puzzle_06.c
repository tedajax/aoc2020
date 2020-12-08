#include <stdio.h>
#include <string.h>

int add_sum(int answered[26], int group_size, int *sum)
{
    int count = 0;
    for (int i = 0; i < 26; ++i)
    {
        if (answered[i] >= group_size)
        {
            count++;
        }
    }
    *sum += count;
    return count;
}

int main(int argc, char *argv[])
{
    FILE *file = fopen("input_06.txt", "r");
    if (!file)
    {
        printf("Failed to open input file.\n");
        return 0;
    }

    int sum = 0;
    int answered[26] = {0};
    char buffer[28] = {0};
    int group = 0;
    int group_size = 0;
    while (fgets(buffer, 28, file))
    {
        if (buffer[0] == '\n')
        {
            int count = add_sum(answered, group_size, &sum);
            printf("Group %d - %d\n", group++, count);
            group_size = 0;
            memset(answered, 0, sizeof(int) * 26);
        }
        else
        {
            group_size++;
            char *c = buffer;
            while (*c >= 'a' && *c <= 'z')
            {
                answered[*c - 'a'] += 1;
                c++;
            }
        }
    }

    int count = add_sum(answered, group_size, &sum);
    printf("Group %d - %d\n", group++, count);

    printf("%d\n", sum);

    fclose(file);
    return 0;
}