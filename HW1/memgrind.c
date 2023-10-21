#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "mymalloc.h"

// FORWARD DECLARATIONS
double performanceTest(int testNum, int iterations);

void test1() {
   for (int i = 0; i < 120; i++) {
      char *ptr = malloc(1);  // Allocate 1 byte of memory
      free(ptr);
   }
}

void test2() {
   char *ptrArray[120];  // Array to store 120 pointers

   for (int i = 0; i < 120; i++) {
      ptrArray[i] =
          malloc(1);  // Allocate 1 byte of memory and store the address
   }

   for (int i = 0; i < 120; i++) {
      free(ptrArray[i]);  // Release the memory
   }
}

void test3() {
   char *ptrArray[120];       // Array to store 120 pointers
   int allocated[120] = {0};  // Initialize the memory allocation status array
   int loc = 0;               // Current location

   for (int i = 0; i < 120; i++) {
      if (loc == 0 || (rand() % 2 == 0 && loc < 120)) {
         // Allocate 1 byte of memory and store the address
         ptrArray[loc] = malloc(1);
         allocated[loc] = 1;
         loc++;
      } else {
         // Release the memory
         loc--;
         free(ptrArray[loc]);
         allocated[loc] = 0;
      }
   }

   // Clean up any unreleased memory
   for (int i = 0; i < 120; i++) {
      if (allocated[i] == 1) {
         free(ptrArray[i]);
      }
   }
}

void test4() {
   char *charArr[60];
   int *intArr[60];
   for (int i = 0; i < 120; i++) {
      if (i % 2 == 0) {
         charArr[i / 2] = malloc(sizeof(char *));
      } else {
         intArr[i / 2] = malloc(sizeof(int *));
      }
   }
   for (int i = 0; i < 120; i++) {
      if (i % 2 == 0) {
         free(intArr[i / 2]);
      } else {
         free(charArr[i / 2]);
      }
   }
}

/**
void test5() {
   int sizes[] = {1, 10, 40, 1000, 2096, 500, 32, 4, 36, 199};
   int arrSize = sizeof(sizes) / sizeof(int);
   // Shuffle the sizes array utilizing fisher yates
   for (int i = arrSize - 1; i > 0; i--) {
      int j = rand() % (i + 1);
      int temp = sizes[i];
      sizes[i] = sizes[j];
      sizes[j] = temp;
   }
   void *pointers[arrSize];
   // Malloc the shuffled size array and put into the ptr array
   for (int i = 0; i < arrSize; i++) {
      printf("%d\n", sizes[i]);
      pointers[i] = malloc(sizes[i]);
   }

   // Shuffle the ptr array utilizing fisher yates
   for (int i = arrSize - 1; i > 0; i--) {
      int j = rand() % (i + 1);
      void *temp = pointers[i];
      pointers[i] = pointers[j];
      pointers[j] = temp;
   }

   // Deallocate the shuffled ptr array
   for (int i = 0; i < arrSize; i++) {
      free(pointers[i]);
   }

}*/

void test5() {
   int sizes[] = {1, 10, 40, 100, 200, 500, 32, 4, 36, 199};
   int arrSize = sizeof(sizes) / sizeof(int);
   void *pointers[arrSize];

   for (int i = 0; i < arrSize; i++) {
      pointers[i] = malloc(sizes[i]);
   }

   for (int i = 0; i < arrSize; i++) {
      free(pointers[i]);
   }
}

//Performance Testing
void getAllTests(int iterations){

   int size = 5;
   double times[size];
   for(int i = 0; i < size; i++){
      times[i] = performanceTest((i+1), iterations);
   }

   printf("---------------------------------------------\n");
   printf("Testing times for ");
   printf("%d", iterations);
   printf(" iterations");
   for(int i = 0; i < size; i++){
      printf("\nTest ");
      printf("%d", (i+1));
      printf(" time:\n");
      printf("%lf",times[i]);
      printf(" microseconds");
   }
   printf("\n---------------------------------------------\n");
}

double performanceTest(int testNum, int iterations) {
   struct timeval start, end;
   double timeTaken = 0;
   switch (testNum) {
      case 1:
         for (int i = 0; i < iterations; i++) {
            gettimeofday(&start, NULL);
            test1();
            gettimeofday(&end, NULL);
            timeTaken +=
                (end.tv_sec - start.tv_sec) * 1e6 + end.tv_usec - start.tv_usec;
         }
         printf("TEST 1\n");
         printf("MemClear?: %d\n", memCleared());  // Check if memory is cleared
         break;
      case 2:
         for (int i = 0; i < iterations; i++) {
            gettimeofday(&start, NULL);
            test2();
            gettimeofday(&end, NULL);
            timeTaken +=
                (end.tv_sec - start.tv_sec) * 1e6 + end.tv_usec - start.tv_usec;
         }
         printf("TEST 2\n");
         printf("MemClear?: %d\n", memCleared());  // Check if memory is cleared
         break;
      case 3:
         for (int i = 0; i < iterations; i++) {
            gettimeofday(&start, NULL);
            test3();
            gettimeofday(&end, NULL);
            timeTaken +=
                (end.tv_sec - start.tv_sec) * 1e6 + end.tv_usec - start.tv_usec;
         }
         printf("TEST 3\n");
         printf("MemClear?: %d\n", memCleared());  // Check if memory is cleared
         break;
      case 4:
         for (int i = 0; i < iterations; i++) {
            gettimeofday(&start, NULL);
            test4();
            gettimeofday(&end, NULL);
            timeTaken +=
                (end.tv_sec - start.tv_sec) * 1e6 + end.tv_usec - start.tv_usec;
         }
         printf("TEST 4\n");
         printf("MemClear?: %d\n", memCleared());  // Check if memory is cleared
         break;
      case 5:
         for (int i = 0; i < iterations; i++) {
            gettimeofday(&start, NULL);
            test5();
            gettimeofday(&end, NULL);
            timeTaken +=
                (end.tv_sec - start.tv_sec) * 1e6 + end.tv_usec - start.tv_usec;
         }
         printf("TEST 5\n");
         printf("MemClear?: %d\n", memCleared());  // Check if memory is cleared
         break;
   }

   return (timeTaken / iterations);
}

int main() { getAllTests(50); }