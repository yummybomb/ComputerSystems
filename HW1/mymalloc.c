#include <stdio.h>

#define MEMSIZE 512
#define heap ((char *)memory)

#define ROUNDUP8(x) (((x) + 7) & (-8))

static double memory[MEMSIZE];


void *mymalloc(size_t _Size) {
  if (_Size = 0){
    return NULL;
  }

  int size = ROUNDUP8(size);

  char* res = NULL;
  char* memStart = heap;
  char* memEnd = memStart + MEMSIZE * sizeof(double);
  

}

void *myfree(void *_Memory) {}

int main() {
  
  
}