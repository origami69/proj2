#define _POSIX_C_SOURCE 200809L
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

static double now_sec(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + 1e-9 * (double)ts.tv_nsec;
}

static double work(double v)
{
    double a = sqrt(fabs(v));
    a += sin(a) * cos(v);
    a += log1p(fabs(v)) * 0.5;
    a += atan(a);
    return a;
}

typedef struct {
    const double *x;
    long   lo;        /* first index this thread owns          */
    long   hi;        /* one past the last index it owns       */
    double partial;   /* this thread writes here and nowhere else */
} targ_t;

static void *sum_range(void *arg)
{
    targ_t *data = (targ_t *)arg;
    double partial = 0;
    const double *get = data->x;
    int limiter = data->hi;
    for (int i = data->lo; i < limiter; i++){
        partial += work(get[i]);
    }
    data->partial = partial;
    return NULL;
}

static int run_once(double *x, long n, int nthreads,
                    double *fill_out, double *sum_out, double *total_out)
{
    double t0 = now_sec();
    for (long i = 0; i < n; i++) x[i] = sin((double)i);
    *fill_out = now_sec() - t0;

    double t1 = now_sec();
    //this will setup the thread array and the structs
    pthread_t workers[nthreads];
    targ_t *setters = malloc(nthreads * sizeof(targ_t));
    if (!setters) { fprintf(stderr, "tsum: cannot allocate %ld array of structs\n", n); return -1; }
    int rc;
    //loop that creates the threads and sets the structs for each thread
    for (int i = 0; i < nthreads; i++){
        //setup the structs
        setters[i].lo = ((long)i * n) / nthreads;
        setters[i].hi = ((long)(i + 1) * n) / nthreads;
        setters[i].x = x;
        //make thread and see if made succesfully according to section 4.6, input is pthreadm attributes, routine to run, and arguments to pass
        rc = pthread_create(&workers[i], NULL, sum_range, &setters[i]);
        if(rc != 0){
            fprintf(stderr, "tsum: pthread_create for thread %d: %s\n", i, strerror(rc));
            free(setters);
            return -1;
        }
    }
    //join threads
    for (int i = 0; i < nthreads; i++){
        rc = pthread_join(workers[i], NULL);
        if(rc != 0){
            fprintf(stderr, "tsum: pthread_create for thread %d: %s\n", i, strerror(rc));
            free(setters);
            return -1;
        }
    }
    //go through each thread squentially from 0 the nth nthread count and add each partial to total
    double total = 0.0;
    for (int i = 0; i < nthreads; i++){
        total += setters[i].partial;
    }
    *sum_out   = now_sec() - t1;
    (void)nthreads;
    *total_out = total;
    free(setters);
    return 0;
}

int main(int argc, char **argv)
{
    long n = 20000000;
    int nthreads = 1, reps = 3;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--threads") && i + 1 < argc)    nthreads = atoi(argv[++i]);
        else if (!strcmp(argv[i], "--n") && i + 1 < argc)     n        = atol(argv[++i]);
        else if (!strcmp(argv[i], "--reps") && i + 1 < argc)  reps     = atoi(argv[++i]);
        else { fprintf(stderr, "usage: %s [--threads T] [--n N] [--reps R]\n", argv[0]); return 2; }
    }
    if (nthreads < 1 || n < 1 || reps < 1) {
        fprintf(stderr, "tsum: --threads, --n and --reps must all be >= 1\n");
        return 2;
    }

    double *x = malloc((size_t)n * sizeof *x);
    if (!x) { fprintf(stderr, "tsum: cannot allocate %ld doubles\n", n); return 1; }

    double best_wall = 0.0, best_fill = 0.0, best_sum = 0.0, total = 0.0;
    for (int r = 0; r < reps; r++) {
        double f, s, t;
        if (run_once(x, n, nthreads, &f, &s, &t) != 0) { free(x); return 1; }
        double wall = f + s;
        if (r == 0 || wall < best_wall) {
            best_wall = wall; best_fill = f; best_sum = s; total = t;
        }
    }

    printf("threads   %d\n", nthreads);
    printf("n         %ld\n", n);
    printf("reps      %d\n", reps);
    printf("total     %.6f\n", total);
    printf("fill_s    %.4f\n", best_fill);
    printf("sum_s     %.4f\n", best_sum);
    printf("wall_s    %.4f\n", best_wall);

    free(x);
    return 0;
}
