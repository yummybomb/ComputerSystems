#include <stdbool.h>
#include <stdio.h>

#define MEMSIZE 512
#define heap ((char *)memory)

#define ROUNDUP8(x) (((x) + 7) & (-8))

static double memory[MEMSIZE];

int GetChunkSize(void *start);
bool IsFree(void *start);
void SetChunkSize(void *start, int size);
void MarkAsAllocated(void *start);
bool NextChunkIsUninitialized(void *start);
void SetNextChunkSize(void *start, int size);
void *GetNextChunk(void *start);

void *mymalloc(size_t _Size) {

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

      return res;
    }

    if (isFree == true && chunkSize >= size + 8) {
      SetChunkSize(memStart, size + 8);
      MarkAsAllocated(memStart);
      res = memStart + 8;
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

void *myfree(void *_Memory) {}

int GetChunkSize(void *start) { return *(int *)start; }

bool IsFree(void *start) { return *((char *)start + 4) == 0; }

void SetChunkSize(void *start, int size) { *(int *)start = size; }

void MarkAsAllocated(void *start) { *((char *)start + 4) = 1; }

bool NextChunkIsUninitialized(void *start) {
  return GetChunkSize((char *)start + GetChunkSize(start)) == 0;
}

void SetNextChunkSize(void *start, int size) {
  SetChunkSize((char *)start + GetChunkSize(start), size);
}

void *GetNextChunk(void *start) { return (char *)start + GetChunkSize(start); }

int main() { printf("end\n"); }