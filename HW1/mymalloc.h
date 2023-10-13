#ifndef MYMALLOC_H
#define MYMALLOC_H

#define heap ((char *)memory)
#define MEMLENGTH 512

static double memory[MEMLENGTH];

typedef struct header{
    int size;
    struct header *next;
} header_t;  

#endif