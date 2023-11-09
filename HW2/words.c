#include<assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <fcntl.h> 
#include <ctype.h>
#include "quickSort.h"
#include "hashMap.h"

#define FILE_NOT_FOUND_ERROR "File not found"
#define FILE_NOT_OPENED_ERROR "File not opened yet"
#define STARTSIZE 8

long long global_ctr = 1;
map_t* map;

bool processFile(const char* fileName, map_t *map);
bool processDirectory(const char* dirName);
int isValidCharacter(char prev, char curr, char next);
void printWords(map_t *map);

void generate_strings(char *str, int index, int length) {
    if (index == length) {
        str[index] = '\0'; 
        map_set(map, str, global_ctr);
        assert(map_get(map, str) == global_ctr);
        global_ctr++;
        return;
    }

    for (char c = 'a'; c <= 'z'; c++) {
        str[index] = c;
        generate_strings(str, index + 1, length);
    }
}

void generate_stringsToCheck(char *str, int index, int length) {
    if (index == length) {
        str[index] = '\0'; 
        assert(map_get(map, str) == global_ctr);
        global_ctr++;
        return;
    }

    for (char c = 'a'; c <= 'z'; c++) {
        str[index] = c;
        generate_strings(str, index + 1, length);
    }
}

int main(int argc, char* argv[]){
    // Check if any arguments were supplied
    if (argc < 2) {
        printf("No directories or files specified.\n");
        return 1;
    }
    map = init_map();
    for(int i = 1; i < argc; i++){
        processFile(argv[i], map);
    }

    printWords(map);
    // int length = 6;
    // char str[length+1];
    // generate_strings(str, 0, length);
    // printf("%d\n", map_length(map));

    // global_ctr = 1;
    // char str2[length+1];
    // generate_stringsToCheck(str2, 0, length);
    // printf("done\n");
    
    // //TESTING FOR NOW
    // for (int i = 1; i < argc; i++){
    //     processFile(argv[i]);
    // }
    map_destroy(map);
    return 0;
}

bool processDirectory(const char* dirName){
    //TODO LATER
    return true;
}

bool processFile(const char* fileName, map_t *map) {
    int fd = open(fileName, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Error on line %d : %s\n", __LINE__, FILE_NOT_FOUND_ERROR);
        return false;
    }

    //Initialize buffer
    char* word = calloc(STARTSIZE, sizeof(char));
    int wordCapacity = STARTSIZE;
    int wordIndex = 0;
    
    char prev = '\0'; char c = '\0'; char next = '\0';
    while (read(fd, &next, 1) > 0){
        if(wordIndex >= wordCapacity){
            wordCapacity *= 2;
            word = realloc(word, wordCapacity);
        }
        int validResult = isValidCharacter(prev, c, next);
        
        if(validResult == 1){
            //continue word
            word[wordIndex++] = c;
        }else if(validResult == 0 && wordIndex > 0){
            //New word
            word[wordIndex] = '\0';
            map_inc(map, word);
            free(word);
            word = calloc(STARTSIZE, sizeof(char));
            wordCapacity = STARTSIZE;
            wordIndex = 0;
        }else if(validResult == 2){
            //For ' special case (new word, but the ' is part of the next word)
            if(wordIndex > 0){
                word[wordIndex] = '\0';
                map_inc(map, word);
                free(word); 
                
                word = calloc(STARTSIZE, 1);
                wordCapacity = STARTSIZE;
            }
            word[0] = '\'';
            wordIndex = 1;
        }
        //shift characters
        prev = c;
        c = next;
    }
    //Check if the very last character is part of a word, then add word to hashmap
    next = '\0';
    if(isValidCharacter(prev, c, next)){
        word[wordIndex++] = c;
        word[wordIndex] = '\0';
        map_inc(map, word);
    }else{
        map_inc(map, word);
    }
    free(word);

    if (close(fd) < 0) { 
        fprintf(stderr, "Error on line %d : %s\n", __LINE__, FILE_NOT_OPENED_ERROR);
        return false;
    }
    return true;
}


//0 is false, 1 is true, 2 only happens if the character after a ' is a letter
int isValidCharacter(char prev, char curr, char next){
    if(isalpha(curr) || (curr == '\'' && isalpha(prev)) || (curr == '-' && isalpha(prev) && isalpha(next))) {
        return 1;
    }
    else if(curr == '\'' && isalpha(next)) {    
        return 2;
    }
    return 0;
}

void printWords(map_t *map){
    int cap = map->capacity;
    int length = map_length(map);

    char* wordList[length];
    int wordCount[length];
    int curr = 0;
    //Copy all words in the hash map to these arrays to be sorted
    for(int i = 0; i < cap; i++){
        if (curr >= length) break;
        if(map->items[i].isUsed == true){
            wordList[curr] = map->items[i].key;
            wordCount[curr] = map->items[i].value;
            curr++;
        }
    }

    quickSort(wordCount, wordList, 0, length - 1, DESCENDING);

    for(int i = 0; i < length; i++){
        printf("%s %d\n", wordList[i], wordCount[i]);
    }
}