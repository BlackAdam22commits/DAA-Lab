/*
 * Q3 - Maximum Weight Component with Size Constraint K  (DAA Lab-10)
 *
 * Representation : vertex-weight array w[] + edge STREAM (u, v, edge-weight).
 *                  State = DSU storing size[root] and sum[root].
 *                  (Edge weights do not affect "total VERTEX weight of a
 *                   component", so they are read and ignored.)
 * Why not a plain running maximum?
 *                  A component of size <= K may later be merged into one of
 *                  size > K; then it STOPS being a candidate.  So the answer can
 *                  go DOWN, and we need "max over the CURRENT valid components".
 * Idea           : keep a max-heap of snapshots (sum, root, size) with LAZY
 *                  deletion.  Every time a component is created with size <= K
 *                  push its snapshot.  A snapshot is stale when its root is no
 *                  longer a root or its size changed (it was merged).  To answer,
 *                  pop stale tops, then peek.  Works for negative weights too.
 *
 * Complexity     : pushes <= V + (#merges) <= 2V, each push/pop O(log V)
 *                  => O((V + E) * alpha(V) + V log V)  time ~ O((V+E) log V),
 *                  O(V) space.  Peeking is O(1) amortised.
 *                  (Recomputing all components after each edge: O(E*(V+E)).)
 *
 * Usage          : ./q3           -> built-in tests + random brute-force check
 *                  ./q3 -i < in   -> input: V K / w1..wV / E / E lines "u v wt" (1-indexed)
 *                                    prints best value before any edge and after every edge
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long long ll;
typedef struct { ll sum; int root, size; } Snap;

typedef struct {
    int n, K;
    int *p, *sz; ll *sum;
    Snap *h; int hn;
} State;

/* ---- max-heap on Snap.sum ---- */
static void hpush(State *s, Snap x) {
    int i = s->hn++; s->h[i] = x;
    while (i && s->h[(i-1)/2].sum < s->h[i].sum) {
        Snap t = s->h[i]; s->h[i] = s->h[(i-1)/2]; s->h[(i-1)/2] = t; i = (i-1)/2; }
}
static void hpop(State *s) {
    s->h[0] = s->h[--s->hn]; int i = 0;
    for (;;) { int l = 2*i+1, r = l+1, m = i;
        if (l < s->hn && s->h[l].sum > s->h[m].sum) m = l;
        if (r < s->hn && s->h[r].sum > s->h[m].sum) m = r;
        if (m == i) break;
        Snap t = s->h[i]; s->h[i] = s->h[m]; s->h[m] = t; i = m; }
}

static void st_init(State *s, int n, int K, const ll *w) {
    s->n = n; s->K = K; s->hn = 0;
    s->p = malloc(n * sizeof(int)); s->sz = malloc(n * sizeof(int));
    s->sum = malloc(n * sizeof(ll)); s->h = malloc(2 * (size_t)n * sizeof(Snap));
    for (int i = 0; i < n; i++) {
        s->p[i] = i; s->sz[i] = 1; s->sum[i] = w[i];
        if (1 <= K) hpush(s, (Snap){w[i], i, 1});
    }
}
static void st_free(State *s) { free(s->p); free(s->sz); free(s->sum); free(s->h); }

static int find(State *s, int x) {
    while (s->p[x] != x) { s->p[x] = s->p[s->p[x]]; x = s->p[x]; }
    return x;
}
static void add_edge(State *s, int u, int v) {
    int a = find(s, u), b = find(s, v);
    if (a == b) return;
    if (s->sz[a] < s->sz[b]) { int t = a; a = b; b = t; }
    s->p[b] = a; s->sz[a] += s->sz[b]; s->sum[a] += s->sum[b];
    if (s->sz[a] <= s->K) hpush(s, (Snap){s->sum[a], a, s->sz[a]});
}
/* 1 and *out = answer, or 0 if no component has size <= K */
static int query(State *s, ll *out) {
    while (s->hn) {
        Snap t = s->h[0];
        if (s->p[t.root] == t.root && s->sz[t.root] == t.size) { *out = t.sum; return 1; }
        hpop(s);
    }
    return 0;
}

