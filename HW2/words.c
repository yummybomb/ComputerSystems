#include<assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <fcntl.h> 
#include <ctype.h>
#include "hashMap.h"

#define FILE_NOT_FOUND_ERROR "File not found"
#define FILE_NOT_OPENED_ERROR "File not opened yet"
#define STARTSIZE 8

bool processFile(const char* fileName);
bool processDirectory(const char* dirName);

bool isValidCharacter(char currentChar, char previousChar, char prevPreviousChar);

long long global_ctr = 1;
map_t* map;

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
    int length = 7;
    char str[length+1];
    generate_strings(str, 0, length);
    printf("%d\n", map_length(map));

    global_ctr = 1;
    char str2[length+1];
    generate_stringsToCheck(str2, 0, length);
    printf("done\n");
    




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

bool processFile(const char* fileName) {
    int fd = open(fileName, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Error on line %d : %s\n", __LINE__, FILE_NOT_FOUND_ERROR);
        return false;
    }

    //Initialize buffer
    char* word = malloc(STARTSIZE);
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
            //TODO: Add Only the first character to hashmap
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
            else if(validResult == 0){
                //New word
                word[wordIndex] = '\0';
                // ADD WORD TO HASHMAPHERE

                word = realloc(word, STARTSIZE);
                wordCapacity = STARTSIZE;
                wordIndex = 0;
            }
            else{
                //For ' special case (new word, but the ' is part of the next word)
                word[wordIndex] = '\0';
                // ADD WORD TO HASHMAPHERE

                word = realloc(word, STARTSIZE);
                wordCapacity = STARTSIZE;
                word[0] = next; //next should be '
                wordIndex = 1;
            }

            //shift characters
            prev = c;
            c = next;


        }
        //Check if the very last character is part of a word, then add word to hashmap
        next = '\0';
        if(isValidCharacter){
            word[wordIndex++] = c;
            word[wordIndex] = '\0';
            //TODO: push to hashmap

        }

    }

    free(word);

    if (close(fd) < 0) { 
        fprintf(stderr, "Error on line %d : %s\n", __LINE__, FILE_NOT_OPENED_ERROR);
        return false;
    }
    return true;
}

bool processDirectory(const char* dirName){
    //TODO LATER
    return false;
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
