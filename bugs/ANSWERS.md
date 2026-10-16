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
Defect: compute() would pass the the memory address of answer back to main, but since it's a local variable on the 
worker thread's stack, it would get destroyed when the thread finishes before main could use it.
Fix: Change the pass by reference to a pass by value using the same method in bug1_args; cast it to intptr and then
back into a pointer so it doesn't disappear when the local stack is gone.

## bug4_errno.c
Defect: We were checking for a '-1' result, but the pthread function would return errors directly, marking any
return that was not '-1' as a success when it should have been a failure.
Fix: 
Instead of checking for errors by checking if create and join equal -1, we should compare if they do not equal 0. 
Zero represents a successful run and we can have many types of errors beyond just -1, so changing it to !0 throws 
an error for anything that isn't the normal result. We can also save the output in the result variable and print 
the a string error of that variable, letting us know the specific one.
Instead of checking for errors by checking if join and create equal -1 we should compare if join and create do not 
equal 0 since 0 represents a successful run and we can have many types of errors not just -1 and checking if we 
dont run succesfully will alllow us to catch the error better 
Then we can save the outputs of create worker and join in the result variable and print the string error of that 
variable since we create and join do use errno but returns the errors by themselves.
