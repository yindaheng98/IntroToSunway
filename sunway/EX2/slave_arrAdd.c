#include "simd.h"
#include "slave.h"
#define X 64
#define Y 2048

__thread_local int my_id;
__thread_local volatile unsigned long get_reply, put_reply;
__thread_local int A_slave[Y], B_slave[Y], C_slave[Y];
extern int A[X][Y], B[X][Y], C[X][Y];

void func()
{
    int i;
    intv8 v,va;
    my_id = athread_get_id(-1);
    get_reply = 0;
    put_reply = 0;
    athread_get(PE_MODE, &A[my_id][0], A_slave, Y * 4, &get_reply, 0, 0, 0);
    athread_get(PE_MODE, &B[my_id][0], B_slave, Y * 4, &get_reply, 0, 0, 0);
    while (get_reply != 2)
        ;
    for (i = 0; i < Y; i+=8)
    {
        simd_load(va,&(A_slave[i]));
        simd_load(v,&(B_slave[i]));
        v=v+va;
        simd_store(v,&(C_slave[i]));
    }
    athread_put(PE_MODE, C_slave, &C[my_id][0], Y * 4, &put_reply, 0, 0);
    while (put_reply != 1)
        ;
}