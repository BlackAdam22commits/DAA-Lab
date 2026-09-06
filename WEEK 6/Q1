/* ============================================================
   DAA Lab-06, Q1: 1D array operations and their complexities
   ============================================================
   Input representation: array of n unsorted integers, stored as
   a dynamically allocated int[] with an explicit length n.

   Each routine below is timed and its worst-case complexity is
   stated in the header comment above the function.
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/* ---------- (i) Maximum element ----------
   Worst case: O(n)  -- single linear scan, every element must be
   examined at least once (adversary can hide the max at the end). */
int find_max(int *a, int n) {
    int max = a[0];
    for (int i = 1; i < n; i++)
        if (a[i] > max) max = a[i];
    return max;
}

/* ---------- (ii) First and second largest ----------
   Worst case: O(n) -- one linear pass, keep two running values. */
void first_second_largest(int *a, int n, int *first, int *second) {
    if (a[0] > a[1]) { *first = a[0]; *second = a[1]; }
    else             { *first = a[1]; *second = a[0]; }
    for (int i = 2; i < n; i++) {
        if (a[i] > *first) { *second = *first; *first = a[i]; }
        else if (a[i] > *second) { *second = a[i]; }
    }
}

/* ---------- (iii) Mean ----------
   Worst case: O(n) -- sum all n elements once. */
double mean(int *a, int n) {
    long long sum = 0;
    for (int i = 0; i < n; i++) sum += a[i];
    return (double) sum / n;
}

/* comparator for qsort */
static int cmp_int(const void *x, const void *y) {
    return (*(int *)x - *(int *)y);
}

/* ---------- (iv) Median ----------
   Worst case: O(n log n) using comparison sort (qsort) then O(1)
   lookup of the middle element(s).
   Note: an O(n) worst-case median is possible with the
   median-of-medians (BFPRT) selection algorithm, but the
   straightforward/robust approach used here is O(n log n). */
double median(int *a, int n) {
    int *tmp = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) tmp[i] = a[i];
    qsort(tmp, n, sizeof(int), cmp_int);
    double result;
    if (n % 2 == 1) result = tmp[n / 2];
    else result = (tmp[n / 2 - 1] + tmp[n / 2]) / 2.0;
    free(tmp);
    return result;
}

/* ---------- (v) Standard deviation ----------
   Worst case: O(n) -- one pass for mean (already have it), one
   more pass to accumulate squared deviations => O(n) + O(n) = O(n). */
double std_dev(int *a, int n, double m) {
    double sum_sq = 0.0;
    for (int i = 0; i < n; i++) {
        double d = a[i] - m;
        sum_sq += d * d;
    }
    return sqrt(sum_sq / n);
}

/* ---------- (vi) Mode ----------
   Worst case: O(n log n) -- sort the array, then a single linear
   scan finds the longest run (the most frequent value).
   (If the value range were bounded/small, counting-sort style
   hashing would give expected O(n), but for arbitrary integers
   comparison-sort based O(n log n) is the safe worst case.) */
int mode(int *a, int n) {
    int *tmp = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) tmp[i] = a[i];
    qsort(tmp, n, sizeof(int), cmp_int);

    int best_val = tmp[0], best_count = 1;
    int cur_val = tmp[0], cur_count = 1;
    for (int i = 1; i < n; i++) {
        if (tmp[i] == cur_val) cur_count++;
        else { cur_val = tmp[i]; cur_count = 1; }
        if (cur_count > best_count) { best_count = cur_count; best_val = cur_val; }
    }
    free(tmp);
    return best_val;
}

/* ---------- (vii) Remove all duplicates ----------
   Worst case: O(n log n) -- sort, then a single linear pass keeps
   only the first occurrence of each value (compacting in place).
   Returns the new length. */
int remove_duplicates(int *a, int n) {
    if (n == 0) return 0;
    qsort(a, n, sizeof(int), cmp_int);
    int write = 1;
    for (int read = 1; read < n; read++) {
        if (a[read] != a[write - 1]) {
            a[write] = a[read];
            write++;
        }
    }
    return write;
}

