A README file containing the name and netID of both partners, your test plan, descriptions
of your test programs (including any arguments they may take), and any design notes you
think are worth pointing out. This should be a plain text file.


GROUP PARTNERS:
Anuraag Sarkar - as3895
Nicholas Chen - nhc29


MALLOC DESIGN NOTES:


	The malloc function allocates a memory block of the users' requested size + the size of the header and then returns a ptr to the userData. 


	First the function checks if our heap is uninitialized. If it is, we initialize the malloced header and the header of the free shortly after.


	Now we go through 3 cases as we iterate through the heap: 


Case 1 (chunkisFree AND chunkSize == userSize + headerSize):
	Mark the current chunk as allocated, and return the pointer of the data.
Case 2 (chunk isFree AND chunkSize > userSize + headerSize):
	We cut the current chunk by shortening the header size, marking the first part allocated, and making the next chunk free and the remainder of the chunk. Then we return the pointer of the data.
Case 3(chunk not free or chunkSize < userSize + headerSize):
	We move onto the next space where we can fit the chunk
If the while loop ends without any return, we know that there is no space left in the chunk, so we print an error message that displays the file and line number where it is called and return null.


FREE DESIGN NOTES:


	The Free function frees any pointer that has been malloc’d.
	
	First, myfree checks if the pointer passed is a valid pointer. A pointer is valid if the pointer is free and if the pointer is the start of a chunk. Read “Errors in Free” for more information. The following conditions must be met for the pointer to be valid:
		1: The pointer is not already freed
		2: The pointer points to the start of a chunk


If the pointer is valid, the pointer designates the chunk as free.


	Once it is verified that the pointer is valid, we iterate through every single chunk in the heap. If the current and next chunk is free, then the function “CoalesceNextChunk” is called.


	CoalesceNextChunk is only called when:
		1: There is an available ‘next chunk’
		2: If there is an available ‘next chunk,’ both the current and next chunk must be free.
	If both conditions are met, CoalesceNextChunk(ptr) is called.


	CoalesceNextChunk(void *ptr) takes a pointer to a chunk and coalesces it with the chunk after it. This is done by changing the first chunk size to include the next chunk (ptr = size of ptr1 + size of ptr2).


	At the end of the program, the pointer passed should be free, and all free chunks that can be coalesced are coalesced.


Performance Testing:
    The performance testing function getAllTests(int iterations) gets the timings of how long it takes each of the 5 tests over the specified number of iterations


    To test for 50 iterations, do getAllTests(50);
   
    This function will return the time it takes for the computer to run 50 iterations of each test case.
    Additionally, before testing times, the getAllTests() function says whether memory has been cleared after each test has been run.


    This function calls upon the function performanceTest(int testNum, int iterations). performanceTest takes in the test number and number of iterations testing will be done over for that test, and gets the average time it took for the test to run.


    do NOT run each test individually. There are no print statements in the individual tests, so you will not get any feedback. To test a single iteration of all test, do getAllTests(1);


TESTING PLAN:


    Testmallocfree.c: correctness testing we get 7 objects of size 512 (large objects) and allocate them with malloc, and then utilize the <string.h> library to memset them with a distinct byte pattern. 
    Then we will check if the objects follow their respective byte pattern after memsetting all the objects. 
    This will prove that the user is able to allocate memory and write memory onto the heap. 
    After, we free the memory and check if the heap is free. If it is, all the tests are successful and we print a success. 
    If there is a failure on any part of this test, we will print a fail message and the respective failure.




DESCRIPTION OF TEST PROGRAMS (memgrind.c):


	Test 4:
		Test 4 checks if an array of alternating items (int, char) can be malloc’d and freed. This is done by having a for loop that mallocs integer pointers every odd i, and character pointers every even i. Free also frees alternatively. Free frees integer pointers every even i, and character pointers every odd i.
		By alternatively using different types, we can ensure that our mymalloc and myfree functions work well with chunks of differing adjacent sizes.


	Test 5:
		Tests an array of varying sizes, shuffles the array sizes using fisher yates algorithm before mallocing in a certain order, putting them into a ptr array, and then randomizing them with fisher yates algorithm before deallocating them in the randomed order. This ensures that our algorithm, when not seeded, shouldn’t make errors for mallocs varying sizes and that the allocation and deallocation works on varying orders.


ERRORS:


    Errors in mymalloc():


    "[ERROR] in file (filename) at line (line number): cannot allocate 0 bytes"
        This error occurs when the user tries to use malloc() with parameter 0.
        Because malloc() is used to allocate space, malloc() cannot allocate a pointer to a space of.


    "[ERROR] in file (filename) at line (line number): not enough memory"
        This error occurs when the user tries to use malloc() while the heap has no free space large enough for the size passed.
        The heap does not have to be completely full to get this error. This error can also occur if there is no available chunk of the passed size. For instance, if the user tries malloc(100), and the largest free block is size 90, then this error will be shown.


    Errors in myfree:


    "[ERROR] in file (filename) at line (line number): Pointer is already free"
        This error occurs when the pointer passed through myfree points to a chunk that is already free
        This error only shows if the following conditions are met:
            1: The pointer points to the start of a valid chunk
            2: The chunk the pointer points to is free


    "[ERROR] in file (filename) at line (line number): Pointer does not point to the start of a chunk"
        This error occurs when the pointer passed through myfree does not point to the start of a valid chunk
        This error can occur if the pointer points to the middle of a valid chunk, or if the pointer is not in the heap

TESTING TIMES:

Here are the times we got by using the getAllTests() on iLab:

Testing times for 50 iterations

Test 1 time:
6.620000 microseconds
Test 2 time:
122.320000 microseconds
Test 3 time:
12.320000 microseconds
Test 4 time:
123.100000 microseconds
Test 5 time:
2.480000 microseconds