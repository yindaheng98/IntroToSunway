#include <stdlib.h>
#include <stdio.h>
#include <athread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

extern SLAVE_FUN(func)();
extern SLAVE_FUN(funcs)();

#define X 10
#define Y 400
#define Z 30

int A[X][Y][Z], B[X][Y][Z], C[X][Y][Z], e;

void init()
{
    int i, j, k;
    e = 10;
    for (i = 0; i < X; i++)
        for (j = 0; j < Y; j++)
            for (k = 0; k < Z; k++)
            {
                A[i][j][k] = i + j + k;
                B[i][j][k] = i + j + k + 1;
                C[i][j][k] = 0;
            }
    printf("Init finished\n");
}

int main(void)
{
    int i, j, k;
    struct timeval start, end;
    double time_use;

    init();
    printf("No boost proceed:\n");
    gettimeofday(&start, NULL);
    for (i = 0; i < X; i++)
        for (j = 0; j < Y; j++)
            for (k = 0; k < Y; k++)
                C[i][j][k] = A[i][j][k] + B[i][j][k] + e;
    gettimeofday(&end, NULL);
    time_use = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("Time usage is %lf us\n", time_use);
    printf("(C[5][5][5], C[9][399][29]) = (%d, %d)\n", C[5][5][5], C[9][399][29]);

    init();
    athread_init();
    printf("Boosted proceed:\n");
    gettimeofday(&start, NULL);
    athread_spawn(func, 0);
    athread_join();
    gettimeofday(&end, NULL);
    time_use = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("Time usage is %lf us\n", time_use);
    printf("(C[5][5][5], C[9][399][29]) = (%d, %d)\n", C[5][5][5], C[9][399][29]);
    athread_halt();
    return 0;
}
