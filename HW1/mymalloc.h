#ifndef MYMALLOC_H
#define MYMALLOC_H

#define MEMSIZE 512
#define heap ((char *)memory)

static double memory[MEMSIZE];

typedef struct header {
  bool isUsed;
  int size;
  struct header *next;
} header_t;

#endif