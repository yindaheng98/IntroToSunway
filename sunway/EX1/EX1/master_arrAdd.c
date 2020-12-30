#include <stdlib.h>
#include <stdio.h>
#include <athread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

extern SLAVE_FUN(func)();
extern SLAVE_FUN(funcs)();

#define X 64
#define Y 2048

int A[X][Y], B[X][Y], C[X][Y],CC[X][Y];

void init()
{
    int i, j;
    for (i = 0; i < X; i++)
        for (j = 0; j < Y; j++)
        {
            A[i][j] = i + j;
            B[i][j] = i + j + 1;
            C[i][j] = 0;
            CC[i][j]=0;
        }
    printf("Init finished\n");
}

int main(void)
{
    int i, j;
    struct timeval start, end;
    init();
    double time_use;
    printf("No boost proceed:\n");
    gettimeofday(&start, NULL);
    for (i = 0; i < X; i++)
        for (j = 0; j < Y; j++)
            C[i][j] = A[i][j] + B[i][j];
    gettimeofday(&end, NULL);
    time_use = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("Time usage is %lf us\n", time_use);
    printf("(C[32][0], C[63][999]) = (%d, %d)\n", C[32][0], C[63][999]);

    init();
    athread_init();

    printf("Boosted proceed:\n");
    gettimeofday(&start, NULL);
    athread_spawn(func, 0);
    athread_join();
    gettimeofday(&end, NULL);
    time_use = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("Time usage is %lf us\n", time_use);
    printf("(C[32][0], C[63][999]) = (%d, %d)\n", C[32][0], C[63][999]);
/*
    athread_enter64();
    printf("64 Boosted proceed:");
    gettimeofday(&start, NULL);
    athread_spawn64(funcs, 0);
    athread_join64();
    gettimeofday(&end, NULL);
    time_use = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("Time usage is %lf us\n", time_use);
    printf("(C[0][0], C[0][999]) = (%d, %d)\n", CC[0][0], CC[0][999]);
    athread_leave64();
*/
    athread_halt();

    return 0;
}
