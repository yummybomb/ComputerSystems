#ifndef MYMALLOC_H
#define MYMALLOC_H

#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)

#endif