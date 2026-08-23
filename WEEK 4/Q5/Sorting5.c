/*
 * DAA Lab-04, Q5: Application of sorting-V
 * -------------------------------------------
 * Problem : Given n intervals (x_i, y_i), merge all overlapping intervals.
 *           E.g. {(1,3),(2,6),(8,10),(7,18)} -> {(1,6),(7,18)}.
 *
 * Approach: 1. Sort intervals by start point x_i           -> O(n log n)
 *           2. Single linear scan keeping a "current" merged interval
 *              [curStart, curEnd]; for the next interval (x, y):
 *                - if x <= curEnd  : overlaps/touches -> curEnd = max(curEnd, y)
 *                - else            : close out current, start new (x, y)
 *              -> O(n)
 *
 * Complexity: O(n log n) time (dominated by the sort), O(n) extra space.
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x, y;
} Interval;

int cmpIntervalByStart(const void *a, const void *b) {
    const Interval *ia = (const Interval *)a;
    const Interval *ib = (const Interval *)b;
    return ia->x - ib->x;
}

/*
 * Merges overlapping intervals in `intervals` (size n).
 * Writes the merged result into `out` (caller must allocate at least n slots).
 * Returns the number of intervals in the merged result.
 */
int mergeIntervals(const Interval *intervals, int n, Interval *out) {
    if (n == 0) return 0;

    Interval *sorted = (Interval *)malloc(n * sizeof(Interval));
    for (int i = 0; i < n; i++) sorted[i] = intervals[i];
    qsort(sorted, n, sizeof(Interval), cmpIntervalByStart);

    int outLen = 0;
    Interval cur = sorted[0];

    for (int i = 1; i < n; i++) {
        if (sorted[i].x <= cur.y) {
            /* overlaps (or touches) current merged interval -> extend */
            if (sorted[i].y > cur.y) cur.y = sorted[i].y;
        } else {
            /* no overlap -> close out current, start new */
            out[outLen++] = cur;
            cur = sorted[i];
        }
    }
    out[outLen++] = cur;

    free(sorted);
    return outLen;
}

void printIntervals(const char *label, const Interval *arr, int n) {
    printf("%s: {", label);
    for (int i = 0; i < n; i++)
        printf("(%d,%d)%s", arr[i].x, arr[i].y, (i + 1 < n) ? ", " : "");
    printf("}\n");
}

void runTest(const char *label, const Interval *in, int n) {
    printf("--- %s ---\n", label);
    printIntervals("Input ", in, n);

    Interval *out = (Interval *)malloc(n * sizeof(Interval));
    int outLen = mergeIntervals(in, n, out);
    printIntervals("Output", out, outLen);
    printf("\n");
    free(out);
}

int main(void) {
    /* Test 1: exact example from the lab sheet */
    Interval t1[] = {{1, 3}, {2, 6}, {8, 10}, {7, 18}};
    runTest("Test 1: lab sheet example (expect {(1,6),(7,18)})", t1, 4);

    /* Test 2: no overlaps at all */
    Interval t2[] = {{1, 2}, {5, 6}, {10, 12}};
    runTest("Test 2: no overlaps", t2, 3);

    /* Test 3: everything merges into one */
    Interval t3[] = {{1, 4}, {2, 5}, {3, 6}, {4, 8}};
    runTest("Test 3: all merge into one", t3, 4);

    /* Test 4: touching intervals (endpoint equal) should merge */
    Interval t4[] = {{1, 5}, {5, 10}};
    runTest("Test 4: touching endpoints merge", t4, 2);

    /* Test 5: unsorted input, single interval nested inside another */
    Interval t5[] = {{10, 20}, {1, 30}, {5, 8}};
    runTest("Test 5: nested interval absorbed", t5, 3);

    /* Test 6: single interval */
    Interval t6[] = {{1, 1}};
    runTest("Test 6: single interval", t6, 1);

    return 0;
}
