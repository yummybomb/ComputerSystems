#ifndef MYMALLOC_H
#define MYMALLOC_H

#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)

#define ROUNDUP8(x) (((x) + 7) & (-8))

void *mymalloc(size_t _Size, char *file, int line);
void myfree(void *_Memory, char *file, int line);
void *GetHeapStart();
bool memCleared();

#endif