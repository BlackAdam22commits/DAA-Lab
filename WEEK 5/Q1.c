/* ============================================================
   DAA Lab-05, Q1: Median of N numbers WITHOUT sorting the list
   ============================================================
   Input representation: array of n integers, int a[n].

   Key idea: the median is just the k-th smallest element for
   k = ceil(n/2) (and for even n, the average of the n/2-th and
   (n/2+1)-th smallest). Finding the k-th smallest element does
   NOT require a full sort -- it is the classical SELECTION
   problem, solvable in O(n) worst-case time using the
   "median-of-medians" (BFPRT) algorithm, without ever fully
   ordering the array.

   ---------------- Median-of-medians (BFPRT) selection ----------------
   select(a, lo, hi, k):
     1. If (hi - lo + 1) is small (<= 5), sort that tiny group
        directly (O(1) work) and return the k-th smallest.
     2. Otherwise, divide a[lo..hi] into groups of 5. Find the
        median of each group (O(1) per group since group size
        is fixed at 5) -- this costs O(n) overall.
     3. Recursively find the median of these ~n/5 group-medians;
        call it "pivot".
     4. Partition a[lo..hi] around "pivot" (like Quickselect's
        partition step) -- O(n).
     5. If pivot's rank (index) == k, return it. If k is smaller,
        recurse on the left part; else recurse on the right part
        with an adjusted k.

   ---------------- Worst-case complexity ----------------
   Because the pivot (median of medians) is guaranteed to be
   greater than at least 3 elements in each of at least half of
   the ceil(n/5) groups, and smaller than at least 3 elements in
   at least half of the groups, it is guaranteed to eliminate at
   least ~30% of the array at every recursive call. This gives
   the recurrence:
        T(n) <= T(n/5) + T(7n/10) + O(n)
   Since n/5 + 7n/10 = 9n/10 < n, this recurrence solves to
        T(n) = O(n)          (worst case, NOT just expected case)
   Hence the median can be found in O(n) time WITHOUT sorting
   the array (sorting would cost O(n log n)).
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* insertion sort for tiny groups (<=5 elements) -- O(1) per group */
void insertion_sort(int *a, int lo, int hi) {
    for (int i = lo + 1; i <= hi; i++) {
        int key = a[i], j = i - 1;
        while (j >= lo && a[j] > key) { a[j + 1] = a[j]; j--; }
        a[j + 1] = key;
    }
}

void swap(int *a, int i, int j) { int t = a[i]; a[i] = a[j]; a[j] = t; }

/* partition a[lo..hi] around the value a[pivot_index]; returns final
   index of the pivot (standard Lomuto partition) -- O(hi-lo+1) */
int partition_around(int *a, int lo, int hi, int pivot_index) {
    int pivot_val = a[pivot_index];
    swap(a, pivot_index, hi);
    int store = lo;
    for (int i = lo; i < hi; i++) {
        if (a[i] < pivot_val) { swap(a, i, store); store++; }
    }
    swap(a, store, hi);
    return store;
}

/* median-of-medians: returns the index of a good pivot for a[lo..hi] */
int median_of_medians(int *a, int lo, int hi) {
    int n = hi - lo + 1;
    if (n <= 5) {
        insertion_sort(a, lo, hi);
        return lo + n / 2;
    }

    int num_groups = (n + 4) / 5;
    int *medians = malloc(num_groups * sizeof(int));

    for (int g = 0; g < num_groups; g++) {
        int group_lo = lo + g * 5;
        int group_hi = group_lo + 4;
        if (group_hi > hi) group_hi = hi;
        insertion_sort(a, group_lo, group_hi);
        int mid_idx = group_lo + (group_hi - group_lo) / 2;
        medians[g] = a[mid_idx];
    }

    /* move the medians to the front of a temporary contiguous block
       so we can recursively select the median of medians */
    int *tmp = malloc(num_groups * sizeof(int));
    for (int g = 0; g < num_groups; g++) tmp[g] = medians[g];

    /* recursively find median of the medians array (k = middle rank) */
    extern int select_kth(int *a, int n, int k); /* forward decl */
    int mom_value = select_kth(tmp, num_groups, (num_groups + 1) / 2);

    free(medians);
    free(tmp);

    /* find an index in a[lo..hi] holding mom_value, to use as pivot index */
    for (int i = lo; i <= hi; i++)
        if (a[i] == mom_value) return i;
    return lo; /* fallback (shouldn't happen) */
}

