#include <stdio.h>
#include <math.h>
#include <string.h>
#include "slave.h"
#define X 64
#define Y 2048

#define Y0 64
#define N 32
//Y0*N=Y

#define L0 256
//L0=Y*4

__thread_local int my_id;
__thread_local volatile unsigned long get_reply[N], put_reply = 0;
extern int A[X][Y], B[X][Y], C[X][Y], CC[X][Y];

__thread_local int A_slave[Y], B_slave[Y], C_slave[Y];

void func()
{
    int i, j, t,tt;
    put_reply=0;
    my_id = athread_get_id(-1);

    for (i = 0; i < N; i++)//输入数据
    {
        get_reply[i]=0;
        t = i * Y0; //当前位置
        athread_get(PE_MODE, &A[my_id][t], &A_slave[t], L0, &get_reply[i], 0, 0, 0);
        athread_get(PE_MODE, &B[my_id][t], &B_slave[t], L0, &get_reply[i], 0, 0, 0);
    }

    for (i = 0; i < N; i++)//计算
    {
        t = i * Y0; //当前起始位置
        while (get_reply[i] != 2)
            ;
        for (j = 0; j < Y0; j++)
        {
            tt=t+j;
            C_slave[tt] = A_slave[tt] + B_slave[tt];
        }
        athread_put(PE_MODE, &C_slave[t], &C[my_id][t], L0, &put_reply, 0, 0);
    }
    while (put_reply != N)
        ;
}