PARTNERS:
Anuraag Sarkar - as3895
Nicholas Chen - nhc29

Testing Plan

Testing Strategy:

For testing, there are a few important contents to keep in mind:

1. Built in command should be tested, along with conditionals
2. Error handling should be tested
3. Pipin and redirection should be tested
4. Commands that were not built in (like wc or ls) should be tested as well

All sh files are stored in testcases/(sh file). To test these test cases, batch mode can be used on these sh files. There are two txt files in subdir (one for input and output) which is used to test redirection.

Feel free to create your own input/output files (or have mysh create output files for you)

Test Cases:

thenelse.sh: this test case is used to check if the then else statements are functioning correctly.

piperedirect.sh: this is a compilation of multiple pipe and redirect lines. The test case works if all test cases have a non-error output 

errors.sh: this is used to test if errors are printed correctly. There are multiple lines, and all lines should have some form of error.

These errors are all-around and test the entirety of mysh.


Mysh Documentation:

There is an additional file called test.c, which is not used at all. This is because it contains code for wildcard handling, which we weren't able to implement into mysh, however, we would like to show our thought process for handling wild cards with the find_matches program.

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