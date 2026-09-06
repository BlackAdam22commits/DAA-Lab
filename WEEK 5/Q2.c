/* ============================================================
   DAA Lab-05, Q2: K'th smallest element WITHOUT sorting the list
   ============================================================
   Input representation: array of n integers, int a[n], and an
   integer k (1-indexed: k=1 means smallest element).

   Algorithm: Quickselect (Hoare, 1961) -- a divide-and-conquer
   selection algorithm, closely related to Quicksort but which
   recurses into only ONE side of the partition (the side that
   contains the k-th element) instead of both, so it never needs
   to fully order the array.

   quickselect(a, lo, hi, k):
     1. Pick a pivot (here: random index in [lo,hi], to avoid
        worst-case behaviour on already-sorted/adversarial input).
     2. Partition a[lo..hi] around the pivot (Lomuto scheme) --
        O(hi-lo+1) work; let p be the pivot's final index.
     3. rank = p - lo + 1 (1-indexed rank of pivot within [lo,hi]).
        - if k == rank: return a[p]           (found it)
        - if k <  rank: recurse into a[lo .. p-1]
        - if k >  rank: recurse into a[p+1 .. hi] with k -= rank

   ---------------- Complexity ----------------
   Average case: each partition step is expected to roughly halve
   the search range (with a random pivot), giving
        T(n) = T(n/2) + O(n)  =>  T(n) = O(n)   (expected)
   Worst case: an adversarial/unlucky sequence of pivots (e.g.
   always picking the min or max) shrinks the range by only 1
   element per step:
        T(n) = T(n-1) + O(n)  =>  T(n) = O(n^2)  (worst case)
   Using RANDOM pivots (as done here) makes the O(n^2) worst case
   extremely unlikely in practice; a deterministic O(n) WORST-CASE
   algorithm exists (median-of-medians / BFPRT, see Q1) and can be
   substituted as the pivot-selection step to guarantee O(n) even
   in the worst case, at the cost of a larger constant factor.
   In all cases, the array is never fully sorted.
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int i, int j) { int t = a[i]; a[i] = a[j]; a[j] = t; }

/* Lomuto partition around a[pivot_index]; returns final pivot index.
   Cost: O(hi - lo + 1) */
int partition(int *a, int lo, int hi, int pivot_index) {
    int pivot_val = a[pivot_index];
    swap(a, pivot_index, hi);
    int store = lo;
    for (int i = lo; i < hi; i++) {
        if (a[i] < pivot_val) { swap(a, i, store); store++; }
    }
    swap(a, store, hi);
    return store;
}

/* quickselect: returns the k-th smallest (1-indexed) value of a[lo..hi] */
int quickselect(int *a, int lo, int hi, int k) {
    while (1) {
        if (lo == hi) return a[lo];

        int pivot_index = lo + rand() % (hi - lo + 1); /* random pivot */
        pivot_index = partition(a, lo, hi, pivot_index);

        int rank = pivot_index - lo + 1;
        if (k == rank) return a[pivot_index];
        else if (k < rank) hi = pivot_index - 1;
        else { k -= rank; lo = pivot_index + 1; }
    }
}

int kth_smallest_no_sort(int *a, int n, int k) {
    int *copy = malloc(n * sizeof(int)); /* preserve caller's array */
    for (int i = 0; i < n; i++) copy[i] = a[i];
    int result = quickselect(copy, 0, n - 1, k);
    free(copy);
    return result;
}

/* ---------------- driver / validation ---------------- */
int cmp_int(const void *x, const void *y) { return (*(int *)x - *(int *)y); }

int kth_smallest_by_sorting(int *a, int n, int k) { /* for cross-check only */
    int *tmp = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) tmp[i] = a[i];
    qsort(tmp, n, sizeof(int), cmp_int);
    int r = tmp[k - 1];
    free(tmp);
    return r;
}

int main(void) {
    srand((unsigned) time(NULL));

    int a[] = {12, 3, 5, 7, 4, 19, 26, 1, 15, 8, 9, 22, 6};
    int n = sizeof(a) / sizeof(a[0]);

    printf("Array: [");
    for (int i = 0; i < n; i++) printf("%d%s", a[i], i == n - 1 ? "" : ", ");
    printf("]\n\n");

    for (int k = 1; k <= n; k++) {
        int r1 = kth_smallest_no_sort(a, n, k);
        int r2 = kth_smallest_by_sorting(a, n, k);
        printf("k=%2d : quickselect = %2d, sort-based = %2d  %s\n",
               k, r1, r2, (r1 == r2) ? "OK" : "MISMATCH-BUG");
    }

    /* ---------------- empirical timing ---------------- */
    printf("\n--- Empirical timing (quickselect ~O(n) avg, sort-based O(n log n)) ---\n");
    for (int size = 10000; size <= 160000; size *= 2) {
        int *arr = malloc(size * sizeof(int));
        for (int i = 0; i < size; i++) arr[i] = rand();
        int k = size / 3;

        clock_t t0 = clock();
        kth_smallest_no_sort(arr, size, k);
        clock_t t1 = clock();
        kth_smallest_by_sorting(arr, size, k);
        clock_t t2 = clock();

        printf("n=%7d  quickselect: %.6fs   sort-based: %.6fs\n",
               size, (double)(t1 - t0) / CLOCKS_PER_SEC, (double)(t2 - t1) / CLOCKS_PER_SEC);
        free(arr);
    }
    return 0;
}
