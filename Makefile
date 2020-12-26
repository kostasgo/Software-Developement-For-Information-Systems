CC = gcc
EXEC = disambugator
TEST = test
SRCS = main.c specs.c lowlevel-io.c parser.c clique.c keyvalue.c hashtable.c create-output.c tests.c bucket.c linkedlist.c negative-cliques.c vocabulary.c
HDRS = specs.h lowlevel-io.h parser.h clique.h keyvalue.h hashtable.h create-output.h acutest.h bucket.h linkedlist.h negative-cliques.h vocabulary.h
OBJS = main.o specs.o lowlevel-io.o parser.o clique.o keyvalue.o hashtable.o create-output.o bucket.o linkedlist.o negative-cliques.o vocabulary.o
OBJS2 = tests.o specs.o clique.o keyvalue.o hashtable.o bucket.o linkedlist.o negative-cliques.o create-output.o
FLAGS = -g3 -c


all : $(OBJS2) $(OBJS)
	$(CC) -o $(TEST) $(OBJS2) -lm
	$(CC) -o $(EXEC) $(OBJS) -lm

main.o:
	$(CC) $(FLAGS) main.c

tests.o:
	$(CC) $(FLAGS) tests.c

specs.o:
	$(CC) $(FLAGS) specs.c

negative-cliques.o:
	$(CC) $(FLAGS) negative-cliques.c

linkedlist.o:
	$(CC) $(FLAGS) linkedlist.c

bucket.o:
	$(CC) $(FLAGS) bucket.c

hashtable.o:
	$(CC) $(FLAGS) hashtable.c

create-output.o:
	$(CC) $(FLAGS) create-output.c

keyvalue.o:
	$(CC) $(FLAGS) keyvalue.c

lowlevel-io.o:
	$(CC) $(FLAGS) lowlevel-io.c

clique.o:
	$(CC) $(FLAGS) clique.c

parser.o:
	$(CC) $(FLAGS) parser.c

.PHONY : clean info

clean :
	rm -f $(OBJS) $(OBJS2) $(EXEC) $(TEST) output.csv

run: $(EXEC)
	valgrind ./$(EXEC)

count:
	wc $(SRCS) $(HDRS)

info :
	@echo Univercity of Athens, Department of Informatics \& Telecomunications
	@echo Konstantinos Gkogkas, 1115201200027
	@echo Nikolaos Sentis, 1115200700156
