#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include "mymalloc.h"

void test1() {
  char *ptr = malloc(1); // Allocate 1 byte of memory
  printf("%p", ptr);
  printf("MemClear?: %d\n", memCleared()); // Check if memory is cleared
  printf("%p", heap);
}

int main() { test1(); }