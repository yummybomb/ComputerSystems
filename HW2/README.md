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

QUICKSORT IMPLEMENTATION FOR SORTING WORDS: