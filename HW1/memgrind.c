#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "mymalloc.h"

void test1() {
  for (int i = 0; i < 120; i++) {
    char *ptr = malloc(1); // Allocate 1 byte of memory
    free(ptr);             // Release the memory
  }
  printf("MemClear?: %d\n", memCleared()); // Check if memory is cleared
}

int main() { test1(); }