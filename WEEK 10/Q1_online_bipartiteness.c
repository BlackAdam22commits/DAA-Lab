/*
 * Q1 - Online Graph Bipartiteness Testing  (DAA Lab-10)
 *
 * Representation : edge STREAM (edges arrive online, so no adjacency list is
 *                  ever built).  State = DSU with a "parity to parent" bit.
 * Idea           : parity[x] = colour of x XOR colour of its DSU parent.
 *                  After find(x) we also know colour(x) relative to its root.
 *                  Edge (u,v):
 *                    - different trees -> merge, choose parity so that u,v get
 *                      opposite colours  (cannot create a cycle)
 *                    - same tree, parity(u) != parity(v) -> even cycle, OK
 *                    - same tree, parity(u) == parity(v) -> ODD cycle, reject
 *                  Self-loops (u==v) are odd cycles of length 1 -> rejected.
 *
 * Complexity     : find/union by size + path compression => O(alpha(V)) amortised
 *                  per edge.   E edges : O(V + E*alpha(V)) time, O(V) space.
 *                  (Offline BFS/DFS re-colouring after every edge would be
 *                   O(E*(V+E)).)
 *
 * Usage          : ./q1            -> runs built-in tests + random brute-force check
 *                  ./q1 -i < in    -> input: V E, then E lines "u v" (1-indexed)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { int *p, *sz; unsigned char *par; int n; } DSU;

static void dsu_init(DSU *d, int n) {
    d->n = n;
    d->p = malloc(n * sizeof(int));
    d->sz = malloc(n * sizeof(int));
    d->par = calloc(n, 1);
    for (int i = 0; i < n; i++) { d->p[i] = i; d->sz[i] = 1; }
}
static void dsu_free(DSU *d) { free(d->p); free(d->sz); free(d->par); }

/* returns root of x; *px = colour of x relative to root */
static int find(DSU *d, int x, int *px) {
    if (d->p[x] == x) { *px = 0; return x; }
    int pp, r = find(d, d->p[x], &pp);   /* depth <= log V (union by size) */
    d->par[x] ^= pp;
    d->p[x] = r;
    *px = d->par[x];
    return r;
}

/* 1 = edge accepted, 0 = would create odd cycle (skipped) */
static int add_edge(DSU *d, int u, int v) {
    int pu, pv, ru = find(d, u, &pu), rv = find(d, v, &pv);
    if (ru == rv) return pu != pv;
    if (d->sz[ru] < d->sz[rv]) { int t = ru; ru = rv; rv = t; }
    d->p[rv] = ru;
    d->par[rv] = pu ^ pv ^ 1;
    d->sz[ru] += d->sz[rv];
    return 1;
}

/* ---------------- brute force: BFS 2-colouring on accepted edges ---------- */
static int brute_bip(int n, int adj[][12]) {
    int col[12]; memset(col, -1, sizeof col);
    int q[12];
    for (int s = 0; s < n; s++) if (col[s] < 0) {
        int h = 0, t = 0; q[t++] = s; col[s] = 0;
        while (h < t) { int x = q[h++];
            for (int y = 0; y < n; y++) if (adj[x][y]) {
                if (col[y] < 0) { col[y] = col[x] ^ 1; q[t++] = y; }
                else if (col[y] == col[x]) return 0; } }
    }
    return 1;
}

static int tests = 0, fails = 0;
#define CHECK(c) do { tests++; if (!(c)) { fails++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)

static void run_case(int V, int E, int (*ed)[2], const int *expect) {
    DSU d; dsu_init(&d, V);
    for (int i = 0; i < E; i++) CHECK(add_edge(&d, ed[i][0], ed[i][1]) == expect[i]);
    dsu_free(&d);
}

static void self_tests(void) {
    int t1[][2] = {{0,1},{1,2},{2,0}};            int e1[] = {1,1,0};
    int t2[][2] = {{0,1},{1,2},{2,3},{3,0}};      int e2[] = {1,1,1,1};   /* C4 ok */
    int t3[][2] = {{0,0}};                         int e3[] = {0};        /* self loop */
    int t4[][2] = {{0,1},{2,3},{1,2},{3,0},{0,2}}; int e4[] = {1,1,1,1,0};
    run_case(3, 3, t1, e1); run_case(4, 4, t2, e2);
    run_case(1, 1, t3, e3); run_case(4, 5, t4, e4);

    srand(12345);
    for (int it = 0; it < 3000; it++) {
        int V = 2 + rand() % 10, E = rand() % 25;
        DSU d; dsu_init(&d, V);
        int adj[12][12]; memset(adj, 0, sizeof adj);
        for (int i = 0; i < E; i++) {
            int u = rand() % V, v = rand() % V;
            int a = adj[u][v], b = adj[v][u];
            adj[u][v] = adj[v][u] = 1;
            int want = (u != v) && brute_bip(V, adj);
            if (!want) { adj[u][v] = a; adj[v][u] = b; }
            CHECK(add_edge(&d, u, v) == want);
        }
        dsu_free(&d);
    }
}

int main(int argc, char **argv) {
    if (argc > 1 && !strcmp(argv[1], "-i")) {
        int V, E;
        if (scanf("%d %d", &V, &E) != 2) return 1;
        DSU d; dsu_init(&d, V);
        for (int i = 1; i <= E; i++) {
            int u, v; if (scanf("%d %d", &u, &v) != 2) break;
            int ok = add_edge(&d, u - 1, v - 1);
            printf("Edge %d (%d,%d): %s\n", i, u, v,
                   ok ? "added (still bipartite)" : "REJECTED - would create an odd cycle");
        }
        dsu_free(&d);
        return 0;
    }
    self_tests();
    printf("Q1: %d checks, %d failed -> %s\n", tests, fails, fails ? "FAIL" : "ALL PASS");
    return fails != 0;
}
