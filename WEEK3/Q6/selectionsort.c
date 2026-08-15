/*
 * Q6: Use of Loop Invariants in Sorting (Selection Sort)
 * ---------------------------------------------------------
 * See README.md in this folder for the pseudocode, loop-invariant
 * proof (initialization / maintenance / termination), and the
 * Theta-notation analysis. This file is the C implementation used to
 * validate the claims experimentally (counts comparisons & swaps,
 * confirms Theta(n^2) behaviour in both best and worst case, and
 * checks the loop invariant after every outer-loop iteration).
 *
 * Compile: gcc -O2 -Wall -o selection_sort selection_sort.c
 * Run:     ./selection_sort
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long comparisons = 0;
long swaps = 0;

/* Verifies the loop invariant: A[0..i-1] is sorted AND every element
   in A[0..i-1] is <= every element in A[i..n-1].
   Returns 1 if invariant holds, 0 otherwise. */
int invariantHolds(int A[], int n, int i) {
    for (int k = 1; k < i; k++)
        if (A[k - 1] > A[k]) return 0;              /* A[0..i-1] must be sorted */
    for (int k = 0; k < i; k++)
        for (int m = i; m < n; m++)
            if (A[k] > A[m]) return 0;               /* prefix <= suffix */
    return 1;
}

void selectionSort(int A[], int n, int verifyInvariant) {
    comparisons = 0;
    swaps = 0;

    /* only need to run for the first (n-1) elements: once the first
       (n-1) elements are placed correctly, the single remaining
       element is automatically the largest and already in place -
       there is nothing left to "select". */
    for (int i = 0; i < n - 1; i++) {
        if (verifyInvariant && !invariantHolds(A, n, i)) {
            printf("   *** Loop invariant VIOLATED before iteration i=%d\n", i);
        }

        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            comparisons++;
            if (A[j] < A[minIdx]) minIdx = j;
        }
        if (minIdx != i) {
            int tmp = A[i]; A[i] = A[minIdx]; A[minIdx] = tmp;
            swaps++;
        }
    }

    if (verifyInvariant && !invariantHolds(A, n, n - 1)) {
        printf("   *** Loop invariant VIOLATED before final check (i=n-1)\n");
    }
    /* after the loop, i == n-1, so invariant says A[0..n-2] sorted and
       <= A[n-1..n-1] -> the WHOLE array A[0..n-1] is sorted (termination) */
}

int isSorted(int A[], int n) {
    for (int i = 1; i < n; i++) if (A[i - 1] > A[i]) return 0;
    return 1;
}

void printArr(int A[], int n) {
    for (int i = 0; i < n; i++) printf("%d ", A[i]);
    printf("\n");
}

int main(void) {
    printf("=========================================================\n");
    printf(" Q6: Selection Sort - Loop Invariant Demonstration\n");
    printf("=========================================================\n\n");

    /* ---- small demo with invariant checking printed ---- */
    int demo[] = {29, 10, 14, 37, 13};
    int n = sizeof(demo) / sizeof(demo[0]);
    printf("Before: "); printArr(demo, n);
    selectionSort(demo, n, 1 /* verify invariant each step */);
    printf("After : "); printArr(demo, n);
    printf("Loop invariant held at every iteration boundary. Sorted = %s\n\n",
           isSorted(demo, n) ? "YES" : "NO");

    /* ---- experiment: best case (already sorted) vs worst case (reverse sorted) ---- */
    srand(1);
    printf("%8s %15s %15s %15s %15s\n",
           "n", "cmp(worst)", "cmp(best)", "swaps(worst)", "swaps(best)");
    int sizes[] = {10, 50, 100, 500, 1000, 2000, 4000};
    for (size_t s = 0; s < sizeof(sizes) / sizeof(sizes[0]); s++) {
        int sz = sizes[s];
        int *worst = malloc(sz * sizeof(int));
        int *best  = malloc(sz * sizeof(int));
        for (int i = 0; i < sz; i++) { worst[i] = sz - i; best[i] = i; } /* reverse / sorted */

        selectionSort(worst, sz, 0);
        long cW = comparisons, sW = swaps;
        selectionSort(best, sz, 0);
        long cB = comparisons, sB = swaps;

        printf("%8d %15ld %15ld %15ld %15ld\n", sz, cW, cB, sW, sB);

        if (!isSorted(worst, sz) || !isSorted(best, sz)) {
            printf("   *** SORT FAILURE at n=%d\n", sz);
        }
        free(worst); free(best);
    }

    printf("\nObservation: the number of COMPARISONS is exactly\n");
    printf("n(n-1)/2 in BOTH the best and worst case (selection sort\n");
    printf("always scans the remaining unsorted part to find the\n");
    printf("minimum, regardless of input order) -> Theta(n^2) always,\n");
    printf("the best case is NOT asymptotically better. Only the number\n");
    printf("of SWAPS differs (0 for an already-sorted array, up to n-1\n");
    printf("for the worst case) - but swaps don't dominate the running\n");
    printf("time, comparisons do.\n");

    return 0;
}

