sw5cc -host -c master_arrAdd.c
sw5cc -slave -c slave_arrAdd.c
sw5cc -hybrid master_arrAdd.o slave_arrAdd.o -o arrAdd
bsub -I -b -q q_sw_expr -n 1 -cgsp 64 -share_size 4096 -host_stack 128 ./arrAdd
