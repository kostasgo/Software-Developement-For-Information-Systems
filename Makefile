CC = gcc
EXEC = disambugator
SRCS = main.c specs.c
HDRS = specs.h
OBJS = main.o specs.o
FLAGS = -g3 -c
#ARGS =

$(EXEC) : $(OBJS)
	$(CC) -o $(EXEC) $(OBJS) -lm

main.o:
	$(CC) $(FLAGS) main.c

specs.o:
	$(CC) $(FLAGS) specs.c

.PHONY : clean info

clean :
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)

count:
	wc $(SRCS) $(HDRS)

info :
	@echo Konstantinos Gkogkas, 1115201200027
