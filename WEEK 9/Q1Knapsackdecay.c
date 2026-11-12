/*
 * Q1 - Fractional Knapsack with Deterioration Rate
 *
 * MODEL (stated explicitly, because "time" is open in the question):
 *   Items are consumed back-to-back at unit rate, so a chunk that starts after
 *   S units of weight were consumed is consumed at time t = S.  Item i has
 *   density d_i = v_i/w_i, decaying to d_i - lambda_i*t.  Consuming x units of
 *   item i starting at time S earns the integral
 *        gain(i,S,x) = x*d_i - lambda_i*(x*S + x^2/2).
 *   Total consumed weight <= W; x_i <= w_i.
 *
 * KEY FACTS
 *  (a) NAIVE "pick the best current density" GREEDY IS NOT OPTIMAL.
 *      Counter-example: A(d=10,lam=4,w=1), B(d=9,lam=0,w=1), W=2.
 *      Instantaneous greedy gives 14.0 (A, then B, then A again);
 *      A-then-B gives 17.0.
 *  (b) For any fixed amounts x_i, consuming items in NON-INCREASING lambda
 *      order is optimal (exchange argument: total decay loss is
 *      sum lambda_i * x_i * (mean consumption time of i); swapping adjacent
 *      blocks with lambda_a < lambda_b so that a comes first changes the loss
 *      by x_a*x_b*(lambda_a - lambda_b) < 0 -> loss drops when larger lambda
 *      goes first).
 *  (c) So: sort by lambda desc (greedy part), then choose the amounts by a
 *      DP over (item index, weight consumed so far).  This is exact when
 *      weights/W are integers (amounts are chosen in whole units).
 *
 * INPUT REPRESENTATION: arrays of (v_i, w_i, lambda_i), integer w_i and W.
 *
 * COMPLEXITY: sort O(n log n); DP has n*W states, each tries <= w_i+1 amounts
 *   -> O(n * W * w_max) time, O(n*W) space (pseudo-polynomial).
 *
 * VALIDATION: exhaustive search over all permutations x all integer amount
 *   vectors for n <= 4; also compared against the naive greedy to show it fails.
 * Build: gcc -O2 -Wall -o q1 Q1_knapsack_decay.c -lm
 * Run:   ./q1            (demo + randomized validation)
 *        ./q1 stdin      (input: n W, then n lines "v w lambda")
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define MAXN 16
#define MAXW 128
typedef struct { double v, lam; int w, id; } Item;
static double gain(const Item *a, double S, int x) { return x * (a->v / a->w) - a->lam * (x * S + x * (double)x / 2.0); }
static int cmp(const void *p, const void *q) { const Item *a = p, *b = q; return (a->lam < b->lam) - (a->lam > b->lam); }

/* returns optimal value; amt[] (indexed by sorted position) and order[] filled */
static double solve(Item *it, int n, int W, int *amt) {
    qsort(it, n, sizeof(Item), cmp);                    /* lambda non-increasing */
    static double dp[MAXN + 1][MAXW + 1]; static int pick[MAXN + 1][MAXW + 1];
    for (int s = 0; s <= W; s++) dp[0][s] = (s == 0) ? 0 : -1e18;
    for (int i = 1; i <= n; i++)
        for (int s = 0; s <= W; s++) {
            dp[i][s] = -1e18; pick[i][s] = 0;
            for (int x = 0; x <= it[i - 1].w && x <= s; x++) {
                double prev = dp[i - 1][s - x]; if (prev < -1e17) continue;
                double v = prev + gain(&it[i - 1], s - x, x);
                if (v > dp[i][s]) { dp[i][s] = v; pick[i][s] = x; }
            }
        }
    int bs = 0; for (int s = 0; s <= W; s++) if (dp[n][s] > dp[n][bs]) bs = s;
    double ans = dp[n][bs];
    for (int i = n, s = bs; i >= 1; i--) { amt[i - 1] = pick[i][s]; s -= pick[i][s]; }
    return ans;
}
/* naive instantaneous-density greedy (to demonstrate it is not optimal) */
static double naive(const Item *it, int n, double W) {
    double rem[MAXN], T = 0, val = 0; for (int i = 0; i < n; i++) rem[i] = it[i].w;
    const double dt = 1e-3;
    while (T < W - 1e-12) {
        int b = -1; double bd = 0;
        for (int i = 0; i < n; i++) if (rem[i] > 1e-12) { double d = it[i].v / it[i].w - it[i].lam * T; if (d > bd) { bd = d; b = i; } }
        if (b < 0) break;
        double x = dt < rem[b] ? dt : rem[b]; if (x > W - T) x = W - T;
        val += x * (bd - it[b].lam * x / 2); rem[b] -= x; T += x;
    }
    return val;
}
static double brute_best; static int bn, bW; static Item bit[MAXN];
static void rec_amt(int *p, int k, int *x, int used, double S, double val) {
    if (k == bn) { if (val > brute_best) brute_best = val; return; }
    for (int a = 0; a <= bit[p[k]].w && used + a <= bW; a++) rec_amt(p, k + 1, x, used + a, S + a, val + gain(&bit[p[k]], S, a));
}
static void rec_perm(int *p, int k) {
    if (k == bn) { int x[MAXN]; rec_amt(p, 0, x, 0, 0, 0); return; }
    for (int i = k; i < bn; i++) { int t = p[k]; p[k] = p[i]; p[i] = t; rec_perm(p, k + 1); t = p[k]; p[k] = p[i]; p[i] = t; }
}
int main(int argc, char **argv) {
    Item it[MAXN]; int amt[MAXN];
    if (argc > 1 && !strcmp(argv[1], "stdin")) {
        int n, W; if (scanf("%d %d", &n, &W) != 2 || n > MAXN || W > MAXW) return 1;
        for (int i = 0; i < n; i++) { double v; int w; if (scanf("%lf %d %lf", &v, &w, &it[i].lam) != 3) return 1; it[i].v = v; it[i].w = w; it[i].id = i + 1; }
        double r = solve(it, n, W, amt);
        printf("Optimal value = %.4f\nSchedule (consumption order):\n", r);
        for (int i = 0; i < n; i++) if (amt[i]) printf("  item %d : %d of %d units (fraction %.3f)\n", it[i].id, amt[i], it[i].w, (double)amt[i] / it[i].w);
        return 0;
    }
    /* counter-example from the header */
    Item ce[2] = {{10, 4, 1, 1}, {9, 0, 1, 2}};
    Item ce2[2] = {{10, 4, 1, 1}, {9, 0, 1, 2}};
    double r = solve(ce, 2, 2, amt);
    printf("Counter-example: naive greedy = %.3f, optimal (lambda-desc + DP) = %.3f\n", naive(ce2, 2, 2), r);
    srand(11); int bad = 0, T = 1500, naive_bad = 0;
    for (int tc = 0; tc < T; tc++) {
        int n = 1 + rand() % 4, W = 1 + rand() % 8; Item a[MAXN];
        for (int i = 0; i < n; i++) { a[i].w = 1 + rand() % 4; a[i].v = a[i].w * (1 + rand() % 12); a[i].lam = (rand() % 50) / 10.0; a[i].id = i; }
        memcpy(bit, a, sizeof a); bn = n; bW = W; brute_best = 0;
        int p[MAXN]; for (int i = 0; i < n; i++) p[i] = i; rec_perm(p, 0);
        double nv = naive(a, n, W);
        double got = solve(a, n, W, amt);
        if (fabs(got - brute_best) > 1e-6) { if (bad < 3) printf("MISMATCH dp=%.4f brute=%.4f\n", got, brute_best); bad++; }
        if (nv < brute_best - 0.05) naive_bad++;
    }
    printf("Validation vs exhaustive search: %d/%d mismatches\n", bad, T);
    printf("(naive instantaneous greedy was suboptimal in %d/%d cases)\n", naive_bad, T);
    return 0;
}
