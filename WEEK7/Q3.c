/* ============================================================
   Problem 3: Reve's Puzzle (Tower of Hanoi with 4 pegs)
   ============================================================
   INPUT REPRESENTATION
   ---------------------
   n = number of disks (read from stdin), pegs labelled 1..4.
   Disks are represented implicitly by size 1..n (1 = smallest);
   we don't need an explicit array because the recursive
   procedure only needs to know how many disks are in the group
   being moved and which three/four pegs are currently available.

   ALGORITHM: Frame-Stewart algorithm
   -------------------------------------
   With only 3 pegs, moving n disks needs 2^n - 1 moves (classical
   Tower of Hanoi). With a 4th peg we can do much better:

     FrameStewart(n, from, to, spare1, spare2):
        if n == 0: return
        if n == 1: move disk 1 from 'from' to 'to'; return
        choose a split point k (1 <= k < n)
        1) move the top k disks from 'from' to 'spare1'
           using ALL FOUR pegs           -> FrameStewart(k, from, spare1, spare2, to)
        2) move the remaining n-k disks from 'from' to 'to'
           using classic 3-peg Hanoi (spare1 is now blocked
           because it holds the k smaller disks)  -> Hanoi3(n-k, from, to, spare2)
        3) move the k disks from spare1 to 'to'
           using all four pegs again     -> FrameStewart(k, spare1, to, spare2, from)

   The number of moves for a chosen split is
        M(n,k) = 2*M(k) + (2^(n-k) - 1)
   and the Frame-Stewart conjecture (proven optimal for the
   "Reve's puzzle" case by Frame and Stewart, and verified
   computationally for reasonably small n) says we should pick k
   to MINIMIZE this quantity. The program below computes, via DP
   over all k, the optimal split for every disk count 1..n and
   the resulting move count M(n); for n = 8 this yields M(8) = 33,
   the value requested in the question.

   COMPLEXITY
   ----------
   - Computing the optimal move COUNT for all sizes 1..n via DP:
       M(i) = min over k=1..i-1 of ( 2*M(k) + 2^(i-k) - 1 ), M(1)=1
     This is O(n^2) time, O(n) space.
   - Actually GENERATING the move sequence is recursive and takes
     time proportional to the number of moves it prints, which is
     exponential-ish (roughly O(2^(n/2)) for the 4-peg case, far
     less than the 3-peg O(2^n), which is exactly the benefit of
     the extra peg).
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

long long moveCount = 0;

/* Classic 3-peg Hanoi. Moves the 'n' disks whose ACTUAL sizes are
   base+1 .. base+n (base+n is the largest / bottom disk of this
   sub-stack) from peg a to peg b, using peg c as the spare. The
   'base' offset lets sub-problems print the true disk size instead
   of a local 1..n index. */
void hanoi3(int n, int base, int a, int b, int c, int verbose) {
    if (n == 0) return;
    hanoi3(n - 1, base, a, c, b, verbose);
    moveCount++;
    if (verbose) printf("  Move disk %d: peg %d -> peg %d\n", base + n, a, b);
    hanoi3(n - 1, base, c, b, a, verbose);
}

int *bestK; /* bestK[i] = optimal split point for i disks */

/* Frame-Stewart recursive mover. Moves the 'n' disks whose actual
   sizes are base+1 .. base+n from peg a to peg b, using c and d as
   the two spare pegs (all four pegs available for the two "k-disk"
   sub-moves; only three pegs -- a,b,d -- for the middle n-k move
   because c is occupied holding the k smallest disks). */
void frameStewart(int n, int base, int a, int b, int c, int d, int verbose) {
    if (n == 0) return;
    if (n == 1) {
        moveCount++;
        if (verbose) printf("  Move disk %d: peg %d -> peg %d\n", base + 1, a, b);
        return;
    }
    int k = bestK[n];
    /* move the k smallest disks (sizes base+1..base+k): a -> c, all 4 pegs */
    frameStewart(k, base, a, c, d, b, verbose);
    /* move the remaining n-k disks (sizes base+k+1..base+n): a -> b, 3 pegs (d spare) */
    hanoi3(n - k, base + k, a, b, d, verbose);
    /* move the k smallest disks back: c -> b, all 4 pegs */
    frameStewart(k, base, c, b, d, a, verbose);
}

int main(void) {
    int n;
    printf("Enter number of disks (n): ");
    if (scanf("%d", &n) != 1 || n <= 0) { printf("Invalid input\n"); return 1; }

    long long *M = malloc((n + 1) * sizeof(long long));
    bestK = malloc((n + 1) * sizeof(int));
    M[0] = 0;
    if (n >= 1) M[1] = 1;
    bestK[1] = 0;

    for (int i = 2; i <= n; i++) {
        long long best = LLONG_MAX;
        int bk = 1;
        for (int k = 1; k < i; k++) {
            /* 2^(i-k) - 1 computed carefully to avoid overflow for the sizes we use */
            long long threePeg = (1LL << (i - k)) - 1;
            long long total = 2 * M[k] + threePeg;
            if (total < best) { best = total; bk = k; }
        }
        M[i] = best;
        bestK[i] = bk;
    }

    printf("\nOptimal move counts M(i) for i = 1..%d disks (4 pegs):\n", n);
    for (int i = 1; i <= n; i++)
        printf("  M(%2d) = %lld  (split k = %d)\n", i, M[i], bestK[i]);

    printf("\n>>> Answer for n = %d disks: %lld moves <<<\n", n, M[n]);
    if (n == 8) printf("    (matches the classical Reve's Puzzle result of 33 moves)\n");

    int ans;
    printf("\nPrint the full move sequence? (1 = yes, 0 = no): ");
    if (scanf("%d", &ans) == 1 && ans == 1) {
        moveCount = 0;
        printf("\nMove sequence (pegs 1=source, 2=destination, 3,4=spares):\n");
        frameStewart(n, 0, 1, 2, 3, 4, 1);
        printf("Total moves printed: %lld\n", moveCount);
    }

    free(M); free(bestK);
    return 0;
}
