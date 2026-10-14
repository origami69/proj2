# Part B answers

For each program, one sentence naming the actual defect, and one sentence
saying what you changed. Do not paste code here; the code is in the .c files.

## bug1_args.c
Defect: We were passing the pointer value of i to show, but pcreate_thread would finish and start the next
loop show would finish, meaning sometimes i would already increment.
Fix: Pass i by value instead of memory address by casting it to an int and then back into a pointer, so
show doesn't look at a changing memory address and instead a static value.
Helpful Resources:
https://learn.microsoft.com/en-us/answers/questions/227662/what-is-the-differences-between-int*-and-intptr
https://stackoverflow.com/questions/153065/converting-a-pointer-into-an-integer

## bug2_nojoin.c
Defect:
Fix:

## bug3_stack.c
Defect:
Fix:

## bug4_errno.c
Defect:
Fix:
