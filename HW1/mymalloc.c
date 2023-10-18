#include "mymalloc.h"
#include <stdbool.h>
#include <stdio.h>

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
      res = memStart;
      isFree = true;
      SetNextChunkSize(memStart, memEnd - (memStart + size + 8));

      return res;
    }

    if (isFree == true && chunkSize >= size + 8) {
      SetChunkSize(memStart, size + 8);
      MarkAsAllocated(memStart);
      res = memStart;

      if (NextChunkIsUninitialized(memStart)) {
        SetNextChunkSize(memStart, chunkSize - (size + 8));
      }

      return res;
    }

    if (isFree == false || chunkSize < size + 8) {
      memStart = GetNextChunk(memStart);
    }
  }
  printf("Error: not enough memory\n");
  return NULL;
}

void *myfree(void *_Memory, char *file, int line) {
  char *memStart = heap;

  char *memEnd = memStart + MEMSIZE * sizeof(double);

  bool ptrValid = true;
  while (memStart < memEnd) {
    if (memStart == _Memory) {
      if (IsFree(_Memory)) {
        printf("Pointer is already Free\n");
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
    char *ptr2 = memStart + GetChunkSize(memStart);
    if (ptr2 < memEnd) {
      CoalesceNextChunk(memStart);
    }
    memStart += GetChunkSize(memStart);
  }
}

bool memCleared() {
  char *start = heap;

  if (IsUninitialized(start) || IsFullyCleared(start)) {
    return true;
  }

  return false;
}

void CoalesceNextChunk(void *ptr) {

  // First get the size and if the ptr is free
  int size = GetChunkSize(ptr);
  bool freeBool = IsFree(ptr);

  // If the pointer is free, we can check if the second chunk is free.
  if (freeBool) {
    void *ptr2 = ptr + size;

    // Since the second chunk is free, we can coalesce. We do this by adding the
    // chunk size to size of ptr2, +8 because the header of ptr2 is 8 bytes.
    if (IsFree(ptr2)) {
      int size2 = GetChunkSize(ptr2);
      SetChunkSize(ptr, size + size2 + 8);
    }
  }
}

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
}

bool IsFullyCleared(void *start) {
  if (GetChunkSize(start) == 8 * MEMSIZE && IsFree(start) == true)
    return true;
}