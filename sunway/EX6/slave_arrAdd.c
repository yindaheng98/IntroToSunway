#include <stdio.h>
#include <math.h>
#include <string.h>
#include "slave.h"
#define X 20
#define Y 400
#define D 250

typedef struct
{
    double a[Y];
    double b[Y];
} Arry;

__thread_local int my_id;
__thread_local volatile unsigned long get_reply, put_reply;
__thread_local double part_1_slave[D], part_2_slave[D], part_3_slave[D];
extern Arry part_1[X], part_2[X], part_3[X];

void func()
{
    int i, place, s = sizeof(double);
    my_id = athread_get_id(-1);
    place = my_id * D;
    get_reply = 0;
    put_reply = 0;
    athread_get(PE_MODE, (&part_1[0].a[0]) + place, part_1_slave, D * s, &get_reply, 0, 0, 0);
    athread_get(PE_MODE, (&part_2[0].a[0]) + place, part_2_slave, D * s, &get_reply, 0, 0, 0);
    while (get_reply != 2)
        ;
    for (i = 0; i < D; i++)
        part_3_slave[i] = part_1_slave[i] + part_2_slave[i];
    athread_put(PE_MODE, part_3_slave, (&part_3[0].a[0]) + place, D * s, &put_reply, 0, 0);
    while (put_reply != 1)
        ;
}