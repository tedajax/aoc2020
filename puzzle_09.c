#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    FILE* file = fopen("input_09.txt", "r");

    int data[1000];
    int idx = 0;
    while (fscanf(file, "%d\n", &data[idx++]) == 1) {
    }
    fclose(file);

    int weak_idx = -1;

    for (int i = 25; i < 1000; ++i) {
        int found = 0;
        int start = i - 25;
        int end = i;
        for (int j = start; j < end - 1; ++j) {
            for (int k = start + 1; k < end; ++k) {
                if (data[j] + data[k] == data[i]) {
                    found = 1;
                    goto done;
                }
            }
        }
    done:
        if (!found) {
            weak_idx = i;
            printf("Weak Value: %d\n", data[i]);
            break;
        }
    }

    if (weak_idx < 0) {
        printf("No weakness found\n");
        return 0;
    }

    int weak = data[weak_idx];
    int found_head = -1;
    int found_tail = -1;
    for (int head = 0; head < weak_idx - 1; ++head) {
        int sum = 0;
        for (int tail = head; tail < weak_idx; ++tail) {
            sum += data[tail];
            if (sum > weak) {
                break;
            } else if (sum == weak) {
                found_head = head;
                found_tail = tail;
                break;
            }
        }
        if (found_tail >= 0) {
            printf("Range %d - %d\n", found_head, found_tail);
            break;
        }
    }

    int min_val = 0x7FFFFFFF;
    int max_val = 0xFFFFFFFF;
    for (int i = found_head; i < found_tail; ++i) {
        printf("%03d) %d\n", i, data[i]);
        if (data[i] < min_val) {
            min_val = data[i];
        }
        if (data[i] > max_val) {
            max_val = data[i];
        }
    }

    printf("Min: %d, Max: %d, Sum: %d\n", min_val, max_val, min_val + max_val);

    getc(stdin);

    return 0;
}