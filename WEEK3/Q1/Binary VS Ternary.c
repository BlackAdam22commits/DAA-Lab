/*
 * Q1: Binary vs Ternary Search
 * -----------------------------
 * Compares binary search (splits into 2 parts) with ternary search
 * (splits into 3 parts) on a sorted array, counting key comparisons
 * done against the array (comparisons used to decide which sub-interval
 * to recurse into). Also runs a small experiment across many array
 * sizes and writes the WORST-CASE comparison counts to a CSV file so
 * they can be plotted.

#include <stdio.h>
#include <stdlib.h>

/* ---------- Binary Search ---------- */
/* returns index of x if found, else -1. *comparisons is incremented
   for every comparison made against an array element. */
int binarySearch(int arr[], int n, int x, long *comparisons) {
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;

        (*comparisons)++;
        if (arr[mid] == x) return mid;

        (*comparisons)++;
        if (x < arr[mid]) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return -1;
}

/* ---------- Ternary Search ---------- */
int ternarySearch(int arr[], int n, int x, long *comparisons) {
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid1 = low + (high - low) / 3;
        int mid2 = high - (high - low) / 3;

        (*comparisons)++;
        if (arr[mid1] == x) return mid1;

        (*comparisons)++;
        if (arr[mid2] == x) return mid2;

        (*comparisons)++;
        if (x < arr[mid1]) {
            high = mid1 - 1;
        } else {
            (*comparisons)++;
            if (x > arr[mid2]) {
                low = mid2 + 1;
            } else {
                low = mid1 + 1;
                high = mid2 - 1;
            }
        }
    }
    return -1;
}

/* Worst case comparisons for a given n: search for a value NOT present
   (forces the algorithm to go all the way down to an empty interval).
   We average over a few "not-present" query points to get a stable
   worst-case-ish figure. */
void worstCaseCounts(int n, long *binCount, long *terCount) {
    int *arr = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) arr[i] = 2 * i; /* even numbers, sorted */

    long binTotal = 0, terTotal = 0;
    int trials = 0;
    for (int t = 1; t < n; t += (n / 20 > 0 ? n / 20 : 1)) {
        int query = 2 * t - 1; /* always odd -> never present */
        long c1 = 0, c2 = 0;
        binarySearch(arr, n, query, &c1);
        ternarySearch(arr, n, query, &c2);
        binTotal += c1;
        terTotal += c2;
        trials++;
    }
    *binCount = binTotal / (trials > 0 ? trials : 1);
    *terCount = terTotal / (trials > 0 ? trials : 1);
    free(arr);
}

int main(void) {
    /* ---- Part A: simple demo on a small array ---- */
    int demo[] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30};
    int n = sizeof(demo) / sizeof(demo[0]);
    int target = 22;

    long cB = 0, cT = 0;
    int idxB = binarySearch(demo, n, target, &cB);
    int idxT = ternarySearch(demo, n, target, &cT);

    printf("=========================================================\n");
    printf(" Q1: Binary Search vs Ternary Search\n");
    printf("=========================================================\n");
    printf("Demo array size n = %d, searching for x = %d\n", n, target);
    printf("Binary  search -> found at index %2d, comparisons = %ld\n", idxB, cB);
    printf("Ternary search -> found at index %2d, comparisons = %ld\n\n", idxT, cT);

    /* ---- Part B: experiment across sizes, write CSV for plotting ---- */
    FILE *fp = fopen("search_results.csv", "w");
    if (!fp) { perror("fopen"); return 1; }
    fprintf(fp, "n,binary_comparisons,ternary_comparisons\n");

    printf("%10s %20s %20s\n", "n", "avg binary cmp", "avg ternary cmp");
    int sizes[] = {10, 50, 100, 500, 1000, 5000, 10000, 50000,
                   100000, 500000, 1000000, 5000000};
    for (size_t i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++) {
        long b, t;
        worstCaseCounts(sizes[i], &b, &t);
        printf("%10d %20ld %20ld\n", sizes[i], b, t);
        fprintf(fp, "%d,%ld,%ld\n", sizes[i], b, t);
    }
    fclose(fp);

    printf("\nObservation:\n");
    printf("Binary search makes ~log2(n) comparisons per level but only\n");
    printf("1-2 array comparisons per level (~2*log2(n) total in worst case).\n");
    printf("Ternary search makes ~log3(n) LEVELS but up to 4 array\n");
    printf("comparisons per level, giving ~4*log3(n) ~= 2.52*log2(n) total.\n");
    printf("So even though ternary search has fewer recursion LEVELS,\n");
    printf("it performs MORE total comparisons than binary search, which\n");
    printf("is confirmed by the counts above (see search_results.csv / graph).\n");

    return 0;
}
