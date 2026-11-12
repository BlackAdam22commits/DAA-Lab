/*
 * Q5 - Lexicographically Smallest Connected-Component Permutation  (DAA Lab-10)
 *
 * Interpretation : a swap (u,v) exchanges the entries at POSITIONS u and v of A.
 * Representation : edge list fed straight into a DSU; afterwards a CSR bucket
 *                  structure groups positions by component root.
 * Key fact       : positions in one connected component can be rearranged into
 *                  ANY order (a connected graph's swaps generate every
 *                  permutation of its vertices), and components are independent.
 *                  So inside each component the smallest result puts the
 *                  component's values in increasing order on its positions
 *                  (positions taken in increasing order).
 * Linear trick   : A is a permutation of 1..n, so no comparison sort is needed:
 *                  scan values v = 1..n (ascending); v lives at position pos[v];
 *                  give v to the next unused (smallest) position of that
 *                  position's component.  Positions of a component are bucketed in
 *                  increasing order by scanning i = 0..n-1 once.
 *
 * Complexity     : DSU unions O(m*alpha(n)); bucketing O(n); value scan O(n*alpha(n))
 *                  => O(n + m*alpha(n)) time, O(n) space.
 *                  (Comparison-sort per component would be O(n log n).)
 *
 * Usage          : ./q5           -> built-in tests + BFS brute force over all reachable states
 *                  ./q5 -i < in   -> input: n / A1..An / m / m lines "u v" (1-indexed positions)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int *p, *sz;
static int find(int x) { while (p[x] != x) { p[x] = p[p[x]]; x = p[x]; } return x; }
static void unite(int a, int b) {
    a = find(a); b = find(b); if (a == b) return;
    if (sz[a] < sz[b]) { int t = a; a = b; b = t; }
    p[b] = a; sz[a] += sz[b];
}

/* A: values 1..n (modified in place into the answer); edges 0-indexed positions */
static void solve(int n, int *A, int m, int (*ed)[2]) {
    p = malloc(n * sizeof(int)); sz = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) { p[i] = i; sz[i] = 1; }
    for (int i = 0; i < m; i++) unite(ed[i][0], ed[i][1]);

    int *start = calloc(n + 1, sizeof(int)), *slot = malloc(n * sizeof(int));
    int *bucket = malloc(n * sizeof(int)), *fill = malloc(n * sizeof(int));
    int *pos = malloc((n + 1) * sizeof(int)), *out = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) { start[find(i) + 1]++; pos[A[i]] = i; }
    for (int r = 0; r < n; r++) start[r + 1] += start[r];
    memcpy(fill, start, n * sizeof(int));
    for (int i = 0; i < n; i++) { int r = find(i); bucket[fill[r]++] = i; }  /* ascending */
    memcpy(slot, start, n * sizeof(int));                                    /* next free slot per root */
    for (int v = 1; v <= n; v++) {
        int r = find(pos[v]);
        out[bucket[slot[r]++]] = v;
    }
    memcpy(A, out, n * sizeof(int));
    free(p); free(sz); free(start); free(slot); free(bucket); free(fill); free(pos); free(out);
}

/* ---------------- brute force: BFS over every reachable permutation ------- */
static void brute(int n, const int *A, int m, int (*ed)[2], int *best) {
    static int states[800][8]; static char seen[1 << 24];
    int h = 0, t = 0;
    #define ENC(a, code) do { code = 0; for (int _i = 0; _i < n; _i++) code = code * 8 + (a)[_i]; } while (0)
    int code; memcpy(states[t++], A, n * sizeof(int)); ENC(A, code); seen[code] = 1;
    memcpy(best, A, n * sizeof(int));
    int touched[800], tc = 0; touched[tc++] = code;
    while (h < t) {
        int cur[8]; memcpy(cur, states[h++], n * sizeof(int));
        for (int i = 0; i < n; i++) if (cur[i] < best[i]) { memcpy(best, cur, n * sizeof(int)); break; }
        else if (cur[i] > best[i]) break;
        for (int e = 0; e < m; e++) {
            int nx[8]; memcpy(nx, cur, n * sizeof(int));
            int a = ed[e][0], b = ed[e][1], tmp = nx[a]; nx[a] = nx[b]; nx[b] = tmp;
            int c2; ENC(nx, c2);
            if (!seen[c2]) { seen[c2] = 1; touched[tc++] = c2; memcpy(states[t++], nx, n * sizeof(int)); }
        }
    }
    for (int i = 0; i < tc; i++) seen[touched[i]] = 0;
}

static int tests = 0, fails = 0;
#define CHECK(c) do { tests++; if (!(c)) { fails++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)

static void self_tests(void) {
    /* A = [3,2,1,4], swaps (1,3): positions 1 and 3 connected -> [1,2,3,4] */
    int A[] = {3, 2, 1, 4}; int e[][2] = {{0, 2}};
    solve(4, A, 1, e); int x[] = {1, 2, 3, 4};
    for (int i = 0; i < 4; i++) CHECK(A[i] == x[i]);
    /* chain 0-1, 1-2 lets position 0 and 2 cooperate even without a direct edge */
    int B[] = {3, 1, 2, 5, 4}; int e2[][2] = {{0, 1}, {1, 2}, {3, 4}}; int y[] = {1, 2, 3, 4, 5};
    solve(5, B, 3, e2); for (int i = 0; i < 5; i++) CHECK(B[i] == y[i]);
    /* no edges: unchanged */
    int C[] = {2, 3, 1}; solve(3, C, 0, e2); CHECK(C[0] == 2 && C[1] == 3 && C[2] == 1);
    /* two components that interleave: {0,2},{1,3}: [4,3,2,1] -> [2,1,4,3] */
    int D[] = {4, 3, 2, 1}; int e3[][2] = {{0, 2}, {1, 3}}; solve(4, D, 2, e3);
    CHECK(D[0] == 2 && D[1] == 1 && D[2] == 4 && D[3] == 3);

    srand(99);
    for (int it = 0; it < 1500; it++) {
        int n = 1 + rand() % 6, m = rand() % 8, a[8], b[8], bf[8], ed[8][2];
        for (int i = 0; i < n; i++) a[i] = i + 1;
        for (int i = n - 1; i > 0; i--) { int j = rand() % (i + 1), t = a[i]; a[i] = a[j]; a[j] = t; }
        for (int i = 0; i < m; i++) { ed[i][0] = rand() % n; ed[i][1] = rand() % n; }
        memcpy(b, a, sizeof a); brute(n, a, m, ed, bf); solve(n, b, m, ed);
        for (int i = 0; i < n; i++) CHECK(b[i] == bf[i]);
    }
}

int main(int argc, char **argv) {
    if (argc > 1 && !strcmp(argv[1], "-i")) {
        int n, m; if (scanf("%d", &n) != 1) return 1;
        int *A = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) if (scanf("%d", &A[i]) != 1) return 1;
        if (scanf("%d", &m) != 1) return 1;
        int (*ed)[2] = malloc((m + 1) * sizeof *ed);
        for (int i = 0; i < m; i++) { if (scanf("%d %d", &ed[i][0], &ed[i][1]) != 2) return 1; ed[i][0]--; ed[i][1]--; }
        solve(n, A, m, ed);
        for (int i = 0; i < n; i++) printf("%d%c", A[i], i + 1 < n ? ' ' : '\n');
        return 0;
    }
    self_tests();
    printf("Q5: %d checks, %d failed -> %s\n", tests, fails, fails ? "FAIL" : "ALL PASS");
    return fails != 0;
}
