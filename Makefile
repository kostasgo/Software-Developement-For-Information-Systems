CC = gcc
EXEC = disambugator
SRCS = main.c specs.c lowlevel-io.c
HDRS = specs.h lowleve-io.h
OBJS = main.o specs.o lowlevel-io.o
FLAGS = -g3 -c
#ARGS =

$(EXEC) : $(OBJS)
	$(CC) -o $(EXEC) $(OBJS) -lm

main.o:
	$(CC) $(FLAGS) main.c

specs.o:
	$(CC) $(FLAGS) specs.c

lowlevel-io.o:
	$(CC) $(FLAGS) lowlevel-io.c

.PHONY : clean info

clean :
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)

count:
	wc $(SRCS) $(HDRS)

info :
	@echo Konstantinos Gkogkas, 1115201200027
