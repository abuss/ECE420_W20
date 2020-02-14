#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <omp.h>

#define N 1000000  // we'll sum this many numbers

float sum(const float *a, size_t n)
{
    // int my_rank = omp_get_thread_num();
    // int thread_count = omp_get_num_threads();

    // printf("Thread %d of %d   n: %ld\n", my_rank, thread_count, n);

    // base cases
    if (n == 0) {
        return 0;
    }
    else if (n == 1) {
        return *a;
    }

    // recursive case
    size_t half = n / 2;
    float x, y;

    #pragma omp task shared(x)
    x = sum(a, half);
    #pragma omp task shared(y)
    y = sum(a + half, n - half);
    #pragma omp taskwait
    x += y;

    return x;
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

    #pragma omp parallel 
    #pragma omp single nowait
    printf("%f\n", sum(a, N));
    
    return 0;
}
