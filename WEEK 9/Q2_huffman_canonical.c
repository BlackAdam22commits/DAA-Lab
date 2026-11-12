/*
 * Q2 - Huffman Coding with canonical codebook
 *
 * INPUT REPRESENTATION: n symbols (names) with positive frequencies in
 *   parallel arrays; a binary min-heap of tree-node indices; a node pool
 *   {freq, left, right, id} where id (creation order) breaks ties so the
 *   output is deterministic.
 *
 * ALGORITHM
 *   1. Greedy: repeatedly extract the two least-frequent nodes and merge them
 *      (correctness: exchange argument - two rarest symbols can be siblings at
 *      maximum depth in some optimal tree; the reduced problem is again Huffman).
 *   2. Read each symbol's code LENGTH from the tree.
 *   3. Canonical codes: sort symbols by (length, symbol); code of first = 0..0;
 *      next code = (prev + 1) << (len_next - len_prev).
 *      Thus codes are ordered lexicographically by length, then by symbol.
 *
 * COMPLEXITY: n-1 merges, each 2 extract-mins + 1 insert = O(log n)
 *   -> O(n log n) time.  Canonical step: sort O(n log n) + O(n*L) code build.
 *   Space O(n).
 *
 * VALIDATION: Kraft sum == 1, prefix-freeness, and expected length compared with
 *   an independent DP-free check: optimal cost == sum of internal-node weights
 *   (computed by a simple O(n^2) merge) on random instances; also entropy bound
 *   H <= avg < H+1.
 * Build: gcc -O2 -Wall -o q2 Q2_huffman_canonical.c -lm
 * Run: ./q2 | ./q2 stdin  (input: n, then n lines "symbol freq")
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAXN 256
typedef struct { long long f; int l, r, id; } Node;
static Node nd[2 * MAXN]; static int heap[2 * MAXN], hs, nn;
static int less(int a, int b) { return nd[a].f < nd[b].f || (nd[a].f == nd[b].f && nd[a].id < nd[b].id); }
static void push(int x) { int i = hs++; heap[i] = x; while (i && less(heap[i], heap[(i - 1) / 2])) { int p = (i - 1) / 2, t = heap[i]; heap[i] = heap[p]; heap[p] = t; i = p; } }
static int pop(void) {
    int top = heap[0]; heap[0] = heap[--hs]; int i = 0;
    for (;;) { int l = 2 * i + 1, r = l + 1, m = i;
        if (l < hs && less(heap[l], heap[m])) m = l; if (r < hs && less(heap[r], heap[m])) m = r;
        if (m == i) break; int t = heap[i]; heap[i] = heap[m]; heap[m] = t; i = m; }
    return top;
}
static void depth(int u, int d, int *len) { if (nd[u].l < 0) { len[u] = d; return; } depth(nd[u].l, d + 1, len); depth(nd[u].r, d + 1, len); }

typedef struct { int sym, len; unsigned long long code; } Entry;
static int ecmp(const void *a, const void *b) { const Entry *x = a, *y = b; if (x->len != y->len) return x->len - y->len; return x->sym - y->sym; }

/* builds canonical code; returns expected length numerator (sum f*len) */
static long long huffman(const long long *f, int n, Entry *out) {
    hs = 0; nn = 0;
    for (int i = 0; i < n; i++) { nd[nn] = (Node){f[i], -1, -1, nn}; push(nn++); }
    if (n == 1) { out[0] = (Entry){0, 1, 0}; return f[0]; }
    while (hs > 1) { int a = pop(), b = pop(); nd[nn] = (Node){nd[a].f + nd[b].f, a, b, nn}; push(nn++); }
    int len[2 * MAXN] = {0}; depth(pop(), 0, len);
    for (int i = 0; i < n; i++) out[i] = (Entry){i, len[i], 0};
    qsort(out, n, sizeof(Entry), ecmp);
    unsigned long long c = 0; out[0].code = 0;
    for (int i = 1; i < n; i++) { c = (c + 1) << (out[i].len - out[i - 1].len); out[i].code = c; }
    long long tot = 0; for (int i = 0; i < n; i++) tot += f[out[i].sym] * out[i].len;
    return tot;
}
static void pr(unsigned long long c, int len) { for (int i = len - 1; i >= 0; i--) putchar('0' + ((c >> i) & 1)); }
static long long simple_cost(long long *f, int n) { /* O(n^2) independent check */
    long long a[MAXN], cost = 0; int m = n; memcpy(a, f, n * sizeof *a);
    while (m > 1) { int i1 = 0; for (int i = 1; i < m; i++) if (a[i] < a[i1]) i1 = i; long long x = a[i1]; a[i1] = a[--m];
        int i2 = 0; for (int i = 1; i < m; i++) if (a[i] < a[i2]) i2 = i; long long y = a[i2]; a[i2] = x + y; cost += x + y; }
    return cost;
}
int main(int argc, char **argv) {
    char names[MAXN][16]; long long f[MAXN]; int n; Entry e[MAXN];
    if (argc > 1 && !strcmp(argv[1], "stdin")) { if (scanf("%d", &n) != 1) return 1; for (int i = 0; i < n; i++) if (scanf("%15s %lld", names[i], &f[i]) != 2) return 1; }
    else { n = 6; const char *s[] = {"a", "b", "c", "d", "e", "f"}; long long ff[] = {45, 13, 12, 16, 9, 5}; for (int i = 0; i < n; i++) { strcpy(names[i], s[i]); f[i] = ff[i]; } }
    long long tot = huffman(f, n, e), sum = 0; for (int i = 0; i < n; i++) sum += f[i];
    printf("Canonical Huffman codebook (sorted by length, then symbol):\n");
    for (int i = 0; i < n; i++) { printf("  %-4s freq=%-5lld len=%d code=", names[e[i].sym], f[e[i].sym], e[i].len); pr(e[i].code, e[i].len); putchar('\n'); }
    printf("Expected length = %lld/%lld = %.4f bits/symbol\n", tot, sum, (double)tot / sum);
    if (argc > 1) return 0;
    srand(5); int bad = 0, T = 3000;
    for (int tc = 0; tc < T; tc++) {
        int m = 1 + rand() % 30; long long g[MAXN], s = 0; for (int i = 0; i < m; i++) { g[i] = 1 + rand() % 100; s += g[i]; }
        Entry x[MAXN]; long long t = huffman(g, m, x);
        double kraft = 0, H = 0; for (int i = 0; i < m; i++) { kraft += ldexp(1.0, -x[i].len); H -= (double)g[i] / s * log2((double)g[i] / s); }
        int ok = fabs(kraft - 1.0) < 1e-12 || m == 1;
        for (int i = 0; i < m && ok; i++) for (int j = 0; j < m && ok; j++) if (i != j && x[i].len <= x[j].len && (x[j].code >> (x[j].len - x[i].len)) == x[i].code) ok = 0; /* prefix check */
        if (m > 1 && t != simple_cost(g, m)) ok = 0;
        double avg = (double)t / s; if (m > 1 && (avg < H - 1e-9 || avg >= H + 1)) ok = 0;
        if (!ok) bad++;
    }
    printf("Validation (Kraft=1, prefix-free, cost==independent merge, H<=L<H+1): %d/%d failures\n", bad, T);
    return 0;
}
