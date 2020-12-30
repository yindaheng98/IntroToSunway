EXEC=arrAdd
OBJS=master_arrAdd.o slave_arrAdd.o

OPT=-I -b
OPT+=-q q_sw_expr
OPT+=-n 1
OPT+=-cgsp 64

$(EXEC):$(OBJS)
	sw5cc -hybrid $(OBJS) -o $(EXEC)
master_arrAdd.o:master_arrAdd.c
	sw5cc -host -fPIC -c master_arrAdd.c -lm -pthread
slave_arrAdd.o:slave_arrAdd.c
	sw5cc -slave -c slave_arrAdd.c
clean:
	-rm $(OBJS) $(EXEC)
run:$(EXEC)
	bsub $(OPT) ./$(EXEC)
