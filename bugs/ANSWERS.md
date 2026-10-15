# Part B answers

For each program, one sentence naming the actual defect, and one sentence
saying what you changed. Do not paste code here; the code is in the .c files.

## bug1_args.c
Defect: We were passing the pointer value of i to show, but pcreate_thread would finish and start the next
loop before show could finish, meaning sometimes i would already increment.
Fix: Pass i by value instead of memory address by casting it to a intptr int and then back into a pointer, so
show doesn't look at a changing memory address and instead a static value.
Helpful Resources:
https://learn.microsoft.com/en-us/answers/questions/227662/what-is-the-differences-between-int*-and-intptr
https://stackoverflow.com/questions/153065/converting-a-pointer-into-an-integer
https://stackoverflow.com/questions/22624737/casting-a-pointer-to-an-int

## bug2_nojoin.c
Defect: Main would keep going before some of the threads could finish executing, meaning they didnt get to 1000 yet.
Fix: We need to use pthread_join for each thread to make main wait for them to finish working first.
Helpful Resources:
https://stackoverflow.com/questions/11624545/how-to-make-main-thread-wait-for-all-child-threads-finish
https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_join.html

## bug3_stack.c
Defect: compute() would pass the the memory address of a local variable back to main, but since its local it would
get destroyed before main could use it.
Fix: Change the pass by reference to a pass by value using the same method in bug1_args; cast it to intptr and then
back into a pointer so it doesn't disappear when the local stack is gone.

## bug4_errno.c
Defect: We would return a success even though inside the create worker function when we run with an env we should return a error
 if (getenv("CS433_FAIL_CREATE") != NULL) {
        int rc = pthread_create(t, NULL, idle, NULL);
        return (rc == 0) ? EAGAIN : rc;
    }
if we have an env we should return EAGAIN whenever the thread creates successfully otherwise if we get a real error then we report that real error(EAGAIN is error number 11 reporting Insufficient resources to create another thread)
Fix: 
Instead of checking for errors by checking if join and create equal -1 we should compare if join and create do not equal 0 since 0 represents a successful run and we can have many types of errors not just -1 and checking if we dont run succesfully will alllow us to catch the error better 
