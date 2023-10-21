#include <stdbool.h>
#include <stdio.h>
#include "mymalloc.h"

// Define heap, total size, and start ptr
#define MEMSIZE 512
static double memory[MEMSIZE];
#define heap ((char *)memory)
#define headerSize 8

// FORWARD DECLARATIONS
// Malloc functions
int GetChunkSize(void *start);
bool IsFree(void *start);
void SetChunkSize(void *start, int size);
void MarkAsAllocated(void *start);
bool NextChunkIsUninitialized(void *start);
void SetNextChunkSize(void *start, int size);
void *GetNextChunk(void *start);
void MarkNextChunkFree(void *start);

// Free functions
void MarkAsFree(void *start);
void CoalesceNextChunk(void *ptr);
bool NextValidAndFree(char *ptr, char *memEnd);

// Memclear function
bool IsUninitialized(void *start);
bool IsFullyCleared(void *start);

// BEGIN mymalloc
void *mymalloc(size_t _Size, char *file, int line) {
   if (_Size == 0) {
      fprintf(stderr,
              "[ERROR] in file %s at line %d: cannot allocate 0 bytes\n", file,
              line);
      return NULL;
   }

   // Define variables
   int size = ROUNDUP8(_Size);
   char *res = NULL;
   char *memStart = heap;
   char *memEnd = memStart + MEMSIZE * sizeof(double);

   // Begin iterating over heap headers
   while (memStart < memEnd) {
      int chunkSize = GetChunkSize(memStart);
      bool isFree = IsFree(memStart);
      // Init memory (header -> free)
      // Loop through all memories
      //    EC: If size + 8 == chunk.size, then return chunk ptr + chunk.isFree
      //    = False Otherwise, if size + 8 > chunk.size, you cut and set next
      //    free portion to remaining size + currChunk.isFree = false and return
      //    chunk ptr

      // Initialize
      if (isFree == true && chunkSize == 0) {
         SetChunkSize(memStart, size + headerSize);
         MarkAsAllocated(memStart);
         res = memStart + headerSize;
         isFree = true;
         SetNextChunkSize(memStart, memEnd - (memStart + size + headerSize));

         return res;
      }
      // Edge case, where chunk size equals requested size (accounting for
      // header)
      if (isFree == true && chunkSize == size + headerSize) {
         MarkAsAllocated(memStart);
         res = memStart + headerSize;
         return res;
      }

      // Cut
      if (isFree == true && chunkSize > size + headerSize) {
         SetChunkSize(memStart, size + headerSize);
         MarkAsAllocated(memStart);
         res = memStart + headerSize;

         SetNextChunkSize(memStart, chunkSize - (size + headerSize));
         MarkNextChunkFree(memStart);

         return res;
      }

      if (isFree == false || chunkSize < size + headerSize) {
         memStart = GetNextChunk(memStart);
      }
   }
   fprintf(stderr, "[ERROR] in file %s at line %d: not enough memory\n", file,
           line);
   return NULL;
}

// BEGIN myfree
void myfree(void *_Memory, char *file, int line) {
   char *headerStart = (char *)_Memory - headerSize;
   char *memStart = heap;

   char *memEnd = memStart + MEMSIZE * sizeof(double);

   bool ptrValid = true;
   while (memStart < memEnd) {
      if (memStart == headerStart) {
         if (IsFree(headerStart)) {
            fprintf(stderr,
                    "[ERROR] in file %s at line %d: Pointer is already free\n",
                    file, line);
            return;
         } else {
            MarkAsFree(headerStart);
            ptrValid = false;
         }
      }
      memStart = GetNextChunk(memStart);
   }

   if (ptrValid) {
      fprintf(stderr,
              "[ERROR] in file %s at line %d: Pointer does not point to the "
              "start of a chunk \n",
              file, line);
      return;
   }

   memStart = heap;
   while (memStart < memEnd) {
      while (IsFree(memStart) && NextValidAndFree(memStart, memEnd)) {
         CoalesceNextChunk(memStart);
      }
      memStart = GetNextChunk(memStart);
   }
}

// START OF HELPER FUNCTIONS
bool NextValidAndFree(char *ptr, char *memEnd) {
   char *ptr2 = ptr + GetChunkSize(ptr);
   if (ptr2 >= memEnd) {
      return false;
   } else {
      return (IsFree(ptr2));
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

void MarkNextChunkFree(void *start) {
   MarkAsFree((char *)start + GetChunkSize(start));
}

void *GetNextChunk(void *start) { return (char *)start + GetChunkSize(start); }

bool IsUninitialized(void *start) {
   if (GetChunkSize(start) == 0 && IsFree(start) == true) return true;
   return false;
}

bool IsFullyCleared(void *start) {
   if (GetChunkSize(start) == headerSize * MEMSIZE && IsFree(start) == true)
      return true;
   return false;
}