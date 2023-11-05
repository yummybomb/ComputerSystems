#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include <fcntl.h> 
#include <ctype.h>

bool processFile(char* fileName);
bool processDirectory(char* fileName);


int main(int argc, char* argv[]){
    // Check if any arguments were supplied
    if (argc < 2) {
        printf("No directories or files specified.\n");
        return 1;
    }

    //test first file for now
    printf("%d\n", processFile(argv[1]));
    
    for (int i = 1; i < argc; i++){
        break;
    }
    return 0;
}

bool processFile(char* fileName) {
    int fd = open(fileName, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return false;
    }

    //max word limit, looking to see how to change this
    char word[4096];
    int wordIndex = 0;
    char curr, prev;

    //Loop to EOF by character
    while (read(fd, &curr, 1) > 0) {
        //logic to find words
        prev = curr;
    }

    if (close(fd) < 0) { 
        perror("close"); 
        return false;
    }
    return true;
}

bool processDirectory(char* directory){
    //TODO LATER
    return false;
}