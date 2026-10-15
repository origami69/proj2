# Part B answers

For each program, one sentence naming the actual defect, and one sentence
saying what you changed. Do not paste code here; the code is in the .c files.

## bug1_args.c
Defect:

All 4 threads get the address of the same variable which makes them print incorrect and repeated numbers.

Fix:

Created an integer array while passing the index address of the thread to keep the values unchanged.


## bug2_nojoin.c
Defect:

Main thread was finding the total before the thread finished, making the result vary.

Fix:

Added 'pthread_join' to wait for the threads to finish first before calculating.


## bug3_stack.c
Defect:

Thread returned the address of a local variable that was not valid when the function finished.

Fix:

Created an array in main to store the thread results, keeping the values valid until the threads finished. 


## bug4_errno.c
Defect:

It was checking for a '-1' result but the pthread function returned errors directly, marking any return that was not '-1' as a failure when it should have. 

Fix:

Changed the checks to test for nonzero return values.

