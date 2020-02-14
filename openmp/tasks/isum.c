#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#define N 1000000  // we'll sum this many numbers

float sum(const float *a, size_t n)
{
    float total;
    size_t i;

    for (i = 0, total = 0.; i < n; i++) {
        total += a[i];
    }
    return total;
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
