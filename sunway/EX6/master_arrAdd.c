#include <stdlib.h>
#include <stdio.h>
#include <athread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

extern SLAVE_FUN(func)();
extern SLAVE_FUN(funcs)();

#define X 20
#define Y 400

typedef struct
{
    double a[Y];
    double b[Y];
} Arry;

Arry part_1[X], part_2[X], part_3[X];

void init()
{
    int i, j;
    for (i = 0; i < X; i++)
        for (j = 0; j < Y; j++)
        {
            part_1[i].a[j] = i + j;
            part_2[i].a[j] = i + j + 1;
            part_3[i].a[j] = 0;
            part_1[i].b[j] = i + j - 1;
            part_2[i].b[j] = i + j;
            part_3[i].b[j] = 0;
        }
    printf("Init finished\n");
}

int main(void)
{
    int i, j;
    struct timeval start, end;
    double time_use;

    init();
    printf("No boost proceed:\n");
    gettimeofday(&start, NULL);
    for (i = 0; i < X; i++)
        for (j = 0; j < Y; j++)
        {
            part_3[i].a[j] = part_1[i].a[j] + part_2[i].a[j];
            part_3[i].b[j] = part_1[i].b[j] + part_2[i].b[j];
        }
    gettimeofday(&end, NULL);
    time_use = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("Time usage is %lf us\n", time_use);
    printf("(part_3[1].a[1], part_3[19].b[399]) = (%lf, %lf)\n", part_3[1].a[1], part_3[19].b[399]);

    init();
    athread_init();
    printf("Boosted proceed:\n");
    gettimeofday(&start, NULL);
    athread_spawn(func, 0);
    athread_join();
    gettimeofday(&end, NULL);
    time_use = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("Time usage is %lf us\n", time_use);
    printf("(part_3[1].a[1], part_3[19].b[399]) = (%lf, %lf)\n", part_3[1].a[1], part_3[19].b[399]);
    athread_halt();
    return 0;
}
