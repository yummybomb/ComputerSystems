# define the C compiler to use
CC = gcc

# define any compile-time flags
CFLAGS = -Wall -g

# define the C source files
SRCS = hashMap.c words.c

# define the C object files 
OBJS = $(SRCS:.c=.o)

# define the executable file 
MAIN = words

ifdef OS
   RM = del /Q
   FixPath = $(subst /,\,\$1)
else
   ifeq ($(shell uname), Linux)
      RM = rm -f
      FixPath = \$1
   endif
endif

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

hashMap.o: hashMap.c hashMap.h
	$(CC) $(CFLAGS) -c hashMap.c

words.o: words.c
	$(CC) $(CFLAGS) -c words.c

clean:
	$(RM) $(call FixPath, *.o *~ *.exe $(MAIN))