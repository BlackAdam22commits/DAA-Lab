/*
 * DAA Lab-04, Q2: Application of sorting-II
 * -----------------------------------------
 * Problem : Given sets S1, S2 (each size n) and a target x, determine
 *           whether some a in S1 and b in S2 satisfy a + b = x.
 *
 * Approach: 1. Sort S2                                   -> O(n log n)
 *           2. For each a in S1, binary search for (x-a) in sorted S2
 *              -> O(log n) per element, O(n log n) total
 *
 * Complexity: O(n log n) time, O(1) extra space (sort in place).
 */

#include <stdio.h>
#include <stdlib.h>

int cmpInt(const void *a, const void *b) {
    return (*(const int *)a) - (*(const int *)b);
}

/* Binary search for `target` in sorted arr[0..n-1]. Returns index or -1. */
int binarySearch(const int *arr, int n, int target) {
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

/*
 * Finds a pair (S1[i], S2[j]) with S1[i] + S2[j] == x.
 * Returns 1 and sets *outA, *outB if found; returns 0 otherwise.
 * Mutates a local sorted copy of S2, leaves caller's arrays untouched.
 */
int findPairSummingTo(const int *S1, int n1, const int *S2, int n2,
                       int x, int *outA, int *outB) {
    int *sortedS2 = (int *)malloc(n2 * sizeof(int));
    for (int i = 0; i < n2; i++) sortedS2[i] = S2[i];
    qsort(sortedS2, n2, sizeof(int), cmpInt);

    int found = 0;
    for (int i = 0; i < n1 && !found; i++) {
        int need = x - S1[i];
        int idx = binarySearch(sortedS2, n2, need);
        if (idx != -1) {
            *outA = S1[i];
            *outB = sortedS2[idx];
            found = 1;
        }
    }

    free(sortedS2);
    return found;
}

void printArr(const char *label, const int *arr, int n) {
    printf("%s: [", label);
    for (int i = 0; i < n; i++) printf("%d%s", arr[i], (i + 1 < n) ? ", " : "");
    printf("]\n");
}

void runTest(const char *label, const int *S1, int n1, const int *S2, int n2, int x) {
    printf("--- %s ---\n", label);
    printArr("S1", S1, n1);
    printArr("S2", S2, n2);
    printf("x = %d\n", x);

    int a, b;
    if (findPairSummingTo(S1, n1, S2, n2, x, &a, &b))
        printf("Result: FOUND  (%d + %d = %d)\n\n", a, b, a + b);
    else
        printf("Result: NOT FOUND\n\n");
}

int main(void) {
    int S1a[] = {1, 4, 8, 12, 20};
    int S2a[] = {3, 7, 9, 15, 17};
    runTest("Test 1: pair exists (4+9=13)", S1a, 5, S2a, 5, 13);

    int S1b[] = {1, 2, 3};
    int S2b[] = {100, 200, 300};
    runTest("Test 2: no pair exists", S1b, 3, S2b, 3, 50);

    int S1c[] = {-5, -2, 0, 3, 10};
    int S2c[] = {-8, -1, 2, 6, 9};
    runTest("Test 3: negative numbers (0+9=9? try 3+6=9)", S1c, 5, S2c, 5, 9);

    int S1d[] = {5};
    int S2d[] = {5};
    runTest("Test 4: n=1, exact match", S1d, 1, S2d, 1, 10);

    int S1e[] = {5};
    int S2e[] = {5};
    runTest("Test 5: n=1, no match", S1e, 1, S2e, 1, 11);

    return 0;
}
