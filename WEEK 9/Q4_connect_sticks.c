/*
 * Q4 - Minimum cost to connect sticks
 *
 * INPUT REPRESENTATION: array of n stick lengths (long long) stored as a binary
 *   min-heap.
 *
 * ALGORITHM: repeatedly remove the two shortest sticks x<=y, pay x+y, push x+y.
 *   This is exactly Huffman merging: a stick of length L that takes part in k
 *   merges contributes L*k to the cost, i.e. L*depth(L) in the merge tree, so
 *   minimum total cost == minimum weighted external path length == Huffman tree.
 *   Exchange argument: the two shortest sticks can be merged first (deepest leaves).
 *
 * COMPLEXITY: heapify O(n); n-1 rounds x (2 extract-min + 1 insert) = O(n log n)
 *   time, O(n) space.  Cost may reach ~ n*log n*max(L): use 64-bit.
 *
 * VALIDATION: exhaustive recursion over every pair choice (n <= 7).
 * Build: gcc -O2 -Wall -o q4 Q4_connect_sticks.c
 * Run: ./q4 | ./q4 stdin  (input: n then n lengths)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXN 100005
static long long h[MAXN]; static int hs;
static void up(int i) { while (i && h[i] < h[(i - 1) / 2]) { int p = (i - 1) / 2; long long t = h[i]; h[i] = h[p]; h[p] = t; i = p; } }
static void down(int i) { for (;;) { int l = 2 * i + 1, r = l + 1, m = i; if (l < hs && h[l] < h[m]) m = l; if (r < hs && h[r] < h[m]) m = r; if (m == i) return; long long t = h[i]; h[i] = h[m]; h[m] = t; i = m; } }
static long long pop(void) { long long t = h[0]; h[0] = h[--hs]; down(0); return t; }
static void push(long long x) { h[hs] = x; up(hs++); }
static long long min_cost(const long long *L, int n) {
    hs = n; memcpy(h, L, n * sizeof *L); for (int i = n / 2 - 1; i >= 0; i--) down(i);
    long long cost = 0;
    while (hs > 1) { long long x = pop(), y = pop(); cost += x + y; push(x + y); }
    return cost;
}
static long long brute(long long *a, int n) {
    if (n == 1) return 0; long long best = (long long)4e18;
    for (int i = 0; i < n; i++) for (int j = i + 1; j < n; j++) {
        long long b[8]; int m = 0; for (int k = 0; k < n; k++) if (k != i && k != j) b[m++] = a[k];
        b[m++] = a[i] + a[j]; long long c = a[i] + a[j] + brute(b, m); if (c < best) best = c; }
    return best;
}
int main(int argc, char **argv) {
    static long long L[MAXN]; int n;
    if (argc > 1 && !strcmp(argv[1], "stdin")) { if (scanf("%d", &n) != 1) return 1; for (int i = 0; i < n; i++) if (scanf("%lld", &L[i]) != 1) return 1; }
    else { n = 4; long long t[] = {2, 4, 3, 6}; memcpy(L, t, sizeof t); }
    printf("Minimum total cost = %lld\n", min_cost(L, n));
    if (argc > 1) return 0;
    srand(9); int bad = 0, T = 4000;
    for (int tc = 0; tc < T; tc++) { int m = 1 + rand() % 7; long long a[8]; for (int i = 0; i < m; i++) a[i] = 1 + rand() % 50;
        if (min_cost(a, m) != brute(a, m)) bad++; }
    printf("Validation vs exhaustive search: %d/%d mismatches\n", bad, T);
    return 0;
}
