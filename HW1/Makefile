# define the C compiler to use
CC = gcc

# define any compile-time flags
CFLAGS = -Wall -g

# define the C source files
SRCS = memgrind.c mymalloc.c

# define the C object files 
OBJS = $(SRCS:.c=.o)

# define the executable file 
MAIN = memgrind

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

mymalloc.o: mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -c mymalloc.c

memgrind.o: memgrind.c mymalloc.h
	$(CC) $(CFLAGS) -c memgrind.c

clean:
	$(RM) *.o *~ $(MAIN)
