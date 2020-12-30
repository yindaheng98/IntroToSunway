#include <stdlib.h>
#include <stdio.h>
#include <athread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

extern SLAVE_FUN(func)();
extern SLAVE_FUN(funcs)();

#define X 2
#define Y 200
#define Z 100
#define K 4

int A[X][Y][Z][K], B[X][Y][Z][K], C[X][Y][Z][K], e;

void init()
{
    int i, j, k, l;
    e = 10;
    for (i = 0; i < X; i++)
        for (j = 0; j < Y; j++)
            for (k = 0; k < Z; k++)
                for (l = 0; l < K; l++)
                {
                    A[i][j][k][l] = i + j + k + l;
                    B[i][j][k][l] = i + j + k + l + 1;
                    C[i][j][k][l] = 0;
                }
    printf("Init finished\n");
}

int main(void)
{
    int i, j, k, l;
    struct timeval start, end;
    double time_use;

    init();
    printf("No boost proceed:\n");
    gettimeofday(&start, NULL);
    for (i = 0; i < X; i++)
        for (j = 0; j < Y; j++)
            for (k = 0; k < Y; k++)
                for (l = 0; l < K; l++)
                    C[i][j][k][l] = A[i][j][k][l] + B[i][j][k][l] + e + 1;
    gettimeofday(&end, NULL);
    time_use = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("Time usage is %lf us\n", time_use);
    printf("(C[1][1][1], C[1][199][99][3]) = (%d, %d)\n", C[1][1][1], C[1][199][99][3]);

    init();
    athread_init();
    printf("Boosted proceed:\n");
    gettimeofday(&start, NULL);
    athread_spawn(func, 0);
    athread_join();
    gettimeofday(&end, NULL);
    time_use = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("Time usage is %lf us\n", time_use);
    printf("(C[1][1][1], C[1][199][99][3]) = (%d, %d)\n", C[1][1][1], C[1][199][99][3]);
    athread_halt();
    return 0;
}
