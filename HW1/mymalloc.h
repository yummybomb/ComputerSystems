#ifndef MYMALLOC_H
#define MYMALLOC_H

#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)

#define MEMSIZE 512

#define heap ((char *)memory)

#define ROUNDUP8(x) (((x) + 7) & (-8))

static double memory[MEMSIZE];

bool memCleared();

#endif