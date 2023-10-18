#include <stdbool.h>
#include <stdio.h>
#include "mymalloc.h"

#define MEMSIZE 512
static double memory[MEMSIZE];
#define heap ((char *)memory)

// Malloc functions
int GetChunkSize(void *start);
bool IsFree(void *start);
void SetChunkSize(void *start, int size);
void MarkAsAllocated(void *start);
bool NextChunkIsUninitialized(void *start);
void SetNextChunkSize(void *start, int size);
void *GetNextChunk(void *start);

// Free functions
void MarkAsFree(void *start);
void CoalesceNextChunk(void *ptr);

bool NextValidAndFree(char *ptr, char *memEnd) {
  char *ptr2 = ptr + GetChunkSize(ptr);
  if (ptr2 >= memEnd) {
    return false;
  } else {
    return (IsFree(ptr2));
  }
}

// Memclear function
bool IsUninitialized(void *start);
bool IsFullyCleared(void *start);

void *mymalloc(size_t _Size, char *file, int line) {
  if (_Size == 0) {
    printf("Error: cannot allocate 0 bytes\n");
    return NULL;
  }

  int size = ROUNDUP8(_Size);

  char *res = NULL;
  char *memStart = heap;

  char *memEnd = memStart + MEMSIZE * sizeof(double);

  while (memStart < memEnd) {
    int chunkSize = GetChunkSize(memStart);
    bool isFree = IsFree(memStart);

    if (chunkSize == 0 && isFree == true) {
      SetChunkSize(memStart, size + 8);
      MarkAsAllocated(memStart);
      res = memStart + 8;
      isFree = true;
      SetNextChunkSize(memStart, memEnd - (memStart + size + 8));

      return res - 8;
    }

    if (isFree == true && chunkSize >= size + 8) {
      SetChunkSize(memStart, size + 8);
      MarkAsAllocated(memStart);
      res = memStart + 8;

      if (NextChunkIsUninitialized(memStart)) {
        SetNextChunkSize(memStart, chunkSize - (size + 8));
      }

      return res - 8;
    }

    if (isFree == false || chunkSize < size + 8) {
      memStart = GetNextChunk(memStart);
    }
  }
  printf("Error: not enough memory\n");
  return NULL;
}

void myfree(void *_Memory, char *file, int line) {
  char *memStart = heap;

  char *memEnd = memStart + MEMSIZE * sizeof(double);

  bool ptrValid = true;
  while (memStart < memEnd) {
    if (memStart == _Memory) {
      if (IsFree(_Memory)) {
        printf("Pointer is already free\n");
        return;
      } else {
        MarkAsFree(_Memory);
        ptrValid = false;
      }
    }
    memStart += GetChunkSize(memStart);
  }

  if (ptrValid) {
    printf("Pointer not in heap\n");
    return;
  }

  memStart = heap;
  while (memStart < memEnd) {

    while (IsFree(memStart) && NextValidAndFree(memStart, memEnd)) {
      CoalesceNextChunk(memStart);
    }
    memStart += GetChunkSize(memStart);
  }
}

void CoalesceNextChunk(void *ptr) {
  int size = GetChunkSize(ptr);
  void *ptr2 = ptr + size;
  int size2 = GetChunkSize(ptr2);
  SetChunkSize(ptr, size + size2);
}

bool memCleared() {
  void *start = heap;
  if (IsUninitialized(start) || IsFullyCleared(start)) {
    return true;
  }
  return false;
}

void *GetHeapStart() { return heap; }

int GetChunkSize(void *start) { return *(int *)start; }

bool IsFree(void *start) { return *((char *)start + 4) == 0; }

void SetChunkSize(void *start, int size) { *(int *)start = size; }

void MarkAsAllocated(void *start) { *((char *)start + 4) = 1; }

void MarkAsFree(void *start) { *((char *)start + 4) = 0; }

bool NextChunkIsUninitialized(void *start) {
  return GetChunkSize((char *)start + GetChunkSize(start)) == 0;
}

void SetNextChunkSize(void *start, int size) {
  SetChunkSize((char *)start + GetChunkSize(start), size);
}

void *GetNextChunk(void *start) { return (char *)start + GetChunkSize(start); }

bool IsUninitialized(void *start) {
  if (GetChunkSize(start) == 0 && IsFree(start) == true)
    return true;
  return false;
}

bool IsFullyCleared(void *start) {
  if (GetChunkSize(start) == 8 * MEMSIZE && IsFree(start) == true)
    return true;
  return false;
}