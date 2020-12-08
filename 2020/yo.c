#include <stdio.h>

int main(int argc,char *argv[])
{

    int yos = 0;

    do
    {
        scanf("%d",&yos);

        if (yos <= 0)
        {
            break;
        }

        for (int i = 1; i <= yos; ++i)
        {
            for (int j = 0; j < i; ++j)
            {
                printf("yoyo");
            }
            printf("\n");
        }

        for (int i = yos - 1; i > 0; --i)
        {
            for (int j = 0; j < i; ++j)
            {
                printf("yoyo");
            }
            printf("\n");
        }
    } while (yos > 0);

    return 0;
}