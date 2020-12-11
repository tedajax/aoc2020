#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int int_cmp(const void* a, const void* b)
{
    if (*(int*)a < *(int*)b) {
        return -1;
    } else if (*(int*)a > *(int*)b) {
        return 1;
    } else {
        return 0;
    }
}

enum { N = 101 };

struct node {
    int val;
    struct node* children[3];
};

struct node node_pool[N + 1] = {0};
int node_idx = 0;
int numbers[N];
struct node* node_map[1024] = {0};
uint64_t val_map[1024] = {0};

struct node* get(int val)
{
    if (val > 0) {
        if (node_map[val] != NULL) {
            return node_map[val];
        }
    } else {
        return node_map[0];
    }

    if (node_idx > N) {
        printf("OH FUCK\n");
        return NULL;
    }

    struct node* new = &node_pool[node_idx++];
    node_map[val] = new;
    new->val = val;
    return new;
}

uint64_t count(struct node* node)
{
    if (val_map[node->val] > 0) {
        return val_map[node->val];
    } else {
        if (node->children[0] == NULL && node->children[1] == NULL && node->children[2] == NULL) {
            return 1;
        } else {
            uint64_t sum = 0;
            for (int i = 0; i < 3; ++i) {
                if (node->children[i] != NULL) {
                    sum += count(node->children[i]);
                }
            }
            val_map[node->val] = sum;
            printf("%03d %llu\n", node->val, sum);
            return sum;
        }
    }
}

int main(int argc, char* argv[])
{
    FILE* file = fopen("input_10.txt", "r");

    int idx = 0;
    while (idx < N && fscanf(file, "%d\n", &numbers[idx++]) == 1) {
    }
    fclose(file);

    qsort(numbers, N, sizeof(int), int_cmp);

    struct node* new = &node_pool[node_idx++];
    node_map[0] = new;
    new->val = 0;

    for (int i = 0; i < N; ++i) {
        get(numbers[i]);
    }
    int counts[3] = {0};

    for (int i = 0; i < 3; ++i) {
        int diff = numbers[i];
        if (diff <= 3) {
            get(0)->children[diff - 1] = get(numbers[i]);
        }
    }

    for (int i = 0; i < N - 1; ++i) {
        for (int j = 1; j <= 3 && i + j < N; ++j) {
            int diff = numbers[i + j] - numbers[i];
            if (diff <= 3) {
                counts[diff - 1]++;
                get(numbers[i])->children[diff - 1] = get(numbers[i + j]);
            }
        }
    }

    uint64_t c = count(new);

    printf("%d %d %d\n", counts[0], counts[1], counts[2]);
    printf("%llu\n", c);

    getc(stdin);

    return 0;
}