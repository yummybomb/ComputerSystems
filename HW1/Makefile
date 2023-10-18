all: mymalloc.o
	-gcc -o memgrind memgrind.c mymalloc.o

mymalloc.o: mymalloc.c
	-gcc -o mymalloc.o -c mymalloc.c

clean:
	-rm *.o ./memgrind