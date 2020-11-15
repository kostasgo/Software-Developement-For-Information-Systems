CC = gcc
EXEC = disambugator
SRCS = main.c specs.c lowlevel-io.c parser.c clique.c
HDRS = specs.h lowlevel-io.h parser.h clique.h
OBJS = main.o specs.o lowlevel-io.o parser.o clique.o
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

clique.o:
	$(CC) $(FLAGS) clique.c

parser.o:
	$(CC) $(FLAGS) parser.c

.PHONY : clean info

clean :
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)

count:
	wc $(SRCS) $(HDRS)

info :
	@echo Konstantinos Gkogkas, 1115201200027
