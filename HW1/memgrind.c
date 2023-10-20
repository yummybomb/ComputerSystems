#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mymalloc.h"

void test1() {
   for (int i = 0; i < 120; i++) {
      char *ptr = malloc(1);  // Allocate 1 byte of memory
      free(ptr);
   }

   printf("TEST 1\n");
   printf("MemClear?: %d\n", memCleared());  // Check if memory is cleared
}

void test2() {
   char *ptrArray[120];  // Array to store 120 pointers

   for (int i = 0; i < 120; i++) {
      ptrArray[i] =
          malloc(1);  // Allocate 1 byte of memory and store the address
   }

   for (int i = 0; i < 120; i++) {
      free(ptrArray[i]);  // Release the memory
      printf("%p\n", ptrArray[i]);
   }
   printf("TEST 2\n");
   printf("MemClear?: %d\n\n", memCleared());  // Check if memory is cleared
}

void test3() {
   char *ptrArray[120];       // Array to store 120 pointers
   int allocated[120] = {0};  // Initialize the memory allocation status array
   int loc = 0;               // Current location

   for (int i = 0; i < 120; i++) {
      if (loc == 0 || (rand() % 2 == 0 && loc < 120)) {
         // Allocate 1 byte of memory and store the address
         printf("alloc loc=%d\n", loc);
         ptrArray[loc] = malloc(1);
         allocated[loc] = 1;
         loc++;
      } else {
         // Release the memory
         loc--;
         printf("free loc=%d\n", loc);
         free(ptrArray[loc]);
         allocated[loc] = 0;
      }
   }

   printf("Process is done.\n");

   // Clean up any unreleased memory
   for (int i = 0; i < 120; i++) {
      if (allocated[i] == 1) {
         free(ptrArray[i]);
      }
   }
   printf("TEST 3\n");
   printf("MemClear?: %d\n", memCleared());  // Check if memory is cleared
}

void test5() {
    int sizes[] = {1, 10, 40, 1000, 2096, 500, 32, 4, 36,199};
    // Shuffle the sizes array utilizing fisher yates
    for (int i = sizeof(sizes)/sizeof(int) - 1; i > 0; i--) {
    int j = rand() % (i + 1);
    int temp = sizes[i];
    sizes[i] = sizes[j];
    sizes[j] = temp;
}
   void* pointers[sizeof(sizes)/sizeof(int)];
   // Malloc the shuffled size array and put into the ptr array
   for (int i = 0; i < sizeof(sizes)/sizeof(int); i++) {
      pointers[i] = malloc(sizes[i]);
   }

      // Shuffle the ptr array utilizing fisher yates
      for (int i = sizeof(sizes)/sizeof(int) - 1; i > 0; i--) {
         int j = rand() % (i + 1);
         void* temp = pointers[i];
         pointers[i] = pointers[j];
         pointers[j] = temp;
   }

      // Deallocate the shuffled ptr array
      for (int i = 0; i < sizeof(sizes)/sizeof(int); i++) {
         free(pointers[i]);
      }
        printf("TEST 5\n");
      printf("MemClear?: %d\n", memCleared());  // Check if memory is cleared

}

int main() {
   test1();
   test5();
}