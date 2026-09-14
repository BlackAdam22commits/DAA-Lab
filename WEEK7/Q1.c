/* ============================================================
   Problem 1: Invert the Coin-Triangle
   ============================================================
   INPUT REPRESENTATION
   ---------------------
   A triangle with n rows (row i = 0..n-1 has i+1 coins) is packed
   on a triangular lattice. Every coin position is represented by
   AXIAL (hex-grid) coordinates (col, row):
        row i (0-indexed from the apex) contains points
        (col, row) for col = 0 .. i
   This is the natural, compact representation for a triangular
   lattice (2 integers per coin instead of real-valued x,y).

   ALGORITHM (overlap / superposition method)
   --------------------------------------------
   Inverting the triangle = rotating the whole point-set by 180
   degrees. On a triangular lattice, rotating a lattice by 180
   degrees about a lattice point maps the lattice onto itself, so
   180-degree rotation of the coordinates is simply
                (col, row) -> (-col, -row)
   Superimpose the rotated (target) triangle on the original
   triangle at every integer translation (dc, dr) and compute the
   size of the intersection (coins that would already be in a
   correct final spot and therefore never need to move).
   The minimum number of moves equals
                moves = N - max_overlap
   where N = n(n+1)/2 is the total number of coins, because every
   coin outside the best-aligned overlap must be picked up and
   every empty slot outside the overlap (in the target shape) is
   exactly filled by one such coin (a straightforward bijection
   exists since |source \ overlap| = |target \ overlap|).
   This overlap strategy is known to be optimal for this puzzle
   (Levitin, "Algorithmic Puzzles").

   CLOSED-FORM FORMULA (derived/verified by brute-force search,
   see accompanying report):
                moves(n) = floor( n(n+1) / 6 )
   e.g. n = 4 (10 pennies) -> floor(20/6) = 3, the well known
   answer to the classic "10-coin triangle" puzzle.

   COMPLEXITY
   ----------
   Brute-force overlap search over shifts (dc,dr) in [-2n,2n]^2 and
   coordinates stored in a hash/array: O(n^2) coins, O(n^2) shifts
   tried, each overlap test O(n^2) worst case with a hash set ->
   O(n^4) for the constructive/verifying program below (fine for
   the small values of n used to actually flip a real triangle of
   coins).  The closed-form formula gives the count in O(1).
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { int c, r; } Point;

static int cmpPoint(const void *a, const void *b) {
    const Point *p = a, *q = b;
    if (p->c != q->c) return p->c - q->c;
    return p->r - q->r;
}

/* Build the list of N=n(n+1)/2 coin coordinates for an n-row triangle */
static int buildTriangle(int n, Point *pts) {
    int k = 0;
    for (int row = 0; row < n; row++)
        for (int col = 0; col <= row; col++) {
            pts[k].c = col; pts[k].r = row; k++;
        }
    return k;
}

/* Is point p present in a sorted array of m points? (binary search) */
static int contains(Point *arr, int m, Point p) {
    int lo = 0, hi = m - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        int cmp = (arr[mid].c != p.c) ? (arr[mid].c - p.c) : (arr[mid].r - p.r);
        if (cmp == 0) return 1;
        else if (cmp < 0) lo = mid + 1;
        else hi = mid - 1;
    }
    return 0;
}

int main(void) {
    int n;
    printf("Enter number of rows (coins per side) of the triangle: ");
    if (scanf("%d", &n) != 1 || n <= 0) { printf("Invalid input.\n"); return 1; }

    int N = n * (n + 1) / 2;
    Point *orig = malloc(N * sizeof(Point));
    Point *rotated = malloc(N * sizeof(Point));
    buildTriangle(n, orig);

    /* rotated (un-shifted) target coordinates: (col,row) -> (-col,-row) */
    for (int i = 0; i < N; i++) { rotated[i].c = -orig[i].c; rotated[i].r = -orig[i].r; }

    Point *sortedOrig = malloc(N * sizeof(Point));
    memcpy(sortedOrig, orig, N * sizeof(Point));
    qsort(sortedOrig, N, sizeof(Point), cmpPoint);

    int bestOverlap = -1, bestDc = 0, bestDr = 0;
    int range = 2 * n + 2;
    Point *shifted = malloc(N * sizeof(Point));

    for (int dc = -range; dc <= range; dc++) {
        for (int dr = -range; dr <= range; dr++) {
            int overlap = 0;
            for (int i = 0; i < N; i++) {
                Point p = { rotated[i].c + dc, rotated[i].r + dr };
                if (contains(sortedOrig, N, p)) overlap++;
            }
            if (overlap > bestOverlap) {
                bestOverlap = overlap; bestDc = dc; bestDr = dr;
                for (int i = 0; i < N; i++) shifted[i] = (Point){ rotated[i].c + dc, rotated[i].r + dr };
            }
        }
    }

    int moves = N - bestOverlap;
    int formula = (n * (n + 1)) / 6;   /* floor division, integers */

    printf("\nTriangle rows (n)              : %d\n", n);
    printf("Total coins (N = n(n+1)/2)      : %d\n", N);
    printf("Best overlap found by search    : %d  (shift dc=%d, dr=%d)\n", bestOverlap, bestDc, bestDr);
    printf("Minimum moves (search)          : %d\n", moves);
    printf("Minimum moves (closed formula)   : floor(n(n+1)/6) = %d\n", formula);
    printf("%s\n", moves == formula ? "[OK] search matches closed-form formula" :
                                       "[MISMATCH] increase search range");

    /* Print the actual list of coins that must move, and where to */
    Point *stillNeeded = malloc(N * sizeof(Point));  /* target coins not yet covered */
    int needCount = 0;
    Point *sortedOrig2 = sortedOrig; /* already sorted copy of original */
    for (int i = 0; i < N; i++)
        if (!contains(sortedOrig2, N, shifted[i])) stillNeeded[needCount++] = shifted[i];

    printf("\nMove list (%d moves): coin (col,row) -> new (col,row)\n", moves);
    Point *sortedShifted = malloc(N * sizeof(Point));
    memcpy(sortedShifted, shifted, N * sizeof(Point));
    qsort(sortedShifted, N, sizeof(Point), cmpPoint);
    int m = 0;
    for (int i = 0; i < N && m < needCount; i++) {
        if (!contains(sortedShifted, N, orig[i])) {
            printf("  (%d,%d) -> (%d,%d)\n", orig[i].c, orig[i].r, stillNeeded[m].c, stillNeeded[m].r);
            m++;
        }
    }

    free(orig); free(rotated); free(sortedOrig); free(shifted);
    free(stillNeeded); free(sortedShifted);
    return 0;
}
