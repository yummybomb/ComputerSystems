PARTNERS:
Anuraag Sarkar - as3895
Nicholas Chen - nhc29

Testing Plan

Testing Strategy:



Test Cases:



Mysh Documentation:

There are two modes: Batch mode and Interactive mode. Batch runs when an argument is given, interactive when no arguments are given.

the read_line function processes an input line. The read_line function also adds whitespace before special characters < > |, which will be useful later when tokenizing.

Tokenizer:

For our mysh program, we have two functions for tokenizing: count_tokens and get_tokens. count_tokens gives us a count for how many tokens are in an input line, which we use to set the arrays to store the actual tokens.

get_tokens stores all the tokens in a char** array 'tokens.' Tokens are considered non-empty strings that are seperated by whitespace. Special characters < > and | count as their own, courtesy of the read_line function adding whitespace before and after these special characters before tokenization begins.

Once tokenization is complete, get_arguments is used to fill up an array of structs called 'Command.'

The command struct keeps track of the following pieces of information:

1. argc (number of arguments)
2. command (the first 'argument')
3. An array of arguments (following the command)
4. InputFile (if any < is present);
5. OutpueFile (if any > is present);

If a pipe is present, multiple structs are made. A struct is made containing tokens before a pipe, and another struct is made for tokens after a pipe. These structs are stored in the global array 'commands.'

These structures are fed to process_line, which handles the following:

1. Then/Else handling. the function then_else_status is used to check preemptively if a function will run before the 'Command' structs are created. If then is given after the previous command fails, or if else is given after the previous command succeeds, then the program will stop running that line.
2. After then/else is handled, process_line looks at the structs and runs the commands based on the contents of the struct. If the struct has an input or output file specified, process_line will handle that accordingly.
3. All built-in commands (such as pwd and cd) are checked and handled in process_line as well.
4. If a pipe is present, process_line will also pipe accordingly.




*TODO:*

Modes:
- Batch Mode
- Interactive Mode

Testing / README:
- Testing Code
- Test inputs
- README: testing strategy / cases, consiered scenarios and how those checks were performed. Also general code information.

Specifications:
- Determine the number of child processes to start & for each:

1. The path to the executable file
2. List of argument strings
3. Which file sto use for standard IO
^Process command line in stages (tokens)

- Wildcard handling

- Redirection: < and >
- Pipelines
