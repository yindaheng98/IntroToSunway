#include <stdlib.h>
#include <stdio.h>
#include <athread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

extern SLAVE_FUN(func)();
extern SLAVE_FUN(funcs)();

#define X 160000

double A[X], SUM[64],sum;

void init()
{
    int i;
    for (i = 0; i < X; i++)
        A[i] = i % 10;
    sum=0;
    printf("Init finished\n");
}

int main(void)
{
    int i;
    struct timeval start, end;
    double time_use;

    init();
    printf("No boost proceed:\n");
    gettimeofday(&start, NULL);
    for (i = 0; i < X; i++)
        sum += A[i];
    sum /= X;
    gettimeofday(&end, NULL);
    time_use = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("Time usage is %lf us\n", time_use);
    printf("average is %lf\n", sum);

    init();
    athread_init();
    printf("Boosted proceed:\n");
    gettimeofday(&start, NULL);
    athread_spawn(func, 0);
    athread_join();
    for (i = 0; i < 64; i++)
        sum += SUM[i];
    sum /= 64;
    gettimeofday(&end, NULL);
    time_use = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("Time usage is %lf us\n", time_use);
    printf("average is %lf\n", sum);
    athread_halt();
    return 0;
}
