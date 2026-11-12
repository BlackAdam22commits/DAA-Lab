/*
 * Q3 - Minimum number of refuelling stops (reverse / lazy greedy)
 *
 * INPUT REPRESENTATION: target D, initial fuel F (1 fuel = 1 distance unit),
 *   array of stations (d_i, f_i); sorted by d_i if not already.
 *   A max-heap holds the refuel amounts of stations already PASSED.
 *
 * ALGORITHM: drive as far as possible. Whenever the next station/target is out of
 *   range, retroactively "stop" at the passed station with the LARGEST fuel
 *   (reverse decision - we only pay a stop when forced). If the heap is empty
 *   -> impossible (-1).
 *   Correctness: any stop made is needed; among the stops that would have
 *   extended range, the biggest fuel dominates (exchange argument).
 *
 * COMPLEXITY: sort O(n log n); each station is pushed once and popped at most
 *   once, each heap op O(log n)  =>  O(n log n) time, O(n) space.
 *
 * VALIDATION: O(n^2) DP: best[k] = farthest reach using exactly k stops.
 * Build: gcc -O2 -Wall -o q3 Q3_min_refuel_stops.c
 * Run: ./q3 | ./q3 stdin  (input: D F n, then n lines "d f")
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXN 2048
typedef struct { long long d, f; } St;
static long long hp[MAXN]; static int hs;
static void push(long long x) { int i = hs++; hp[i] = x; while (i && hp[i] > hp[(i - 1) / 2]) { int p = (i - 1) / 2; long long t = hp[i]; hp[i] = hp[p]; hp[p] = t; i = p; } }
static long long pop(void) { long long top = hp[0]; hp[0] = hp[--hs]; int i = 0;
    for (;;) { int l = 2 * i + 1, r = l + 1, m = i; if (l < hs && hp[l] > hp[m]) m = l; if (r < hs && hp[r] > hp[m]) m = r; if (m == i) break; long long t = hp[i]; hp[i] = hp[m]; hp[m] = t; i = m; }
    return top; }
static int cmp(const void *a, const void *b) { long long x = ((const St *)a)->d, y = ((const St *)b)->d; return (x > y) - (x < y); }

static int min_stops(long long D, long long F, St *s, int n) {
    qsort(s, n, sizeof(St), cmp); hs = 0;
    long long reach = F; int stops = 0, i = 0;
    while (reach < D) {
        while (i < n && s[i].d <= reach) push(s[i++].f);
        if (!hs) return -1;
        reach += pop(); stops++;
    }
    return stops;
}
static int dp_stops(long long D, long long F, const St *s, int n) { /* s sorted */
    long long best[MAXN + 1]; for (int k = 0; k <= n; k++) best[k] = -1; best[0] = F;
    for (int i = 0; i < n; i++) for (int k = i; k >= 0; k--) if (best[k] >= s[i].d && best[k] + s[i].f > best[k + 1]) best[k + 1] = best[k] + s[i].f;
    for (int k = 0; k <= n; k++) if (best[k] >= D) return k;
    return -1;
}
int main(int argc, char **argv) {
    St s[MAXN]; long long D, F; int n;
    if (argc > 1 && !strcmp(argv[1], "stdin")) { if (scanf("%lld %lld %d", &D, &F, &n) != 3) return 1; for (int i = 0; i < n; i++) if (scanf("%lld %lld", &s[i].d, &s[i].f) != 2) return 1; }
    else { D = 100; F = 10; n = 4; St t[] = {{10, 60}, {20, 30}, {30, 30}, {60, 40}}; memcpy(s, t, sizeof t); }
    printf("Minimum refuelling stops = %d\n", min_stops(D, F, s, n));
    if (argc > 1) return 0;
    srand(3); int bad = 0, T = 5000;
    for (int tc = 0; tc < T; tc++) {
        int m = rand() % 9; St a[16]; long long DD = 1 + rand() % 60, FF = 1 + rand() % 15;
        for (int i = 0; i < m; i++) { a[i].d = 1 + rand() % 60; a[i].f = 1 + rand() % 20; }
        St b[16]; memcpy(b, a, sizeof a); int g = min_stops(DD, FF, a, m);
        qsort(b, m, sizeof(St), cmp); if (g != dp_stops(DD, FF, b, m)) bad++;
    }
    printf("Validation vs O(n^2) DP: %d/%d mismatches\n", bad, T);
    return 0;
}
