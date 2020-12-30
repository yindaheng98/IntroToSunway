#include <stdio.h>
#include <math.h>
#include <string.h>
#include "slave.h"
#define X 10
#define Y 400
#define Z 30
#define D 1875

__thread_local int my_id;
__thread_local volatile unsigned long get_reply, put_reply;
__thread_local int A_slave[D], B_slave[D], C_slave[D], e_slave;
extern int A[X][Y][Z], B[X][Y][Z], C[X][Y][Z], e;

void func()
{
    int i, place;
    my_id = athread_get_id(-1);
    place = my_id * D;
    get_reply = 0;
    put_reply = 0;
    athread_get(PE_MODE, &e, &e_slave, 4, &get_reply, 0, 0, 0);
    athread_get(PE_MODE, (&A[0][0][0]) + place, A_slave, D * 4, &get_reply, 0, 0, 0);
    athread_get(PE_MODE, (&B[0][0][0]) + place, B_slave, D * 4, &get_reply, 0, 0, 0);
    while (get_reply != 3)
        ;
    for (i = 0; i < D; i++)
    {
        C_slave[i] = A_slave[i] + B_slave[i] + e_slave;
    }
    athread_put(PE_MODE, C_slave, (&C[0][0][0]) + place, D * 4, &put_reply, 0, 0);
    while (put_reply != 1)
        ;
}