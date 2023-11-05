#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <fcntl.h> 
#include <ctype.h>

#define FILE_NOT_FOUND_ERROR "File not found"
#define FILE_NOT_OPENED_ERROR "File not opened yet"
#define STARTSIZE 8

bool processFile(const char* fileName);
bool processDirectory(const char* dirName);


int main(int argc, char* argv[]){
    // Check if any arguments were supplied
    if (argc < 2) {
        printf("No directories or files specified.\n");
        return 1;
    }

    //TESTING FOR NOW
    for (int i = 1; i < argc; i++){
        processFile(argv[i]);
    }
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
    
    char c = '\0', prev_c = '\0', prev_prev_c = '\0';

    //Loop to EOF by character
    while (read(fd, &c, 1) > 0) {
        if (wordIndex >= wordCapacity) {
                wordCapacity *= 2;
                word = realloc(word, wordCapacity);
        }
        if (isValidCharacter(c, prev_c, prev_prev_c)) {            
            word[wordIndex++] = c;
        } else if (wordIndex > 0) {
            word[wordIndex] = '\0';
            // ADD WORD TO HASHMAPHERE

            word = realloc(word, STARTSIZE);
            wordCapacity = STARTSIZE;
            wordIndex = 0;
        }
        prev_prev_c = prev_c;
        prev_c = c;
    }

    free(word);

    if (close(fd) < 0) { 
        fprintf(stderr, "Error on line %d : %s\n", __LINE__, FILE_NOT_OPENED_ERROR);
        return false;
    }
    return true;
}

//not working
//check edge cases: '--' and '\'' by itself...
bool isValidCharacter(char currentChar, char previousChar, char prevPreviousChar) {
    return isalpha(currentChar) || currentChar == '\'' 
        || (currentChar == '-' && isalpha(previousChar) && prevPreviousChar != '-');
}

bool processDirectory(const char* dirName){
    //TODO LATER
    return false;
}