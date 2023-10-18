#ifndef MYMALLOC_H
#define MYMALLOC_H

#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)

#define MEMSIZE 512
static double memory[MEMSIZE];
#define heap ((char *)memory)
#define ROUNDUP8(x) (((x) + 7) & (-8))

void *mymalloc(size_t _Size, char *file, int line);
void myfree(void *_Memory, char *file, int line);

bool memCleared();

#endif