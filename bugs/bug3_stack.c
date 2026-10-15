/*
 * BUG 3.  Each thread computes a value and hands it back through
 * pthread_join.  The numbers main prints are garbage, or the program crashes,
 * or it prints the right thing on your laptop and the wrong thing on the
 * course server.
 *
 * Fix it so main reliably prints 100, 200, 300, 400 for threads 1 through 4.
 * Write one sentence in bugs/ANSWERS.md naming the actual defect.
 */
#define _POSIX_C_SOURCE 200809L
#include <pthread.h>
#include <stdio.h>

#define N 4

//struct to hold thread index and result
typedef struct {
    long k; //thread index
    long answer; //thread result
} thread_data;

static void *compute(void *arg) //executed by each thread
{
    thread_data *data = (thread_data *)arg; //get thread's data

    data->answer = (data->k + 1) * 100; //calculate result

    return &data->answer; //return address of stored result
}

/* //Original main
int main(void)
{
    pthread_t t[N];
    for (long k = 0; k < N; k++)
        pthread_create(&t[k], NULL, compute, (void *)k);

    for (int k = 0; k < N; k++) {
        void *ret;
        pthread_join(t[k], &ret);
        printf("thread %d returned %ld\n", k + 1, *(long *)ret);
    }
    return 0;
}
*/

int main(void)
{
    pthread_t t[N]; //array to store thread ids
    thread_data data[N]; //array to store each thread's data

    for (long k = 0; k < N; k++) {
        data[k].k = k; //assign thread index

        pthread_create(&t[k], NULL, compute, &data[k]); //create thread with its own data
    }

    for (int k = 0; k < N; k++) {
        void *ret = NULL; //pointer to receive result

        pthread_join(t[k], &ret); //wait for thread to finish, get address

        printf("thread %d returned %ld\n", k + 1, *(long *)ret); //print result
    }

    return 0; //end program
}