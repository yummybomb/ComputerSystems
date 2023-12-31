Partners:
Nicholas Chen - nhc29
Anuraag Sarkar - as3895

WORD RULES:
Any alphabetical character is a part of a word
hypens must be before AND after a letter
apostraphe can be before OR after any letter 
Anything else (whitespace, symbols) are not part of a word


PROCESS FILE:

Process file opens a specified file and counts the words in that file. Words are stored in the buffer words, which doubles in size every time the array is full.

After adding all words in a file to the hashmap, the function frees all temporary objects and closes the channel for the specified file.

LOGIC FOR WORD COUNTING:

To check how many words are in the file, 3 characters are kept: prev, c, and next. C is the character checked in the function isValidCharacter.

There are 3 returns for isValidCharacter. 1 means continue the current word, 0 means end the current word, and 2 is a special case where an apostraphe is the start of a new word.

There are 3 things isValidCharacter checks for:

1. The character is a letter, in which case it is valid since all letters are part of a word.
2. The character is a hyphen. In this case, we check if prev and next are characters. If they are characters, the hyphen is part of the word. Otherwise, it is not
3. Apostraphes are special. If there is a word directly before the apostraphe, it is a part of the current word, and we return 1 (continue the current word). If an apostraphe has an invalid character before it but a letter after it, we break the current word and start a new word, with the begging of the word containing an apostraphe. If an apostraphe has no valid characters next to it, we return 0 (break current word).

Using the isValidCharacter, we add characters to the buffer array 'words,' and every time isValidCharacter returns 0 or 2, we add a terminating character to the end of the word (for formatting when printing later) and send the word to the hash map and start our next word.

DIRECTORY TRAVERSAL:

The function processDirectory is a recursive function used to traverse through a specified directory. The function opens the directory and checks all contents that are not itself (.) and its parent (..). For each other item in the directory, the function processes any files and runs processDirectory for any directory. processfile adds all words from all files to the hashmap map.

HASH MAP:

The hash map is used to store words. Although its worst case time is the same as keeping track of an array of words, the overall average time for storing and accessing items from the hashmap is better, hence, we used a hashmap implementation.

The hash map has 2 major functions: map_get and map_set. The map uses key-value pairs. The keys are the literal words, while the values are how many times the words have been encountered.

QUICKSORT IMPLEMENTATION FOR SORTING WORDS:

For sorting and printing the contents of the hash map, two arrays are created: one for the word, another for the number of times the word appeared.

First, we sort by number. We sort the words with the same frequencies together in order. Every time a change is made, both arrays are changed. After this is done, the words with the same frequency are sorted alphabetically.

The program prints each word and its frequency in different lines when the sorting is finished.

TESTING:

To test the file, create a text file or directory containing text files and other directories, and fill the text files with words and content. Then, add the files/directories as arguments when running words.c (make sure to make before doing ./words (arguments)).

Additionally, the function printMap(map) is provided in the hash file to print the contents of the map. NOTE: The contents are NOT sorted. TO use this function, do printMap(map) anywhere in words.c.