/* select_kth: returns the k-th smallest (1-indexed) value in a[0..n-1].
   Worst-case O(n) via median-of-medians pivot selection. */
int select_kth_range(int *a, int lo, int hi, int k) {
    if (lo == hi) return a[lo];

    int pivot_index = median_of_medians(a, lo, hi);
    pivot_index = partition_around(a, lo, hi, pivot_index);

    int rank = pivot_index - lo + 1; /* 1-indexed rank within [lo,hi] */
    if (k == rank) return a[pivot_index];
    else if (k < rank) return select_kth_range(a, lo, pivot_index - 1, k);
    else return select_kth_range(a, pivot_index + 1, hi, k - rank);
}

int select_kth(int *a, int n, int k) {
    return select_kth_range(a, 0, n - 1, k);
}

/* median(): uses select_kth to find the median WITHOUT sorting */
double median_no_sort(int *a, int n) {
    /* select_kth partitions "a" in place as a side effect; work on a copy
       so the caller's original array order is preserved */
    int *copy = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) copy[i] = a[i];

    double result;
    if (n % 2 == 1) {
        result = select_kth(copy, n, (n + 1) / 2);
    } else {
        /* need the two middle values; select_kth partitions around each */
        int *copy2 = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) copy2[i] = a[i];
        int lower = select_kth(copy, n, n / 2);
        int upper = select_kth(copy2, n, n / 2 + 1);
        result = (lower + upper) / 2.0;
        free(copy2);
    }
    free(copy);
    return result;
}

/* ---------------- driver / validation ---------------- */
int cmp_int(const void *x, const void *y) { return (*(int *)x - *(int *)y); }

double median_by_sorting(int *a, int n) { /* for cross-check only */
    int *tmp = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) tmp[i] = a[i];
    qsort(tmp, n, sizeof(int), cmp_int);
    double r = (n % 2) ? tmp[n / 2] : (tmp[n / 2 - 1] + tmp[n / 2]) / 2.0;
    free(tmp);
    return r;
}

int main(void) {
    srand((unsigned) time(NULL));

    int a[] = {12, 3, 5, 7, 4, 19, 26, 1, 15, 8, 9, 22, 6};
    int n = sizeof(a) / sizeof(a[0]);

    printf("Array: [");
    for (int i = 0; i < n; i++) printf("%d%s", a[i], i == n - 1 ? "" : ", ");
    printf("]\n");

    double m1 = median_no_sort(a, n);
    double m2 = median_by_sorting(a, n);
    printf("Median (no-sort, O(n) selection) = %.2f\n", m1);
    printf("Median (via sorting, for validation) = %.2f\n", m2);
    printf("Match: %s\n\n", (m1 == m2) ? "YES" : "NO - BUG");

    /* ---------------- empirical timing vs sort-based median ---------------- */
    printf("--- Empirical timing (selection is O(n), sort-based is O(n log n)) ---\n");
    for (int size = 10000; size <= 160000; size *= 2) {
        int *arr = malloc(size * sizeof(int));
        for (int i = 0; i < size; i++) arr[i] = rand();

        clock_t t0 = clock();
        median_no_sort(arr, size);
        clock_t t1 = clock();
        median_by_sorting(arr, size);
        clock_t t2 = clock();

        printf("n=%7d  no-sort: %.6fs   sort-based: %.6fs\n",
               size, (double)(t1 - t0) / CLOCKS_PER_SEC, (double)(t2 - t1) / CLOCKS_PER_SEC);
        free(arr);
    }
    return 0;
}
