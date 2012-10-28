OBJS = parse.o program.o vm.o stack.o
MAIN = main.o
TEST = test.o
CC = gcc
CFLAGS = -g -Wall -O4

main : $(OBJS) $(MAIN)
	$(CC) $(CFLAGS) -o $@ $^

test : $(OBJS) $(TEST)
	$(CC) $(CFLAGS) -o $@ $^

clean :
	rm -f *.o
