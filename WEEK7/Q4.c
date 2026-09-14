/* ============================================================
   Problem 4: Security Switches
   ============================================================
   INPUT REPRESENTATION
   ---------------------
   n switches indexed 1..n, left to right (switch n = rightmost).
   State is an int array sw[1..n], 1 = ON, 0 = OFF. This is exactly
   the classical "Chinese Rings" (Baguenaudier) puzzle: switch n
   plays the role of the freely-manipulated ring, and the stated
   rule ("switch i may be toggled iff switch i+1 is ON and every
   switch to the right of i+1 is OFF") is precisely its legal-move
   rule. The state graph of this puzzle is a SIMPLE PATH (a proven
   property of the Gray-code / Chinese-rings puzzle): from every
   state there are at most two legal moves, one that decreases the
   distance to the goal and one that increases it (i.e. undoes the
   previous move). Consequently a minimal solution is obtained by
   a straightforward greedy walk that NEVER immediately undoes its
   previous move.

   ALGORITHM
   ---------
   1. Start with all switches ON.
   2. Repeat until all switches are OFF:
        a. Find every switch index i (1<=i<=n) that satisfies the
           legality rule in the CURRENT state -- there will be 1
           or 2 such indices.
        b. Discard the option equal to the switch just toggled
           (never immediately undo the previous move).
        c. Among what remains, break ties by the parity of n:
              - if n is EVEN, pick the SMALLEST legal index
              - if n is ODD,  pick the LARGEST  legal index
           (this tie-break was determined by exhaustive testing
           against a BFS-verified optimum for n = 1..11 and always
           reproduces a shortest solution -- see accompanying
           report for the verification data.)
        d. Toggle that switch.
   Because the state graph is a path, this "don't backtrack" walk
   can only move monotonically from the all-ON end to the all-OFF
   end, so it is automatically minimal.

   The MINIMUM MOVE COUNT itself satisfies the recurrence
        f(1) = 1,   f(n) = 2*f(n-1) + (n mod 2)      for n > 1
   which solves in closed form to
        f(n) = floor( 2^(n+1) / 3 )        (OEIS A000975)
   e.g. f(8) = floor(512/3) = 170. (Both the recurrence and the
   closed form were checked against an exhaustive BFS shortest-path
   search for n = 1..8 -- see accompanying report.)

   COMPLEXITY
   ----------
   - Closed-form / recurrence count: O(n) time, O(n) space (O(1)
     extra with the power-of-two formula for moderate n).
   - Simulating and printing the actual move sequence: each of the
     f(n) toggles requires an O(n) legality scan in the naive
     implementation below, giving O(n * f(n)) = O(n * 2^n) time.
     (This can be reduced to O(f(n)) total with an O(1)-amortized
     "next legal switch" pointer, but the puzzle sizes of interest
     here are small enough that the simple version is preferred
     for clarity.)
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>

int n;
int *sw;
long long moveCount = 0;

static int isLegal(int i) {
    if (i == n) return 1;
    if (sw[i + 1] != 1) return 0;
    for (int j = i + 2; j <= n; j++) if (sw[j] != 0) return 0;
    return 1;
}

static long long f(int m) {           /* closed form floor(2^(m+1)/3) */
    if (m <= 0) return 0;
    return (1LL << (m + 1)) / 3;
}

int main(void) {
    printf("Enter number of switches (n): ");
    if (scanf("%d", &n) != 1 || n <= 0) { printf("Invalid input\n"); return 1; }
    sw = malloc((n + 1) * sizeof(int));
    for (int i = 1; i <= n; i++) sw[i] = 1;   /* all ON initially */

    printf("\nRecurrence: f(1)=1, f(m)=2*f(m-1)+(m mod 2) for m>1\n");
    long long *fr = malloc((n + 1) * sizeof(long long));
    fr[0] = 0; if (n >= 1) fr[1] = 1;
    for (int m = 2; m <= n; m++) fr[m] = 2 * fr[m - 1] + (m % 2);
    for (int m = 1; m <= n; m++)
        printf("  f(%2d) = %-6lld   closed-form floor(2^(%d+1)/3) = %-6lld  %s\n",
               m, fr[m], m, f(m), (fr[m] == f(m)) ? "[OK]" : "[MISMATCH]");

    int verbose;
    printf("\nPrint the full move sequence for n=%d? (1=yes,0=no): ", n);
    if (scanf("%d", &verbose) != 1) verbose = 0;

    int last = -1;
    int preferMax = (n % 2 == 1);   /* odd n -> prefer largest legal index; even n -> smallest */
    int allOff = 0;

    if (verbose) printf("\nMove sequence (toggling switch i means flip its state):\n");
    while (!allOff) {
        int legals[2], cnt = 0;
        for (int i = 1; i <= n && cnt < 2; i++) {
            if (isLegal(i)) legals[cnt++] = i;
        }
        int candidates[2], ncand = 0;
        for (int k = 0; k < cnt; k++) if (legals[k] != last) candidates[ncand++] = legals[k];
        if (ncand == 0) { candidates[0] = legals[0]; ncand = 1; }

        int chosen = candidates[0];
        for (int k = 1; k < ncand; k++) {
            if (preferMax) { if (candidates[k] > chosen) chosen = candidates[k]; }
            else           { if (candidates[k] < chosen) chosen = candidates[k]; }
        }

        sw[chosen] ^= 1;
        moveCount++;
        last = chosen;
        if (verbose) {
            printf("  [%3lld] toggle switch %2d -> %s   state: ", moveCount, chosen, sw[chosen] ? "ON" : "OFF");
            for (int i = 1; i <= n; i++) printf("%d", sw[i]);
            printf("\n");
        }

        allOff = 1;
        for (int i = 1; i <= n; i++) if (sw[i]) { allOff = 0; break; }
        if (moveCount > (1LL << (n + 2))) { printf("Safety abort (unexpected loop)\n"); break; }
    }

    printf("\nAll switches OFF. Total moves used: %lld  (expected minimum: %lld)  %s\n",
           moveCount, fr[n], (moveCount == fr[n]) ? "[OK - optimal]" : "[MISMATCH]");

    free(sw); free(fr);
    return 0;
}
