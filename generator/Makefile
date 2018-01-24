
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

default: trajectory-generator

trajectory-generator: $(OBJS)
	gcc -o trajectory_generator $(OBJS)

clean: 
	rm -f trajectory_generator *.o
