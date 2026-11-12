/*
 * Q9 - Optimal alphabetic binary tree (Hu-Tucker problem), via Garsia-Wachs
 *
 * Problem: leaves must stay in the order 1..n; minimise sum w_i*depth(i).
 * (Hu-Tucker and Garsia-Wachs give the same optimal cost; Garsia-Wachs is the
 *  simpler greedy to simulate.)
 *
 * INPUT REPRESENTATION: sequence a[] of tree nodes (weight, children) with
 *   sentinel nodes of weight +INF at both ends; nodes live in a pool so the
 *   merge tree can be inspected afterwards.
 *
 * ALGORITHM
 *   Phase 1 (greedy): find the smallest k with a[k-1] <= a[k+1]; merge a[k-1],a[k]
 *     into a node of weight s = a[k-1]+a[k]; remove them and re-insert s directly
 *     after the nearest element to the left (before k-1) whose weight is >= s.
 *     Repeat until one node remains.  Cost = sum of all merge weights.
 *   Phase 2: leaf depths of the phase-1 tree are the depths of an optimal
 *     alphabetic tree (the phase-1 tree itself is NOT alphabetic).
 *   Phase 3: rebuild the alphabetic tree from the depth sequence with a stack
 *     (push leaf depth; while the two top entries have equal depth d, replace by
 *     d-1) - success proves the depth sequence is realisable in order.
 *
 * COMPLEXITY: this simulation is O(n^2) time (each of n-1 merges scans/shifts the
 *   list), O(n) space.  With a balanced search structure (Hu-Tucker / Klawe-Mumey)
 *   the same algorithm runs in O(n log n).
 *
 * VALIDATION: classic O(n^3) interval DP
 *   C[i][j] = min_k C[i][k]+C[k+1][j] + W(i..j), C[i][i]=0; compare optimal costs,
 *   and check the stack rebuild succeeds with sum w*depth == cost.
 * Build: gcc -O2 -Wall -o q9 Q9_hu_tucker.c
 * Run: ./q9 | ./q9 stdin  (input: n then n weights)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXN 512
#define INF (1LL << 60)
typedef long long ll;
static ll W[2 * MAXN]; static int L[2 * MAXN], R[2 * MAXN], np;

static ll garsia_wachs(const ll *w, int n, int *dep) {
    np = 0; static int a[2 * MAXN]; int m = 0;
    W[np] = INF; L[np] = R[np] = -1; a[m++] = np++;
    for (int i = 0; i < n; i++) { W[np] = w[i]; L[np] = R[np] = -1; a[m++] = np++; }
    W[np] = INF; L[np] = R[np] = -1; a[m++] = np++;
    ll cost = 0;
    while (m > 3) {
        int k = 1; while (!(W[a[k - 1]] <= W[a[k + 1]])) k++;
        int x = a[k - 1], y = a[k]; ll s = W[x] + W[y]; cost += s;
        W[np] = s; L[np] = x; R[np] = y; int z = np++;
        for (int i = k + 1; i < m; i++) a[i - 2] = a[i]; m -= 2;      /* remove x,y */
        int j = k - 2; while (W[a[j]] < s) j--;                          /* nearest left >= s */
        for (int i = m; i > j + 1; i--) a[i] = a[i - 1]; a[j + 1] = z; m++;
    }
    /* depths of leaves in the phase-1 tree rooted at a[1] */
    int st[2 * MAXN], sd[2 * MAXN], sp = 0; st[sp] = a[1]; sd[sp++] = 0;
    while (sp) { int u = st[--sp], d = sd[sp]; if (L[u] < 0) dep[u - 1] = d; else { st[sp] = L[u]; sd[sp++] = d + 1; st[sp] = R[u]; sd[sp++] = d + 1; } }
    return cost;
}
static int rebuild(const int *dep, int n) {            /* stack check, in leaf order */
    int st[MAXN], sp = 0;
    for (int i = 0; i < n; i++) { st[sp++] = dep[i]; while (sp >= 2 && st[sp - 1] == st[sp - 2]) { sp--; st[sp - 1]--; } }
    return sp == 1 && st[0] == 0;
}
static ll dp_cost(const ll *w, int n) {
    static ll C[MAXN][MAXN], P[MAXN + 1]; P[0] = 0; for (int i = 0; i < n; i++) P[i + 1] = P[i] + w[i];
    for (int i = 0; i < n; i++) C[i][i] = 0;
    for (int len = 2; len <= n; len++) for (int i = 0; i + len <= n; i++) { int j = i + len - 1; ll b = INF;
        for (int k = i; k < j; k++) { ll c = C[i][k] + C[k + 1][j]; if (c < b) b = c; } C[i][j] = b + P[j + 1] - P[i]; }
    return n > 1 ? C[0][n - 1] : 0;
}
int main(int argc, char **argv) {
    ll w[MAXN]; int n, dep[MAXN];
    if (argc > 1 && !strcmp(argv[1], "stdin")) { if (scanf("%d", &n) != 1) return 1; for (int i = 0; i < n; i++) if (scanf("%lld", &w[i]) != 1) return 1; }
    else { n = 6; ll t[] = {7, 2, 1, 3, 9, 4}; memcpy(w, t, sizeof t); }
    if (n == 1) { printf("Optimal cost = 0\n"); return 0; }
    ll c = garsia_wachs(w, n, dep);
    printf("Optimal cost = %lld\nLeaf depths (in order):", c); for (int i = 0; i < n; i++) printf(" %d", dep[i]); printf("\n");
    ll chk = 0; for (int i = 0; i < n; i++) chk += w[i] * dep[i];
    printf("sum w*depth = %lld, alphabetic rebuild %s\n", chk, rebuild(dep, n) ? "OK" : "FAILED");
    if (argc > 1) return 0;
    srand(12); int bad = 0, T = 5000;
    for (int tc = 0; tc < T; tc++) { int m = 2 + rand() % 14; ll b[MAXN]; int d[MAXN]; for (int i = 0; i < m; i++) b[i] = 1 + rand() % 30;
        ll g = garsia_wachs(b, m, d), s = 0; for (int i = 0; i < m; i++) s += b[i] * d[i];
        if (g != dp_cost(b, m) || s != g || !rebuild(d, m)) bad++; }
    printf("Validation vs O(n^3) interval DP: %d/%d mismatches\n", bad, T);
    return 0;
}
