#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <omp.h>

#define N 1000000  // we'll sum this many numbers
#define CUTOFF 100


float serial_sum(const float *a, size_t n)
{
    // base cases
    if (n == 0) {
        return 0;
    }
    else if (n == 1) {
        return *a;
    }

    // recursive case
    size_t half = n / 2;
    return serial_sum(a, half) + serial_sum(a + half, n - half);
}

float parallel_sum(const float *a, size_t n)
{
    // int my_rank = omp_get_thread_num();
    // int thread_count = omp_get_num_threads();

    // printf("Thread %d of %d   n: %ld\n", my_rank, thread_count, n);

    // base case
    if (n <= CUTOFF) {
        return serial_sum(a, n);
    }

    // recursive case
    float x, y;
    size_t half = n / 2;

    #pragma omp task shared(x)
    x = parallel_sum(a, half);
    #pragma omp task shared(y)
    y = parallel_sum(a + half, n - half);
    #pragma omp taskwait
    // x += y;
    return x + y;
}


float sum(const float *a, size_t n)
{
    float r;

    #pragma omp parallel
    #pragma omp single nowait
    r = parallel_sum(a, n);
    return r;
}


int main()
{
    float* a = malloc(N * sizeof(float));
    if (a == NULL) {
        perror("malloc");
        return 1;
    }

    // fill the array a
    for (size_t i = 0; i < N; i++) {
        a[i] = .000001;
    }

    printf("%f\n", sum(a, N));
    return 0;
}
