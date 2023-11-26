# define the C compiler to use
CC = gcc

# define any compile-time flags
CFLAGS = -Wall -g

# define the C source files
SRCS = mysh.c

# define the executable file 
MAIN = mysh

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

$(MAIN): 
	$(CC) $(CFLAGS) -o $@ $(SRCS)

clean:
	$(RM) $(call FixPath, *~ *.exe $(MAIN))
