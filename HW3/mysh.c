#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//structs
typedef struct Command {
    int argc;
    char* command;
    char* arguments[4096];
    char* inputFile;
    char* outputFile;
    bool isBackground;
} Command;


//global variables
//exit status (1 is fail, 0 is success) of last command.
int lastStatus = 0;
Command commands[100];
int comC;

#define INITIAL_BUFFER_SIZE 256
// Function prototypes
int batch_mode(char* fileName);
void interactive_mode(void);
// Helper Funcs
void read_file(FILE* file);
char *read_line(void);
bool is_empty_or_whitespace(const char* str);
int process_line(char* line, int lastStatus);
int then_else_status(char** tokens, int tokc);
void get_tokens(char* line , char** tokens, int tokc);
int count_tokens(char* line);
void handle_error(const char* msg);
int set_commands(char** tokens, int tokc);
//Built-in commands
int cd(const char* path);
int pwd(void);
char* which(const char* progName);
void exit_mysh(char* line);
void echo(char** arguments, int argc);

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
        int status = process_line(line, lastStatus);
        if(status != 2) lastStatus = status;
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
        if (contentSize+1 >= bufferSize) {
            bufferSize *= 2;
            line = realloc(line, bufferSize);
            if (line == NULL) {
                handle_error("Error reallocating memory");
            }
        }
        c = getc(stdin);
        if(c == '|') comC++;
        if(c == '|' || c == '<' || c == '>'){line[contentSize++] = ' '; line[contentSize++] = (char)c; line[contentSize++] = ' ';}
        else line[contentSize++] = (char)c;

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
            process_line(line, lastStatus);
            contentSize = 0;
        }
    }

    // Process leftover content in a line
    if (contentSize > 0) {
        line[contentSize] = '\0';
        process_line(line, lastStatus);
    }

    // Clean up
    free(line);
}

