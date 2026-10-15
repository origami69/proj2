/*
 * BUG 1.  Each thread is supposed to print its own index, 0 through 3.
 * Run it a few times.  The indices come out wrong, repeated, or out of range.
 *
 * Fix it so every run prints 0, 1, 2 and 3 exactly once each, in some order.
 * Write one sentence in bugs/ANSWERS.md naming the actual defect.
 */
#define _POSIX_C_SOURCE 200809L
#include <pthread.h>
#include <stdio.h>

#define N 4 //num of threads

static void *show(void *arg) //executed by each thread
{
    int id = *(int *)arg; //convert to int, then dereference to get the value
    printf("thread index %d\n", id); //print
    return NULL; //end thread
}

/* //Original code 
int main(void)
{
    pthread_t t[N];
    for (int i = 0; i < N; i++)
        pthread_create(&t[i], NULL, show, &i);
    for (int i = 0; i < N; i++)
        pthread_join(t[i], NULL);
    return 0;
}
*/

int main(void)
{
    pthread_t t[N]; //array to store the 4 thread ids
    int ids[N]; //array to hold thread indices

    for (int i = 0; i < N; i++) { //loop 4 times
        ids[i] = i; //store current index
        pthread_create(&t[i], NULL, show, &ids[i]); //create thread, pass address to its own index
    }
    for (int i = 0; i < N; i++) //loop 4 times (wait for threads to finish)
        pthread_join(t[i], NULL);

    return 0; //end program
}