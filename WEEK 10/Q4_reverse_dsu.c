/*
 * Q4 - Reverse Time-Step DSU Application  (DAA Lab-10)
 *
 * Representation : CSR adjacency list (offset[] + nbr[]) built once from the edge
 *                  list in O(V+E); vertex values val[]; deletion order P[].
 * Idea           : DSU supports MERGE but not SPLIT, and deleting a vertex splits
 *                  components.  So run time BACKWARDS: start with the empty graph
 *                  (all vertices deleted) and RE-INSERT vertices in the order
 *                  p_V, p_{V-1}, ..., p_2.  Inserting v = make-set + union with each
 *                  already-active neighbour.  Keep best = max component sum.
 *                    ans[V]   = 0  (nothing left)
 *                    ans[i-1] = best after re-inserting p_V .. p_i     (i = V..2)
 *                  where ans[i] = answer after deleting p_1..p_i.
 *                  Values are assumed NON-NEGATIVE: a merge then never lowers
 *                  the maximum, so a running max suffices.
 *
 * Complexity     : each vertex inserted once, each edge examined twice (once from
 *                  each endpoint)  =>  O((V + E) * alpha(V)) time, O(V + E) space.
 *                  (Recomputing components after every deletion: O(V*(V+E)).)
 *
 * Usage          : ./q4           -> built-in tests + random brute-force check
 *                  ./q4 -i < in   -> input: V E / v1..vV / E lines "u v" / P (V numbers)
 *                                    all 1-indexed; prints V answers
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long long ll;

static int *dp, *dsz; static ll *dsum;
static int find(int x) { while (dp[x] != x) { dp[x] = dp[dp[x]]; x = dp[x]; } return x; }
static void unite(int a, int b) {
    a = find(a); b = find(b); if (a == b) return;
    if (dsz[a] < dsz[b]) { int t = a; a = b; b = t; }
    dp[b] = a; dsz[a] += dsz[b]; dsum[a] += dsum[b];
}

/* ans[i] (0-indexed) = max component sum after deleting P[0..i]; ans has V slots */
static void solve(int V, int E, const ll *val, int (*ed)[2], const int *P, ll *ans) {
    int *off = calloc(V + 2, sizeof(int)), *nbr = malloc(2 * (size_t)E * sizeof(int) + 1);
    for (int i = 0; i < E; i++) { off[ed[i][0] + 1]++; off[ed[i][1] + 1]++; }
    for (int i = 0; i < V; i++) off[i + 1] += off[i];
    int *pos = malloc((V + 1) * sizeof(int)); memcpy(pos, off, (V + 1) * sizeof(int));
    for (int i = 0; i < E; i++) { nbr[pos[ed[i][0]]++] = ed[i][1]; nbr[pos[ed[i][1]]++] = ed[i][0]; }

    dp = malloc(V * sizeof(int)); dsz = malloc(V * sizeof(int)); dsum = malloc(V * sizeof(ll));
    char *active = calloc(V, 1);
    ll best = 0;
    ans[V - 1] = 0;
    for (int i = V - 1; i >= 1; i--) {
        int v = P[i];
        active[v] = 1; dp[v] = v; dsz[v] = 1; dsum[v] = val[v];
        for (int k = off[v]; k < off[v + 1]; k++)
            if (active[nbr[k]]) unite(v, nbr[k]);     /* self-loops: find(v)==find(v) */
        if (dsum[find(v)] > best) best = dsum[find(v)];
        ans[i - 1] = best;
    }
    free(off); free(nbr); free(pos); free(dp); free(dsz); free(dsum); free(active);
}

/* ---------------- brute force: rebuild components after each deletion ----- */
static void brute(int V, int E, const ll *val, int (*ed)[2], const int *P, ll *ans) {
    char gone[16]; memset(gone, 0, sizeof gone);
    for (int t = 0; t < V; t++) {
        gone[P[t]] = 1;
        int lab[16]; for (int i = 0; i < V; i++) lab[i] = i;
        for (int ch = 1; ch;) { ch = 0;
            for (int i = 0; i < E; i++) { int a = ed[i][0], b = ed[i][1];
                if (gone[a] || gone[b] || lab[a] == lab[b]) continue;
                int lo = lab[a] < lab[b] ? lab[a] : lab[b]; lab[a] = lab[b] = lo; ch = 1; } }
        ll best = 0;
        for (int c = 0; c < V; c++) { ll sm = 0;
            for (int i = 0; i < V; i++) if (!gone[i] && lab[i] == c) sm += val[i];
            if (sm > best) best = sm; }
        ans[t] = best;
    }
}

static int tests = 0, fails = 0;
#define CHECK(c) do { tests++; if (!(c)) { fails++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)

static void self_tests(void) {
    /* path 1-2-3-4-5 (0..4), values 1..5, delete the middle first, then 0,1,3,4 */
    ll val[] = {1, 2, 3, 4, 5}; int ed[][2] = {{0,1},{1,2},{2,3},{3,4}};
    int P[] = {2, 0, 1, 3, 4}; ll exp[] = {9, 9, 9, 5, 0}, ans[5];
    solve(5, 4, val, ed, P, ans);
    for (int i = 0; i < 5; i++) CHECK(ans[i] == exp[i]);
    /* V = 1 */
    ll v1[] = {7}; int P1[] = {0}; ll a1[1]; solve(1, 0, v1, ed, P1, a1); CHECK(a1[0] == 0);
    /* no edges: answer = max remaining single value */
    ll v3[] = {5, 9, 2}; int P3[] = {1, 0, 2}; ll a3[3]; solve(3, 0, v3, ed, P3, a3);
    CHECK(a3[0] == 5 && a3[1] == 2 && a3[2] == 0);

    srand(4242);
    for (int it = 0; it < 4000; it++) {
        int V = 1 + rand() % 9, E = rand() % 15; ll vv[16]; int e[16][2], perm[16];
        for (int i = 0; i < V; i++) { vv[i] = rand() % 20; perm[i] = i; }
        for (int i = 0; i < E; i++) { e[i][0] = rand() % V; e[i][1] = rand() % V; }
        for (int i = V - 1; i > 0; i--) { int j = rand() % (i + 1), t = perm[i]; perm[i] = perm[j]; perm[j] = t; }
        ll a[16], b[16]; solve(V, E, vv, e, perm, a); brute(V, E, vv, e, perm, b);
        for (int i = 0; i < V; i++) CHECK(a[i] == b[i]);
    }
}

int main(int argc, char **argv) {
    if (argc > 1 && !strcmp(argv[1], "-i")) {
        int V, E; if (scanf("%d %d", &V, &E) != 2) return 1;
        ll *val = malloc(V * sizeof(ll)); int (*ed)[2] = malloc((E + 1) * sizeof *ed);
        int *P = malloc(V * sizeof(int)); ll *ans = malloc(V * sizeof(ll));
        for (int i = 0; i < V; i++) if (scanf("%lld", &val[i]) != 1) return 1;
        for (int i = 0; i < E; i++) { if (scanf("%d %d", &ed[i][0], &ed[i][1]) != 2) return 1; ed[i][0]--; ed[i][1]--; }
        for (int i = 0; i < V; i++) { if (scanf("%d", &P[i]) != 1) return 1; P[i]--; }
        solve(V, E, val, ed, P, ans);
        for (int i = 0; i < V; i++) printf("%lld%c", ans[i], i + 1 < V ? ' ' : '\n');
        return 0;
    }
    self_tests();
    printf("Q4: %d checks, %d failed -> %s\n", tests, fails, fails ? "FAIL" : "ALL PASS");
    return fails != 0;
}
