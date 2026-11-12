/*
 * Q7 - Minimise deviation (two-way greedy with max-heap)
 *
 * Operations: odd -> *2 ; even -> /2.   deviation = max - min.
 *
 * INPUT REPRESENTATION: array a[0..n-1] of positive ints (64-bit used); a
 *   max-heap of current values plus a running minimum.
 *
 * ALGORITHM
 *   Normalise: multiply every odd element by 2 (an odd x can only be x or 2x, so
 *   this gives each element its MAXIMUM reachable value; thereafter only "divide
 *   even by 2" is needed -> the two-way problem becomes one-way).
 *   Loop: record best = min(best, max - min).  Pop the max; if it is odd it cannot
 *   decrease further -> stop.  Else push max/2, update min.
 *   Why stopping is right: the maximum is the only element whose reduction can
 *   shrink the deviation; once it can't be reduced, no later state can be better.
 *
 * COMPLEXITY: every element can be halved at most log2(M) times (M = max value
 *   after normalisation), each halving is one heap op:
 *   O(n log n) build + O(n log M * log n) loop  =>  O(n log n log M) time, O(n) space.
 *
 * VALIDATION: each element's reachable set is {odd*2^j : 0<=j<=max(k,1)} (e = odd*2^k);
 *   exhaustive enumeration of the product for n <= 5.
 * Build: gcc -O2 -Wall -o q7 Q7_min_deviation.c
 * Run: ./q7 | ./q7 stdin  (input: n then n ints)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXN 100005
typedef long long ll;
static ll h[MAXN]; static int hs;
static void up(int i) { while (i && h[i] > h[(i - 1) / 2]) { int p = (i - 1) / 2; ll t = h[i]; h[i] = h[p]; h[p] = t; i = p; } }
static void down(int i) { for (;;) { int l = 2 * i + 1, r = l + 1, m = i; if (l < hs && h[l] > h[m]) m = l; if (r < hs && h[r] > h[m]) m = r; if (m == i) return; ll t = h[i]; h[i] = h[m]; h[m] = t; i = m; } }
static ll min_dev(const ll *a, int n) {
    hs = n; ll mn = (ll)4e18;
    for (int i = 0; i < n; i++) { h[i] = (a[i] & 1) ? a[i] * 2 : a[i]; if (h[i] < mn) mn = h[i]; }
    for (int i = n / 2 - 1; i >= 0; i--) down(i);
    ll best = h[0] - mn;
    while (h[0] % 2 == 0) {
        ll mx = h[0]; h[0] = mx / 2; if (h[0] < mn) mn = h[0]; down(0);
        if (h[0] - mn < best) best = h[0] - mn;
    }
    return best;
}
static ll bbest, ba[8]; static int bn;
static void rec(int i, ll mn, ll mx) {
    if (i == bn) { if (mx - mn < bbest) bbest = mx - mn; return; }
    ll o = ba[i]; int k = 0; while (o % 2 == 0) { o /= 2; k++; } int top = k > 1 ? k : 1;
    for (int j = 0; j <= top; j++) { ll v = o << j; rec(i + 1, v < mn ? v : mn, v > mx ? v : mx); }
}
int main(int argc, char **argv) {
    static ll a[MAXN]; int n;
    if (argc > 1 && !strcmp(argv[1], "stdin")) { if (scanf("%d", &n) != 1) return 1; for (int i = 0; i < n; i++) if (scanf("%lld", &a[i]) != 1) return 1; }
    else { n = 4; ll t[] = {4, 1, 5, 20}; memcpy(a, t, sizeof t); }
    printf("Minimum deviation = %lld\n", min_dev(a, n));
    if (argc > 1) return 0;
    srand(8); int bad = 0, T = 20000;
    for (int tc = 0; tc < T; tc++) { int m = 1 + rand() % 5; ll b[8]; for (int i = 0; i < m; i++) b[i] = 1 + rand() % 40;
        memcpy(ba, b, sizeof b); bn = m; bbest = (ll)4e18; rec(0, (ll)4e18, 0);
        if (min_dev(b, m) != bbest) bad++; }
    printf("Validation vs exhaustive search: %d/%d mismatches\n", bad, T);
    return 0;
}
