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
    
    char firstThree[3];
    int bytesRead = read(fd, firstThree, 3);

    if(bytesRead == 1){
        //TODO: Add Only the first character to hashmap
    }
    else if(bytesRead == 2){
        //TODO: Check both characters. This can be done manually

    }
    else{
        char prev = firstThree[0]; char c = firstThree[1]; char next = firstThree[2];
        while (read(fd, &c, 1) > 0){
            if(wordIndex >= wordCapacity){
                wordCapacity *= 2;
                word = realloc(word, wordCapacity);
            }


        }
    }

    bool isValidCharacter(char prev, char curr, char next){
        
    }

    /*
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
    }*/

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