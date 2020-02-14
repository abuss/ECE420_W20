/*
 gcc -g -Wall -o quickSort quickSort.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "timer.h"


int partition (int p, int r, float *data)
{
    float x = data[p];
    int k = p;
    int l = r + 1;
    float t;
    
    while (1) {
        do
            k++;
        while ((data[k] <= x) && (k < r));
        
        do
            l--;
        while (data[l] > x);
        
        while (k < l) {
            t = data[k];
            data[k] = data[l];
            data[l] = t;
            
            do
                k++;
            while (data[k] <= x);
            
            do
                l--;
            while (data[l] > x);
        }
            
        t = data[p];
        data[p] = data[l];
        data[l] = t;
        return l;
    }
}


void seq_quick_sort (int p, int r, float *data)
{
    if (p < r) {
        int q = partition (p, r, data);
        seq_quick_sort (p, q - 1, data);
        seq_quick_sort (q + 1, r, data);
    }
}

void quick_sort (int p, int r, float *data, int low_limit)
{
    if (p < r) {
        if ((r - p) < low_limit) {
            seq_quick_sort (p, r, data);
        }
        else {
            int q = partition (p, r, data);
            quick_sort (p, q - 1, data, low_limit);
            quick_sort (q + 1, r, data, low_limit);
        }
    }
}

void validate_sort (int n, float *data)
{
    int i;
    for (i = 0; i < n - 1; i++) {
        if (data[i] > data[i+1]) {
            printf ("Validate failed\n");
        }
    }
    printf ("Validate passed\n");
}

int main (int argc, char *argv[])
{
    int i, n, low_limit;
    float *data;
    double start, finish;
    
    if (argc != 3) {
        printf ("a.out num_elems low_limit\n");
        return 1;
    }
    n = atoi(argv[1]);
    low_limit = atoi(argv[2]);
    
    /*
     * Generate the array.
     */
    data = (float *)malloc (sizeof (float) * n);
    for ( i=0; i<n; i++ ) {
        data[i] = 1.1 * rand() * 5000 / RAND_MAX;
    }
    printf ("\nSorting %d numbers seqentially...\n\n", n);
    
    GET_TIME(start);
    quick_sort (0, n - 1, &data[0], low_limit);
    GET_TIME(finish);
    printf("Elapsed time = %e seconds\n", finish - start);
    
    validate_sort(n, &data[0]);
    free (data);
    return 0;
}