/* ---------------- brute force: label components from scratch -------------- */
static int brute(int n, int K, const ll *w, int m, int (*ed)[2], ll *out) {
    int lab[16]; for (int i = 0; i < n; i++) lab[i] = i;
    /* simple relaxation until stable (labels only shrink) */
    for (int ch = 1; ch;) { ch = 0;
        for (int i = 0; i < m; i++) { int a = ed[i][0], b = ed[i][1];
            if (lab[a] != lab[b]) { int lo = lab[a] < lab[b] ? lab[a] : lab[b]; lab[a] = lab[b] = lo; ch = 1; } } }
    int found = 0; ll best = 0;
    for (int c = 0; c < n; c++) { int cnt = 0; ll sm = 0;
        for (int i = 0; i < n; i++) if (lab[i] == c) { cnt++; sm += w[i]; }
        if (cnt && cnt <= K && (!found || sm > best)) { found = 1; best = sm; } }
    *out = best; return found;
}

static int tests = 0, fails = 0;
#define CHECK(c) do { tests++; if (!(c)) { fails++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)

static void self_tests(void) {
    /* hand example: answers 6,7,7,6,none  (best DROPS when {1,2,3,4} outgrows K) */
    ll w[] = {4, 3, 5, 2, 6}; int ed[][2] = {{0,1},{2,3},{1,2},{3,4}};
    ll exp[] = {6, 7, 7, 6, 0}; int has[] = {1, 1, 1, 1, 0}; ll got;
    State s; st_init(&s, 5, 3, w);
    CHECK(query(&s, &got) && got == 6);
    for (int i = 0; i < 4; i++) {
        add_edge(&s, ed[i][0], ed[i][1]);
        int ok = query(&s, &got);
        CHECK(ok == has[i + 1]); if (ok) CHECK(got == exp[i + 1]);
    }
    st_free(&s);
    /* K = 0 -> never any valid component */
    st_init(&s, 2, 0, w); CHECK(!query(&s, &got)); st_free(&s);

    srand(2026);
    for (int it = 0; it < 4000; it++) {
        int n = 1 + rand() % 9, K = rand() % (n + 2), m = rand() % 14;
        ll ww[16]; int e[16][2];
        for (int i = 0; i < n; i++) ww[i] = (rand() % 21) - 8;      /* negatives too */
        for (int i = 0; i < m; i++) { e[i][0] = rand() % n; e[i][1] = rand() % n; }
        State t; st_init(&t, n, K, ww); ll a, b;
        for (int i = 0; i <= m; i++) {
            if (i) add_edge(&t, e[i-1][0], e[i-1][1]);
            int x = query(&t, &a), y = brute(n, K, ww, i, e, &b);
            CHECK(x == y); if (x && y) CHECK(a == b);
        }
        st_free(&t);
    }
}

int main(int argc, char **argv) {
    if (argc > 1 && !strcmp(argv[1], "-i")) {
        int V, K, E;
        if (scanf("%d %d", &V, &K) != 2) return 1;
        ll *w = malloc(V * sizeof(ll));
        for (int i = 0; i < V; i++) if (scanf("%lld", &w[i]) != 1) return 1;
        if (scanf("%d", &E) != 1) return 1;
        State s; st_init(&s, V, K, w); ll b;
        printf("start : %s", query(&s, &b) ? "" : "none\n");
        if (query(&s, &b)) printf("%lld\n", b);
        for (int i = 1; i <= E; i++) {
            int u, v; ll wt; if (scanf("%d %d %lld", &u, &v, &wt) != 3) break;
            add_edge(&s, u - 1, v - 1);
            if (query(&s, &b)) printf("edge %d: %lld\n", i, b); else printf("edge %d: none\n", i);
        }
        st_free(&s); free(w); return 0;
    }
    self_tests();
    printf("Q3: %d checks, %d failed -> %s\n", tests, fails, fails ? "FAIL" : "ALL PASS");
    return fails != 0;
}
