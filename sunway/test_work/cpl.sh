sw5cc -host -c master.c
sw5cc -slave -c slave.c
sw5cc -hybrid master.o slave.o -o test

