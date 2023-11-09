#include "quickSort.h"
#include <stdbool.h>
#include <string.h>

void swap_int(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

void swap_str(char **a, char **b) {
  char *t = *a;
  *a = *b;
  *b = t;
}

int partition(int arr[], char *wordList[], int low, int high,
              bool isAscending) {
  int pivot = arr[high];
  int i = (low - 1);

  for (int j = low; j <= high - 1; j++) {
    if ((isAscending && arr[j] < pivot) || (!isAscending && arr[j] > pivot)) {
      i++;
      swap_int(&arr[i], &arr[j]);
      swap_str(&wordList[i], &wordList[j]);
    } else if (arr[j] == pivot) {
      if (strcmp(wordList[j], wordList[high]) < 0) {
        i++;
        swap_int(&arr[i], &arr[j]);
        swap_str(&wordList[i], &wordList[j]);
      }
    }
  }
  swap_int(&arr[i + 1], &arr[high]);
  swap_str(&wordList[i + 1], &wordList[high]);
  return (i + 1);
}

void quickSort(int arr[], char *wordList[], int low, int high,
               int isAscending) {
  if (low < high) {
    int pi = partition(arr, wordList, low, high, isAscending);

    quickSort(arr, wordList, low, pi - 1, isAscending);
    quickSort(arr, wordList, pi + 1, high, isAscending);
  }
}
