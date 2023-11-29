#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define INITIAL_BUFFER_SIZE 256
// Function prototypes
int batch_mode(char* fileName);
void interactive_mode(void);
// Helper Funcs
void read_file(FILE* file);
char *read_line(void);
bool is_empty_or_whitespace(const char* str);
void process_line(char* line);
void handle_error(const char* msg);
//Built-in commands
void cd(const char* path);
int pwd(void);
void which(const char* progName);
void echo(const char* line);
void exit_mysh();


int main(int argc, char* argv[]) {
    if (argc > 2) {
        fprintf(stderr, "Error: mysh only takes 0 or 1 arguments\n");
        return 1;
    }
    if (argc == 2) {  // Batch mode - 1 argument
        batch_mode(argv[1]);
    }
    if (argc == 1) {  // Interactive mode - 0 args
        interactive_mode();
    }
    return 0;
}

int batch_mode(char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        handle_error("Error opening file");
    }

    read_file(file);

    if (fclose(file) != 0) {
        handle_error("Error closing file");
    }

    return 0;
}

void interactive_mode(void) { 
    //Introduction Message
    printf("Welcome to myShell!\n");
    char *line;
    while(1 == 1){
        printf("mysh> ");
        line = read_line();

        if(strcmp(line, "exit\n") != 0){
            process_line(line);
            free(line);
        }
        else{
            free(line);
            exit_mysh();
        }
    }
    return; }

// Helper Funcs
char *read_line(){
     // Allocate initial buffer
    char* line = malloc(INITIAL_BUFFER_SIZE);
    if (line == NULL) {
        handle_error("Error allocating memory");
    }

    size_t bufferSize = INITIAL_BUFFER_SIZE;
    size_t contentSize = 0;

    // Read character
    char c;

    while(c != '\n'){
        if (contentSize >= bufferSize) {
            bufferSize *= 2;
            line = realloc(line, bufferSize);
            if (line == NULL) {
                handle_error("Error reallocating memory");
            }
        }
        c = getc(stdin);
        line[contentSize++] = (char)c;

        if (c == '\n') {
            line[contentSize] = '\0';
            contentSize = 0;
        }
    }

    if (contentSize > 0) {
        line[contentSize] = '\0';
    }

    return line;

}


void read_file(FILE* file) {
    // Allocate initial buffer
    char* line = malloc(INITIAL_BUFFER_SIZE);
    if (line == NULL) {
        handle_error("Error allocating memory");
    }

    size_t bufferSize = INITIAL_BUFFER_SIZE;
    size_t contentSize = 0;

    // Read character
    int c;
    while ((c = getc(file)) != EOF) {
        if (contentSize >= bufferSize) {
            bufferSize *= 2;
            line = realloc(line, bufferSize);
            if (line == NULL) {
                handle_error("Error reallocating memory");
            }
        }

        // add a character to buffer
        line[contentSize++] = (char)c;

        // EOL - process and reset buffer
        if (c == '\n') {
            line[contentSize] = '\0';
            process_line(line);
            contentSize = 0;
        }
    }

    // Process leftover content in a line
    if (contentSize > 0) {
        line[contentSize] = '\0';
        process_line(line);
    }

    // Clean up
    free(line);
}

void process_line(char* line) {

    if (is_empty_or_whitespace(line)) {
        return;
    }

    //First check if there exists a /, in which case it is a path
    if(line[0] == '/'){
        //TODO FILE SEARCHING

        return;
    }

    //Otherwise, check if the first argument is a built-in command

    //Commands that don't require arguments
    if(strcmp(line, "pwd\n") == 0){
        pwd();
        return;
    }

    //commands that require arguments

    //get the first argument (which should be a command or file name)
    char* argument = strtok(line, " ");

    if(strcmp(argument, "cd") == 0) {cd(argument); return;}
    else if(strcmp(argument, "cd\n") == 0){ printf("cd requires one argument\n"); return;}
    //Then check if the first argument is a file name
    

    //Lastly, print an error message if it is none of the above
    printf("Not a valid command\n");

    return;

}

void handle_error(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

bool is_empty_or_whitespace(const char* str) {
    while (*str) {
        if (!isspace((unsigned char)*str))
            return false;  // Found a non-whitespace character
        str++;
    }
    return true;  // String is empty or contains only whitespace
}



//Built-in Commands

//Change working directory
//path argument is a path to a directory
//Prints an error and fails if it is given the wrong number of args, or if chdir() fails
void cd(const char* path){
    path = strtok(NULL, " ");
    char* cdArg = strdup(path);
    path = strtok(NULL, " ");
    if(path != NULL){
        printf("cd can only accept one argument\n");
        free(cdArg);
        return;
    }

    int status = chdir(cdArg);
    if(status != 0){
        printf("No such file or directory\n");
    }

    free(cdArg);

}


//Prints the current working directory to StdOut
//Returns 0 on error and 1 on success
int pwd(){
    char cwd[PATH_MAX];

    if(getcwd(cwd, sizeof(cwd)) != NULL){
        printf("%s\n", cwd);
    }
    else{
        perror("Could not get current working directory");
        return 0;
    }
    return 1;

}

//Takes signle argument: name of a program
//Prints the path that mysh would use if asked to start that program
//^The result of the search used for bare names
//Prints nothing and fails if given wrong number of args, name of a built-in, or program not found
void which(const char* progName){}

void exit_mysh(){
    printf("mysh: exiting");
    exit(1);
}

void echo(const char* line){
    printf("%s", line);
}