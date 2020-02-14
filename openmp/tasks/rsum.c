#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#define N 1000000  // we'll sum this many numbers

float sum(const float *a, size_t n)
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
    return sum(a, half) + sum(a + half, n - half);
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
