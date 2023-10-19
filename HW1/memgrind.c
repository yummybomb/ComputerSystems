#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mymalloc.h"

void test1() {
  for (int i = 0; i < 120; i++) {
    char *ptr = malloc(1); // Allocate 1 byte of memory
    printf("Counter: %d\t Heapstart: %p\t Malloc: %p\n", i, GetHeapStart(),
           ptr);
    free(ptr);
  }

  printf("TEST 1\n");
  printf("MemClear?: %d\n", memCleared()); // Check if memory is cleared
}

void test2() {
  char *ptrArray[120]; // Array to store 120 pointers

  for (int i = 0; i < 120; i++) {
    ptrArray[i] = malloc(1); // Allocate 1 byte of memory and store the address
  }

  for (int i = 0; i < 120; i++) {
    free(ptrArray[i]); // Release the memory
    printf("%p\n", ptrArray[i]);
  }
  printf("TEST 2\n");
  printf("MemClear?: %d\n\n", memCleared()); // Check if memory is cleared
}

int main() {
  test1();
  test2();
}