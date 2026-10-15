/*
 * BUG 2.  Four threads each add 1000 to their own slot.  main then reports the
 * total, which should be 4000.  It usually reports less, and the number changes
 * between runs.
 *
 * Fix it so the total is 4000 on every run.
 * Write one sentence in bugs/ANSWERS.md naming the actual defect.
 */
#define _POSIX_C_SOURCE 200809L
#include <pthread.h>
#include <stdio.h>

#define N 4 //number of threads

static long slot[N]; //array to hold each thread's result

static void *add_mine(void *arg) //is done by each thread
{
    long k = (long)arg; //get index
    for (int i = 0; i < 1000; i++) slot[k]++; //add 1k to the slot
    return NULL; //end
}

/* //Original code
int main(void)
{
    pthread_t t[N];
    for (long k = 0; k < N; k++)
        pthread_create(&t[k], NULL, add_mine, (void *)k);

    long total = 0;
    for (int k = 0; k < N; k++) total += slot[k];
    printf("total %ld (expected 4000)\n", total);
    return 0;
}
*/

int main(void)
{
    pthread_t t[N]; //array to store 4 thread ids

    for (long k = 0; k < N; k++)
        pthread_create(&t[k], NULL, add_mine, (void *)k); //create each thread

    for (int k = 0; k < N; k++)
        pthread_join(t[k], NULL); //wait for each thread to finish

    long total = 0; //initialize total
    for (int k = 0; k < N; k++)
        total += slot[k]; //add each thread's result

    printf("total %ld (expected 4000)\n", total); //print total
    return 0; //end program
}