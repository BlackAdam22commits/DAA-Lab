/*
 * Q2: Search the Defective (Lighter) Coin
 * ----------------------------------------
 * We have n coins, all supposed to weigh exactly W. At most ONE coin
 * may be lighter than W (never heavier). We must find that coin using
 * a balance scale, or report that none is defective, using a
 * DIVIDE AND CONQUER strategy that takes about log2(n) weighings.
 *
 * IMPORTANT: a real balance scale only tells you which PAN is lighter,
 * so the two pans being compared must always hold the SAME NUMBER of
 * coins (otherwise a heavier pan could simply be the one with more
 * coins, which would corrupt the result). We therefore split the
 * current candidate group into two EQUAL-SIZE halves:
 *
 *   - If the group size m is even: split into two halves of m/2 coins
 *     each and weigh them against each other.
 *       - balanced -> no defective coin exists anywhere in this group
 *         (report NONE for this call).
 *       - left lighter  -> recurse into the left half only.
 *       - right lighter -> recurse into the right half only.
 *
 *   - If the group size m is odd (m >= 3): set ONE coin aside
 *     (unweighed), split the remaining (m-1) coins into two equal
 *     halves of (m-1)/2 each, and weigh them.
 *       - balanced -> both halves are genuine, so the only remaining
 *         suspect is the single set-aside coin. We settle this with
 *         ONE more weighing: the set-aside coin vs. any coin already
 *         known to be genuine (we just proved (m-1) of them are!).
 *       - left/right lighter -> recurse into that half (the set-aside
 *         coin is now known genuine, since the defective coin -- if
 *         it exists at all -- must be in the lighter half).
 *
 * Each full split roughly halves the candidate set, so the number of
 * weighings is about ceil(log2 n) plus a small constant c for the
 * odd-size verification step(s) -> O(log2 n + c) as required.
 *
 * Compile: gcc -O2 -Wall -o defective_coin defective_coin.c
 * Run:     ./defective_coin
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static long weighings = 0; /* counts number of balance-scale uses */

/* Weighs two EQUAL-SIZE groups of coins against each other.
   Returns -1 if group A (l1..r1) is lighter, +1 if group B (l2..r2)
   is lighter, 0 if balanced. Aborts if the groups are not equal size,
   since a real scale would never be used that way. */
int weighEqual(double weights[], int l1, int r1, int l2, int r2) {
    int sizeA = r1 - l1 + 1;
    int sizeB = r2 - l2 + 1;
    if (sizeA != sizeB) {
        fprintf(stderr, "Internal error: unequal pan sizes (%d vs %d)\n", sizeA, sizeB);
        exit(1);
    }
    weighings++;
    double sumA = 0, sumB = 0;
    for (int i = l1; i <= r1; i++) sumA += weights[i];
    for (int i = l2; i <= r2; i++) sumB += weights[i];

    if (sumA < sumB) return -1;
    if (sumB < sumA) return 1;
    return 0;
}

/* Recursively find the defective (lighter) coin's index within
   weights[lo..hi], given that IF a defective coin exists among the
   original n coins, it is guaranteed to lie in this range (otherwise
   none exists at all). Returns -1 if this range is proven all-genuine. */
int findDefective(double weights[], int lo, int hi) {
    int m = hi - lo + 1;
    if (m <= 0) return -1;

    if (m % 2 == 0) {
        /* even split: two equal halves, no leftover */
        int mid = lo + m / 2 - 1;
        int res = weighEqual(weights, lo, mid, mid + 1, hi);
        if (res == 0) return -1;                 /* balanced -> all genuine */
        if (res == -1) return findDefective(weights, lo, mid);
        return findDefective(weights, mid + 1, hi);
    }

    if (m == 1) {
        /* Should only be reached with n == 1 overall (degenerate,
           no reference coin exists to verify against). */
        return lo;
    }

    /* odd, m >= 3: set aside the last coin, weigh the rest in two
       equal halves of (m-1)/2 coins each */
    int leftover = hi;
    int newHi = hi - 1;
    int mid = lo + (newHi - lo + 1) / 2 - 1;
    int res = weighEqual(weights, lo, mid, mid + 1, newHi);

    if (res == -1) return findDefective(weights, lo, mid);
    if (res == 1)  return findDefective(weights, mid + 1, newHi);

    /* balanced: (m-1) coins in [lo..newHi] are proven genuine.
       Only the leftover coin remains a suspect - verify with ONE more
       weighing against a known-genuine coin (e.g. weights[lo]). */
    int refResult = weighEqual(weights, leftover, leftover, lo, lo);
    if (refResult == -1) return leftover;  /* leftover is lighter -> defective */
    return -1;                              /* equal -> leftover is genuine too */
}

void runTest(int n, int defectiveIndex /* -1 means no defective coin */) {
    if (n < 2) {
        printf("n = %6d skipped (need at least 2 coins to have a genuine reference)\n", n);
        return;
    }
    double *weights = malloc(sizeof(double) * n);
    const double STANDARD = 10.0;
    for (int i = 0; i < n; i++) weights[i] = STANDARD;
    if (defectiveIndex >= 0 && defectiveIndex < n) {
        weights[defectiveIndex] = STANDARD - 0.5; /* lighter coin */
    }

    weighings = 0;
    int found = findDefective(weights, 0, n - 1);

    printf("n = %7d | planted = %-4s | found = %-4s | weighings = %2ld",
           n,
           (defectiveIndex == -1 ? "none" : "idx"),
           (found == -1 ? "none" : "idx"),
           weighings);
    if (defectiveIndex != -1) printf("  (planted idx %d)", defectiveIndex);
    if (found != -1) printf("  (found idx %d)", found);
    printf("\n");

    if (found != defectiveIndex) {
        printf("   *** MISMATCH! Algorithm error.\n");
    }
    free(weights);
}

int main(void) {
    printf("=========================================================\n");
    printf(" Q2: Divide & Conquer - Find the Defective (Lighter) Coin\n");
    printf("=========================================================\n\n");

    srand(42);

    int sizes[] = {2, 3, 4, 5, 7, 8, 9, 16, 17, 33, 100, 101, 1000, 1000000};
    for (size_t i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++) {
        int n = sizes[i];
        int defIdx = rand() % n;   /* plant a defective coin */
        runTest(n, defIdx);
        runTest(n, -1);           /* also test "no defective" case */
    }

    printf("\nObservation: for n coins the algorithm uses about\n");
    printf("ceil(log2 n) weighings, with at most one extra weighing per\n");
    printf("odd-sized split to verify a leftover coin against a proven-\n");
    printf("genuine coin. This matches the required O(log2 n + c) bound.\n");
    return 0;
}
