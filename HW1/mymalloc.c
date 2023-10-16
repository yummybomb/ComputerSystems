#include "mymalloc.h"
#include <stdio.h>

int main() {
  header_t curr = { isUsed: 0, size: MEMSIZE-sizeof(header_t), next : NULL };
}

void *mymalloc(size_t _Size) {}

void *myfree(void *_Memory) {}
