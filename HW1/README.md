A README file containing the name and netID of both partners, your test plan, descriptions
of your test programs (including any arguments they may take), and any design notes you
think are worth pointing out. This should be a plain text file.

GROUP PARTNERS:
Anuraag Sarkar - as3895
Nicholas Chen - nhc29

MALLOC DESIGN NOTES:

FREE DESIGN NOTES:

Performance Testing:
    The performance testing function getAllTests(int iterations) gets the timimings of how long it takes each of the 5 tests over the specified number of iterations

    To test for 50 iterations, do getAllTests(50);
    
    This function will return the time it takes for the computer to run 50 iterations of each test case.
    Additionally, before testing times, the getAllTests() function says whether memory has been cleared after each test has been run.

    This function calls upon the function performanceTest(int testNum, int iterations). performanceTest takes in the test number and number of iterations testing will be done over for that test, and get the average time it took for the test to run.

    do NOT run each test individually. There are no print statements in the individual tests, so you will not get any feedback. To test a single iteration of all test, do getAllTests(1);

TESTING PLAN:

DESCRIPTION OF TEST PROGRAMS:

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