/* ---------- (viii) Reverse the array ----------
   Worst case: O(n) -- swap i-th and (n-1-i)-th elements, n/2 swaps. */
void reverse_array(int *a, int n) {
    int i = 0, j = n - 1;
    while (i < j) {
        int t = a[i]; a[i] = a[j]; a[j] = t;
        i++; j--;
    }
}

/* ---------- (ix) Partition w.r.t a random pivot ----------
   Requirement: after partitioning, ALL elements < pivot appear
   AFTER all elements >= pivot (i.e., the mirror image of the
   usual Lomuto/Hoare layout).
   Worst case: O(n) -- one linear pass (Lomuto-style, mirrored). */
int partition_reverse(int *a, int n, int pivot_index) {
    int pivot = a[pivot_index];
    /* Move pivot to the end temporarily */
    int t = a[pivot_index]; a[pivot_index] = a[n - 1]; a[n - 1] = t;

    int store = 0; /* boundary: [0, store) holds elements >= pivot */
    for (int i = 0; i < n - 1; i++) {
        if (a[i] >= pivot) {
            t = a[i]; a[i] = a[store]; a[store] = t;
            store++;
        }
    }
    /* place pivot right after the ">= pivot" block */
    t = a[store]; a[store] = a[n - 1]; a[n - 1] = t;
    return store; /* index where pivot ended up */
}

/* ---------------- driver / validation ---------------- */
void print_array(const char *label, int *a, int n) {
    printf("%s: [", label);
    for (int i = 0; i < n; i++) printf("%d%s", a[i], i == n - 1 ? "" : ", ");
    printf("]\n");
}

int main(void) {
    srand((unsigned) time(NULL));

    int n = 12;
    int a[12] = {7, 3, 9, 3, 5, 1, 9, 2, 8, 5, 3, 7};

    print_array("Original array", a, n);

    printf("(i)   Max                 = %d\n", find_max(a, n));

    int first, second;
    first_second_largest(a, n, &first, &second);
    printf("(ii)  First, Second largest = %d, %d\n", first, second);

    double m = mean(a, n);
    printf("(iii) Mean                 = %.4f\n", m);

    printf("(iv)  Median                = %.4f\n", median(a, n));
    printf("(v)   Std Dev               = %.4f\n", std_dev(a, n, m));
    printf("(vi)  Mode                  = %d\n", mode(a, n));

    /* work on copies so later steps still have the original data */
    int b[12]; for (int i = 0; i < n; i++) b[i] = a[i];
    int new_len = remove_duplicates(b, n);
    print_array("(vii) After remove_duplicates", b, new_len);

    int c[12]; for (int i = 0; i < n; i++) c[i] = a[i];
    reverse_array(c, n);
    print_array("(viii) Reversed array", c, n);

    int d[12]; for (int i = 0; i < n; i++) d[i] = a[i];
    int pivot_idx = rand() % n;
    int placed_at = partition_reverse(d, n, pivot_idx);
    printf("(ix)  Pivot value = %d, final index = %d\n", a[pivot_idx], placed_at);
    print_array("(ix)  Partitioned (>=pivot first, <pivot after)", d, n);

    /* ---------------- empirical timing (sanity check of growth) ---------------- */
    printf("\n--- Empirical timing (grows as expected with n) ---\n");
    for (int size = 1000; size <= 8000; size *= 2) {
        int *arr = malloc(size * sizeof(int));
        for (int i = 0; i < size; i++) arr[i] = rand() % 100000;

        clock_t t0 = clock();
        find_max(arr, size);
        clock_t t1 = clock();
        median(arr, size);
        clock_t t2 = clock();

        printf("n=%6d  max: %.6fs (O(n))   median: %.6fs (O(n log n))\n",
               size,
               (double)(t1 - t0) / CLOCKS_PER_SEC,
               (double)(t2 - t1) / CLOCKS_PER_SEC);
        free(arr);
    }

    return 0;
}
