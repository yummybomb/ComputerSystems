#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define INITIAL_BUFFER_SIZE 256
// Function prototypes
int batch_mode(char* fileName);
void interactive_mode(void);
// Helper Funcs
void read_file(FILE* file);
char *read_line(void);
bool is_empty_or_whitespace(const char* str);
void process_line(char* line);
void get_arguments(char* line , char** arguments, int argc);
int count_arguments(char* line);
void handle_error(const char* msg);
//Built-in commands
void cd(const char* path);
int pwd(void);
char* which(const char* progName);
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

    //While loop will run until exit command is given
    while(1 == 1){
        printf("mysh> ");
        line = read_line();
        process_line(line);
        free(line);
    }
    return; 
}

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
    char c = ' ';

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
    int c = " ";
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

    //These 3 lines are used to get a string array 'arguments,' which stores all arguments of 'line'
    int argc = count_arguments(line);
    char *arguments[argc];
    get_arguments(line, arguments, argc);

    //Whitespace or empty case
    if (argc == 0) return;

    //File searching
    if(arguments[0][0] == '/'){
        //TODO FILE SEARCHING
        return;
    }

    //pwd (only if pwd is the only argument)
    if(strcmp(arguments[0], "pwd") == 0 && argc == 1){
        pwd();
        return;
    }

    //cd (should only take one argument, other if more)
    if(strcmp(arguments[0], "cd") == 0){
        if(argc == 2) cd(arguments[1]);
        else if(argc == 1) printf("cd requires a path\n");
        else printf("incorrect number of arguments\n");
        return;
    }
    // which
    if(strcmp(arguments[0], "which") == 0){
        if(argc == 1) {
            printf("which requires a program name \n"); 
            return;
        }
        if(argc > 2) {
            printf("incorrect number of arguments\n"); 
            return;
        }
        char *path = which(arguments[1]);
        if (path == NULL) {
            printf("Program %s not found\n", arguments[1]);
            return;
        }
        printf("%s\n", path);
        free(path); // free strdup path
        return;
    }

    //TODO: MORE COMMANDS / OPTIONS


    //exit command
    if(strcmp(arguments[0], "exit") == 0 && argc == 1){
        exit_mysh(line);
    }
    
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

//get arguments takes in the string user input, a char** to store the arguments in, and argc (how many arguments)
//there are no returns: the array arguments is modified to store all the arguments present in 'line'
void get_arguments(char* line , char** arguments, int argc){
    int index = 0;
    char* token = strtok(line, " ");
    while(token != NULL){
        if(is_empty_or_whitespace(token) == false){ //only non-empty arguments are stored in the array
            arguments[index++] = token;
            int length = strlen(token);
            if(arguments[index - 1][length - 1] == '\n') arguments[index-1][length-1] = '\0';
        }
        token = strtok(NULL,  " ");
    }
}

//This function counts how many arguments is in a string line (excludes whitespace)
int count_arguments(char* original_line){
    int argc = 0;
    char* line = strdup(original_line); //a duplicate line must be made since strtok modifies strings
    char* countArgs = strtok(line, " ");
    while(countArgs != NULL){
        if(is_empty_or_whitespace(countArgs) == false){ //argc only increases if the token is not empty
            argc++;
        }
        countArgs = strtok(NULL, " ");
    }

    free(line); //freeing the duplicate string
    return argc;
}

//Split arguments for special characters <, >, and |
void split_special_characters(char** result, char** source, int argc){
    for(int i = 0; i < argc; i++){
        char* arg = source[i];
        char* temp;
        
    }
}

//Built-in Commands

//Change working directory
//path argument is a path to a directory
//Prints an error and fails if it is given the wrong number of args, or if chdir() fails
void cd(const char* path){
    int status = chdir(path);
    if(status != 0){
        perror("could not change directory to specified path");
    }
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
char* which(const char *progName) {
   const char *dirs[] = {"/usr/local/bin", "/usr/bin", "/bin"};
   char path[1024];

   for (int i = 0; i < sizeof(dirs) / sizeof(dirs[0]); i++) {
       snprintf(path, sizeof(path), "%s/%s", dirs[i], progName);
       if (access(path, F_OK) != -1) {
           return strdup(path); // Return a copy of the path
       }
   }
   return NULL; // Return NULL if the program is not found
}


//This currently only works for interactive-mode
void exit_mysh(char* line){
    printf("mysh: exiting");
    free(line);
    exit(1);
}