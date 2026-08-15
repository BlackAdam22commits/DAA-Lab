/*
 * Q3: Maximum and Minimum using Divide & Conquer (<= 3n/2 comparisons)
 * ----------------------------------------------------------------------
 * Naive scan: 2(n-1) comparisons.
 *
 * D&C approach (element-pairing form, which hits the tight bound for
 * every n, not just powers of two):
 *   DIVIDE : group the elements into floor(n/2) pairs (plus one
 *            leftover element if n is odd).
 *   CONQUER: for each pair, ONE comparison tells us the "local winner"
 *            (bigger) and "local loser" (smaller) of that pair.
 *   COMBINE: the overall maximum can only be among the local winners,
 *            and the overall minimum can only be among the local
 *            losers, so we linear-scan each of those two (half-sized)
 *            lists once.
 *
 * Comparisons:
 *   pairing        : floor(n/2)
 *   scan winners   : floor(n/2) - 1   (finding max of the winners)
 *   scan losers    : floor(n/2) - 1   (finding min of the losers)
 *   -----------------------------------------------------------
 *   total (n even) : n/2 + (n/2 - 1) + (n/2 - 1) = 3n/2 - 2
 *   total (n odd)  : one extra element folded in with 2 comparisons,
 *                    giving exactly ceil(3n/2) - 2
 *
 * This matches the textbook tight bound ceil(3n/2) - 2 for ALL n
 * (a naive recursive halving down to single elements can overshoot
 * this bound by a small constant for some non-power-of-two n, which
 * is why we pair explicitly instead).
 *
 * Compile: gcc -O2 -Wall -o maxmin maxmin.c
 * Run:     ./maxmin
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct { int max, min; } Pair;

long comparisons = 0;

Pair maxMin(int arr[], int lo, int hi) {
    Pair result;
    int n = hi - lo + 1;

    if (n == 1) {
        result.max = result.min = arr[lo];
        return result;
    }
    if (n == 2) {
        comparisons++;
        if (arr[lo] > arr[hi]) { result.max = arr[lo]; result.min = arr[hi]; }
        else                   { result.max = arr[hi]; result.min = arr[lo]; }
        return result;
    }

    int start = lo;
    if (n % 2 == 1) {
        /* odd size: fold the very first element in as the initial
           max/min "seed", then pair up the remaining even-length rest */
        result.max = result.min = arr[lo];
        start = lo + 1;
    } else {
        /* even size: seed with the first pair (1 comparison) */
        comparisons++;
        if (arr[lo] > arr[lo + 1]) { result.max = arr[lo]; result.min = arr[lo + 1]; }
        else                       { result.max = arr[lo + 1]; result.min = arr[lo]; }
        start = lo + 2;
    }

    /* process the rest strictly in pairs: 3 comparisons per pair
       (1 to find the pair's local winner/loser, 1 to update running
       max, 1 to update running min) */
    for (int i = start; i + 1 <= hi; i += 2) {
        int a = arr[i], b = arr[i + 1];
        int localMax, localMin;
        comparisons++;
        if (a > b) { localMax = a; localMin = b; }
        else       { localMax = b; localMin = a; }

        comparisons++;
        if (localMax > result.max) result.max = localMax;

        comparisons++;
        if (localMin < result.min) result.min = localMin;
    }
    return result;
}

/* Naive linear scan, for comparison purposes */
Pair naiveMaxMin(int arr[], int n, long *cmp) {
    Pair result;
    result.max = result.min = arr[0];
    *cmp = 0;
    for (int i = 1; i < n; i++) {
        (*cmp)++;
        if (arr[i] > result.max) result.max = arr[i];
        (*cmp)++;
        if (arr[i] < result.min) result.min = arr[i];
    }
    return result;
}

int main(void) {
    printf("=========================================================\n");
    printf(" Q3: Max & Min using Divide and Conquer (<= 3n/2 comparisons)\n");
    printf("=========================================================\n\n");

    srand(7);

    /* ---- small correctness demo ---- */
    int demo[] = {12, 45, 3, 67, 89, 1, 23, 56, 90, 5};
    int n = sizeof(demo) / sizeof(demo[0]);
    comparisons = 0;
    Pair p = maxMin(demo, 0, n - 1);
    printf("Demo array of size %d -> max = %d, min = %d, comparisons = %ld\n",
           n, p.max, p.min, comparisons);
    printf("Tight bound ceil(3n/2) - 2 = %d\n\n", (int)((3 * n + 1) / 2) - 2);

    /* ---- experiment across sizes, write CSV ---- */
    FILE *fp = fopen("maxmin_results.csv", "w");
    if (!fp) { perror("fopen"); return 1; }
    fprintf(fp, "n,dc_comparisons,naive_comparisons,bound_3n_2\n");

    printf("%10s %18s %18s %20s\n", "n", "D&C cmp", "naive cmp", "bound ceil(3n/2)-2");
    int sizes[] = {2, 5, 10, 50, 100, 500, 1000, 5000, 10000,
                   50000, 100000, 500000, 1000000};
    for (size_t i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++) {
        int sz = sizes[i];
        int *arr = malloc(sizeof(int) * sz);
        for (int j = 0; j < sz; j++) arr[j] = rand();

        comparisons = 0;
        Pair r1 = maxMin(arr, 0, sz - 1);
        long dcCmp = comparisons;

        long naiveCmp;
        Pair r2 = naiveMaxMin(arr, sz, &naiveCmp);

        int bound = (int)((3 * sz + 1) / 2) - 2;
        printf("%10d %18ld %18ld %20d\n", sz, dcCmp, naiveCmp, bound);
        fprintf(fp, "%d,%ld,%ld,%d\n", sz, dcCmp, naiveCmp, bound);

        if (r1.max != r2.max || r1.min != r2.min) {
            printf("   *** MISMATCH at n=%d!\n", sz);
        }
        if (dcCmp > bound) {
            printf("   *** exceeded bound at n=%d (%ld > %d)\n", sz, dcCmp, bound);
        }
        free(arr);
    }
    fclose(fp);

    printf("\nObservation: the D&C comparison count matches the tight\n");
    printf("ceil(3n/2) - 2 bound for every n, while the naive scan needs\n");
    printf("about 2n - 2 comparisons - roughly 25%% more work. See\n");
    printf("maxmin_results.csv / graph for the trend.\n");
    return 0;
}
