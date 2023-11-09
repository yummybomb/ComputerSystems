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
    




    // Check if any arguments were supplied
    // if (argc < 2) {
    //     printf("No directories or files specified.\n");
    //     return 1;
    // }

    // //TESTING FOR NOW
    // for (int i = 1; i < argc; i++){
    //     processFile(argv[i]);
    // }
    return 0;
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
    
    char firstTwo[2];
    int bytesRead = read(fd, firstTwo, 2);
    if(bytesRead < 0){
        //TODO: Error on read() Message 
    }
    else if(bytesRead == 0){
        //TODO: Empty File Case
    }
    if(bytesRead == 1){
        if(isalpha(firstTwo[0])){
            word[wordIndex++] = firstTwo[0];
            word[wordIndex] = '\0';
            //Add Only the first character to hashmap
            map_inc(map, word);
        }
        else{
            //TODO: Empty File
        }
    }
    else{
        char prev = firstTwo[0]; char c = firstTwo[1]; char next = '\0';
        if(isalpha(prev) || (prev == '\'' && isalpha(c))){
            word[wordIndex] = prev;
            wordIndex++;
        }

        while (read(fd, &next, 1) > 0){
            if(wordIndex >= wordCapacity){
                wordCapacity *= 2;
                word = realloc(word, wordCapacity);
            }

            int validResult = isValidCharacter(prev, c, next);
            if(validResult == 1){
                //continue word
                word[wordIndex++] = c;
            }
            else if(validResult == 0 && wordIndex > 0){
                //New word
                word[wordIndex] = '\0';
                // ADD WORD TO HASHMAPHERE
                map_inc(map, word);
                free(word);
                word = calloc(STARTSIZE, sizeof(char));
                wordCapacity = STARTSIZE;
                wordIndex = 0;
            }
            else if(validResult == 2){
                //For ' special case (new word, but the ' is part of the next word)
                if(wordIndex > 0){
                    word[wordIndex] = '\0';
                    // ADD WORD TO HASHMAPHERE
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
            //push to hashmap
            map_inc(map, word);
        }
    }
    map_inc(map,word);
    free(word);

    if (close(fd) < 0) { 
        fprintf(stderr, "Error on line %d : %s\n", __LINE__, FILE_NOT_OPENED_ERROR);
        return false;
    }
    return true;
}

//0 is false, 1 is true, 2 only happens if the character after a ' is a letter
int isValidCharacter(char prev, char curr, char next){
    if(isalpha(curr)) return 1;
    else if(curr == '\''){
        if(isalpha(prev)) return 1;
        else if(isalpha(next)) return 2;
        else return 0;
    }
    else if(curr == '-'){
        if(isalpha(prev) && isalpha(next)) return 1;
        else return 0;
    }
    else return 0;
}

bool processDirectory(const char* dirName){
    //TODO LATER
    return false;

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
            char* key = map->items[i].key;
            int value = map->items[i].value;
            wordList[curr] = key;
            wordCount[curr] = value;
            curr++;
        }
    }

//     //Bubble sort implementation
// for(int i = 0; i < length-1; i++){
//     for(int j = 0; j < length-i-1; j++){ // Subtract 1 here
//         if(wordCount[j] < wordCount[j+1]){
//             int temp = wordCount[j];
//             wordCount[j] = wordCount[j+1];
//             wordCount[j+1] = temp;

//             char* tempString = wordList[j];
//             wordList[j] = wordList[j+1];
//             wordList[j+1] = tempString;
//         }
//     }
// }

quickSort(wordCount, wordList, 0, length - 1, DESCENDING); // For descending order


for(int i = 0; i < length; i++){
    printf("%s %d\n", wordList[i], wordCount[i]);
}
}