#include "hashMap.h"
#include "quickSort.h"
#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define FILE 1
#define DIRECTORY 2
#define STARTSIZE 8
map_t *map;

int checkFile(const char *path);
bool endsWithTxt(const char *filename);
int isValidCharacter(char prev, char curr, char next);
bool processFile(const char *fileName, map_t *map);
bool processDirectory(const char *dirName, map_t *map);
void printWords(map_t *map);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("No directories or files specified.\n");
        return 1;
    }

    map = init_map();
    for (int i = 1; i < argc; i++) {
        if (checkFile(argv[i]) == FILE) {
            processFile(argv[i], map);
            continue;
        }
        processDirectory(argv[i], map);
    }

    printWords(map);
    map_destroy(map);
    return 0;
}

bool processDirectory(const char *dirName, map_t *map) {
    struct dirent *de;
    DIR *dir = opendir(dirName);
    if (dir == NULL) {
        fprintf(stderr, "Error on line %d : %s\n", __LINE__, strerror(errno));
    }

    while ((de = readdir(dir)) != NULL) {
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
            continue;
        char path[263];
        snprintf(path, sizeof(path), "%s/%s", dirName, de->d_name);

        if (checkFile(path) == FILE) {
            processFile(path, map);
            continue;
        }
        processDirectory(path, map);
    }

    if (closedir(dir) < 0) {
        fprintf(stderr, "Error on line %d : %s\n", __LINE__, strerror(errno));
        return false;
    }
    return true;
}

bool processFile(const char *fileName, map_t *map) {
    int fd = open(fileName, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Error on line %d : %s\n", __LINE__, strerror(errno));
        return false;
    }

    // Initialize buffer
    char *word = calloc(STARTSIZE, sizeof(char));
    int wordCapacity = STARTSIZE;
    int wordIndex = 0;

    char prev = '\0';
    char c = '\0';
    char next = '\0';
    while (read(fd, &next, 1) > 0) {
        if (wordIndex >= wordCapacity) {
            wordCapacity *= 2;
            word = realloc(word, wordCapacity);
        }
        int validResult = isValidCharacter(prev, c, next);

        if (validResult == 1) {
            // continue word
            word[wordIndex++] = c;
        } else if (validResult == 0 && wordIndex > 0) {
            // New word
            word[wordIndex] = '\0';
            map_inc(map, word);
            free(word);
            word = calloc(STARTSIZE, sizeof(char));
            wordCapacity = STARTSIZE;
            wordIndex = 0;
        } else if (validResult == 2) {
            // For ' special case (new word, but the ' is part of the next word)
            if (wordIndex > 0) {
                word[wordIndex] = '\0';
                map_inc(map, word);
                free(word);

                word = calloc(STARTSIZE, 1);
                wordCapacity = STARTSIZE;
            }
            word[0] = '\'';
            wordIndex = 1;
        }
        // shift characters
        prev = c;
        c = next;
    }
    // Check if the very last character is part of a word, then add word to
    // hashmap
    next = '\0';
    if (isValidCharacter(prev, c, next)) {
        word[wordIndex++] = c;
        word[wordIndex] = '\0';
        map_inc(map, word);
    } else {
        map_inc(map, word);
    }
    free(word);

    if (close(fd) < 0) {
        fprintf(stderr, "Error on line %d : %s\n", __LINE__, strerror(errno));
        return false;
    }
    return true;
}

void printWords(map_t *map) {
    int cap = map->capacity;
    int length = map_length(map);

    char *wordList[length];
    int wordCount[length];
    int curr = 0;
    // Copy all words in the hash map to these arrays to be sorted
    for (int i = 0; i < cap; i++) {
        if (curr >= length) break;
        if (map->items[i].isUsed == true) {
            wordList[curr] = map->items[i].key;
            wordCount[curr] = map->items[i].value;
            curr++;
        }
    }

    quickSort(wordCount, wordList, 0, length - 1, DESCENDING);

    for (int i = 0; i < length; i++) {
        printf("%s %d\n", wordList[i], wordCount[i]);
    }
}

// 0 is false, 1 is true, 2 only happens if the character after a ' is a letter
int isValidCharacter(char prev, char curr, char next) {
    if (isalpha(curr) || (curr == '\'' && isalpha(prev)) ||
        (curr == '-' && isalpha(prev) && isalpha(next))) {
        return 1;
    } else if (curr == '\'' && isalpha(next)) {
        return 2;
    }
    return 0;
}

// return 1 if path, 2 if directory
int checkFile(const char *path) {
    struct stat path_stat;
    if (stat(path, &path_stat) != 0) {
        printf("Could not stat %s: %s\n", path, strerror(errno));
        return -1;
    }

    if (S_ISREG(path_stat.st_mode) && endsWithTxt(path))
        return FILE;
    else if (S_ISDIR(path_stat.st_mode))
        return DIRECTORY;
    return -2;
}

bool endsWithTxt(const char *filename) {
    size_t len = strlen(filename);
    return len > 4 && strcmp(filename + len - 4, ".txt") == 0;
}

// ll ctr = 0;
// void generate_strings(char *str, int index, int length) {
//     if (index == length) {
//         str[index] = '\0';
//         map_set(map, str, ctr);
//         assert(map_get(map, str) == ctr);
//         ctr++;
//         return;
//     }

//     for (char c = 'a'; c <= 'z'; c++) {
//         str[index] = c;
//         generate_strings(str, index + 1, length);
//     }
// }

// void generate_stringsToCheck(char *str, int index, int length) {
//     if (index == length) {
//         str[index] = '\0';
//         assert(map_get(map, str) == ctr);
//         ctr++;
//         return;
//     }

//     for (char c = 'a'; c <= 'z'; c++) {
//         str[index] = c;
//         generate_strings(str, index + 1, length);
//     }
// }