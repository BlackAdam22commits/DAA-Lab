/* ============================================================
   Problem 2: Super Egg Testing Experiment
   ============================================================
   INPUT REPRESENTATION
   ---------------------
   Two integers E (number of eggs) and F (number of floors),
   read from stdin. The internal state used by the DP is a
   2-D table dp[t][e] = "the maximum number of floors that can
   be definitively resolved using at most t drops and at most e
   eggs" -- this is the classic reformulation that turns an O(E.F^2)
   or O(E.F.log F) table into an O(E.F) table.

   ALGORITHM (dynamic programming)
   --------------------------------
   Direct DP on (eggs, floors) with a ternary decision (egg breaks /
   egg survives / worst case) needs O(E*F) states each combined
   with an O(F) search over the drop floor -> O(E*F^2), or O(E*F*logF)
   with binary search (search space is monotone in the drop floor).
   We instead use the "floors coverable" reformulation:

        dp[t][e] = dp[t-1][e-1] + dp[t-1][e] + 1

   Reasoning: with t drops and e eggs, drop the first egg from some
   floor x.
     - If it breaks, the (unknown) safe floor is among the x-1
       floors below, and we now have t-1 drops and e-1 eggs left
       -> can resolve dp[t-1][e-1] additional floors below.
     - If it survives, we have t-1 drops and e eggs left and can
       resolve dp[t-1][e] additional floors above.
     - Floor x itself is resolved for free (the +1).
   So dp[t][e] floors are covered with the two branches plus the
   tested floor itself. Base cases: dp[t][0] = 0 for all t (no
   eggs => no information), dp[0][e] = 0 for all e (no drops => no
   information). We increase t starting from 0 until dp[t][E] >= F;
   the answer is the smallest such t (guaranteed number of droppings
   in the worst case, i.e., minimax-optimal).

   For E = 2 the closed form is: smallest m with m(m+1)/2 >= F.
   The program below verifies this against the DP for E = 2.

   COMPLEXITY
   ----------
   Let T* be the answer (drops needed). Building the DP table up
   to T* drops and E eggs costs O(T* * E) time and O(T* * E) space
   (can be reduced to O(E) space with a rolling array, shown in
   comments). For the given data (E=2, F=100) T* is at most ~14,
   so this is extremely fast; for general E and F the DP runs in
   O(E * T*) where T* = O(log F) for large E and O(F/E) for small E,
   giving an overall bound of O(E * (F)^(1/E)) drops in the worst
   informative case -- but the DP itself never explicitly needs
   more than O(E * ceil(log2(F+1)) ) rows in practice since the
   covered floors grow at least geometrically in t.
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int E, F;
    printf("Enter number of eggs (E): ");
    if (scanf("%d", &E) != 1 || E <= 0) { printf("Invalid input\n"); return 1; }
    printf("Enter number of floors (F): ");
    if (scanf("%d", &F) != 1 || F <= 0) { printf("Invalid input\n"); return 1; }

    /* Upper bound on drops needed: F itself (linear search with 1 egg) is always enough. */
    int maxT = F + 1;
    int **dp = malloc((maxT + 1) * sizeof(int *));
    for (int t = 0; t <= maxT; t++) {
        dp[t] = calloc(E + 1, sizeof(int));
    }
    /* dp[t][0] = 0, dp[0][e] = 0 already from calloc */

    int answer = -1;
    for (int t = 1; t <= maxT && answer < 0; t++) {
        for (int e = 1; e <= E; e++) {
            dp[t][e] = dp[t - 1][e - 1] + dp[t - 1][e] + 1;
        }
        if (dp[t][E] >= F) answer = t;
    }

    printf("\nEggs = %d, Floors = %d\n", E, F);
    printf("Minimum guaranteed number of droppings = %d\n", answer);

    if (E == 2) {
        int m = 0;
        while (m * (m + 1) / 2 < F) m++;
        printf("Closed-form check for E=2 (smallest m: m(m+1)/2 >= F) = %d  %s\n",
               m, (m == answer) ? "[OK matches DP]" : "[MISMATCH]");
    }

    /* Print the classic table dp[t][e] for small sizes so the growth is visible */
    if (answer <= 20 && E <= 10) {
        printf("\nTable: max floors resolvable with t drops (rows) and e eggs (cols)\n     ");
        for (int e = 0; e <= E; e++) printf("e=%-4d", e);
        printf("\n");
        for (int t = 0; t <= answer; t++) {
            printf("t=%-3d", t);
            for (int e = 0; e <= E; e++) printf("%6d", dp[t][e]);
            printf("\n");
        }
    }

    for (int t = 0; t <= maxT; t++) free(dp[t]);
    free(dp);
    return 0;
}