//0 is successful, 1 is failure, 2 if empty
int process_line(char* line, int lastStatus) {

    //These 3 lines are used to get a string array 'tokens,' which stores all tokens of 'line'
    int tokc = count_tokens(line);
    char *tokens[tokc+1];
    get_tokens(line, tokens, tokc);
    //Whitespace or empty case
    if (tokc == 0) return 2;

    int status = then_else_status(tokens, tokc);
    if(status == 1) return 1;

    if(strcmp(tokens[0], "|") == 0){fprintf(stderr, "Cannot have pipe in the beginning\n"); return 1;}
    if(strcmp(tokens[tokc-1], "|") == 0){fprintf(stderr, "Cannot have pipe at end\n"); return 1;}
    if(strcmp(tokens[tokc-1], "<") == 0 || strcmp(tokens[tokc-1], ">") == 0){fprintf(stderr, "cannot have redirect at end\n"); return 1;}

    for(int i = 1; i < tokc; i++){
        if ((strcmp(tokens[i], tokens[i-1]) == 0) && (strcmp(tokens[i], "<") == 0 || strcmp(tokens[i],">") == 0 || strcmp(tokens[i], "|") == 0)) {
            fprintf(stderr, "Cannot have a double redirect/pipe");
            return 1;
            }
    }

    int total_commands = set_commands(tokens, tokc);

    for (int i = 0; i < total_commands; i++){
        //pwd (only if pwd is the only argument)
        if(strcmp(commands[i].arguments[0], "pwd") == 0){
            if(commands[i].argc != 1){
                fprintf(stderr, "Error: pwd should be the only argument\n");
                return 1;
            }
            return pwd();
        }
         //cd (should only take one argument, other if more)
        if(strcmp(commands[i].arguments[0], "cd") == 0){
            if(commands[i].argc != 2){
                fprintf(stderr, "Error: cd incorrect number of arguments\n");
                return 1;
            }
            return cd(commands[i].arguments[1]);
        }
        if(strcmp(commands[i].arguments[0], "which") == 0){
            if(commands[i].argc == 1) {
                fprintf(stderr, "Error: which requires a program name \n"); 
                return 1;
            }
            if(commands[i].argc > 2) {
                fprintf(stderr, "Error: which incorrect number of arguments\n"); 
                return 1;
            }
            char *path = which(commands[i].arguments[1]);

            if (path == NULL) {
                fprintf(stderr, "Program %s not found\n", commands[i].arguments[1]);
                return 1;
            }
            printf("%s\n", path);
            free(path); // free strdup path
            return 0;
        }

        if(strcmp(commands[i].arguments[0], "echo") == 0){
            if (commands[i].argc == 1) {
                fprintf(stderr, "echo requires a program name \n"); 
                return 1;
            }
            echo(commands[i].arguments, commands[i].argc);
            return 0;
        }
         //TODO: MORE COMMANDS / OPTIONS


        //exit command
        if(strcmp(commands[i].arguments[0], "exit") == 0 && commands[i].argc == 1){
            exit_mysh(line);
        }
        fprintf(stderr, "Not a valid command\n");

        //remove return later
        return 1;
    }
    return 0;
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

//0 is success, 1 is failure
int then_else_status(char** tokens, int tokc){
    if(tokc == 0) return 0;
    if(strcmp(tokens[0], "then") == 0) {
        if(tokc == 1){
        fprintf(stderr, "cannot have then or else by itself\n");
        return 1;
        }
        if(lastStatus == 1) {fprintf(stderr, "then statement failed\n");return 1;}
    }
    if(strcmp(tokens[0], "else") == 0) {
        if(tokc == 1){
        fprintf(stderr, "cannot have then or else by itself\n");
        return 1;
        }
        if(lastStatus == 0){fprintf(stderr, "else statement failed\n");return 1;}
    }
    return 0;
}

//Assume that tokc >= 1
// number of commands
int set_commands(char** tokens, int tokc){
    int i = 0;
    int currArgs = 0;
    int comIndex = 0;

    while(i < tokc){
        if(i == 0 && (strcmp(tokens[i], "then") == 0 || strcmp(tokens[i], "else") == 0)) {
            i++;
            continue;
        }
        
        if(strcmp(tokens[i], "|") == 0 || strcmp(tokens[i], "\n") == 0) {
            comIndex++;
            currArgs = 0;
        }
        else if(strcmp(tokens[i], "<") == 0){
            i++;
            commands[comIndex].inputFile = tokens[i];
        }
        else if(strcmp(tokens[i], ">") == 0){
            i++;
            commands[comIndex].outputFile = tokens[i];
        }
        else{
            commands[comIndex].arguments[currArgs] = tokens[i];
            currArgs++;
            commands[comIndex].argc = currArgs;
        }
        i++;
    }
    return comIndex+1;
}


//get tokens takes in the string user input, a char** to store the tokens in, and tokc (how many tokens)
//there are no returns: the array tokens is modified to store all the tokens present in 'line'
void get_tokens(char* line , char** tokens, int tokc){
    int index = 0;
    char* token = strtok(line, " ");
    while(token != NULL){
        if(is_empty_or_whitespace(token) == false){ //only non-empty tokens are stored in the array
            tokens[index++] = token;
            int length = strlen(token);
            if(tokens[index - 1][length - 1] == '\n') tokens[index-1][length-1] = '\0';
        }
        token = strtok(NULL,  " ");
    }
    tokens[index] = NULL;
}

//This function counts how many tokens is in a string line (excludes whitespace)
int count_tokens(char* original_line){
    int tokc = 0;
    char* line = strdup(original_line); //a duplicate line must be made since strtok modifies strings
    char* countTokens = strtok(line, " ");
    while(countTokens != NULL){
        if(is_empty_or_whitespace(countTokens) == false){ //tokc only increases if the token is not empty
            tokc++;
        }
        countTokens = strtok(NULL, " ");
    }

    free(line); //freeing the duplicate string
    return tokc;
}

//Built-in Commands

//Change working directory
//path argument is a path to a directory
//Prints an error and fails if it is given the wrong number of args, or if chdir() fails
int cd(const char* path){
    int status = chdir(path);
    if(status != 0){
        perror("could not change directory to specified path");
        return 1;
    }
    else return 0;
}


//Prints the current working directory to StdOut
//Returns 0 success, 1 on failure
int pwd(){
    char cwd[PATH_MAX];

    if(getcwd(cwd, sizeof(cwd)) != NULL){
        printf("%s\n", cwd);
    }
    else{
        perror("Could not get current working directory");
        return 1;
    }
    return 0;

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
        lastStatus = 0;
        return strdup(path); // Return a copy of the path
       }
   }
   lastStatus = 1;
   return NULL; // Return NULL if the program is not found
}

void echo(char** arguments, int argc){
    for(int i = 1; i < argc; i++){
        printf("%s ", arguments[i]);
    }
    printf("\n");
}

void exit_mysh(char* line){
    printf("mysh: exiting");
    free(line);
    exit(1);
}