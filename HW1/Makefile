# define the C compiler to use
CC = gcc

# define any compile-time flags
CFLAGS = -Wall -g

# define the C source files
SRCS = memgrind.c mymalloc.c testmalloc.c

# define the C object files 
OBJS = $(SRCS:.c=.o)

# define the executable file 
MAIN = memgrind testmalloc

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $@.o mymalloc.o

mymalloc.o: mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -c mymalloc.c

memgrind.o: memgrind.c mymalloc.h
	$(CC) $(CFLAGS) -c memgrind.c

testmalloc.o: testmalloc.c mymalloc.h
	$(CC) $(CFLAGS) -c testmalloc.c

clean:
	$(RM) *.o *~ $(MAIN)
