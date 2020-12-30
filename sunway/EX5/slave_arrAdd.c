#include <stdio.h>
#include <math.h>
#include <string.h>
#include "slave.h"
#define X 160000
#define D 2500

__thread_local int my_id;
__thread_local volatile unsigned long get_reply, put_reply;
__thread_local double A_slave[D], sum_slave;
extern double A[X], SUM[64];

void func()
{
    int i, s = sizeof(double);
    my_id = athread_get_id(-1);
    get_reply = 0;
    put_reply = 0;
    athread_get(PE_MODE, &A[D * my_id], A_slave, D * s, &get_reply, 0, 0, 0);
    while (get_reply != 1)
        ;
    sum_slave = 0;
    for (i = 0; i < D; i++)
        sum_slave += A_slave[i];
    sum_slave /= D;
    athread_put(PE_MODE, &sum_slave, &SUM[my_id], s, &put_reply, 0, 0);
    while (put_reply != 1)
        ;
}