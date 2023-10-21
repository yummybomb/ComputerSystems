#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "mymalloc.h"

#define NUM_OBJECTS 7
#define SIZE 512

int main() {
    char *objects[NUM_OBJECTS];
    
    // Allocate and fill objects
    for (int i = 0; i < NUM_OBJECTS; i++) {
        objects[i] = (char *) malloc(SIZE);
        if (objects[i] == NULL) {
            printf("malloc failed\n");
            return -1;
        }
        memset(objects[i], i + 1, SIZE);  // Fill with distinct byte pattern
    }

    // Check each object still contains the written pattern
    for (int i = 0; i < NUM_OBJECTS; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (objects[i][j] != i + 1) {
                printf("Memory check failed\n");
                return -1;
            }
        }
    }

    // Free memory
    for (int i = 0; i < NUM_OBJECTS; i++) {
        free(objects[i]);
    }

    // Check if heap is empty
    if (memCleared() == true){
        printf("All tests passed successfully\n");
    }else{
        printf("Heap not freed fully!\n");
    }
    return 0;
}
