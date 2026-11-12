/*
 * Q6 - Reorganise string so equal characters are >= K apart
 *
 * INPUT REPRESENTATION: string S (bytes), integer K; frequency table cnt[256];
 *   a max-heap of (count, char) for characters that are currently AVAILABLE and a
 *   FIFO "cooldown" queue (ring buffer of size K) for characters used in the last
 *   K-1 positions.
 *
 * ALGORITHM (greedy): at each position take the available character with the
 *   largest remaining count (ties: smaller char), append it, decrement, and park
 *   it in the cooldown queue; when the queue holds K items, release the oldest back
 *   into the heap.  If the heap is empty while characters remain -> impossible, "".
 *   Intuition: the most frequent character is the hardest to place, so we use it as
 *   soon as it is legal (exchange argument on the bottleneck character).
 *   K <= 1 : any arrangement works, return S.
 *
 * COMPLEXITY: n positions x O(log sigma) heap ops (sigma <= 256 distinct chars)
 *   => O(n log sigma) = O(n) for a fixed alphabet; extra space O(sigma + K).
 *
 * VALIDATION: exhaustive search over all distinct permutations (|S| <= 8) for
 *   feasibility; any produced answer is checked to be a permutation of S with
 *   all equal-character gaps >= K.
 * Build: gcc -O2 -Wall -o q6 Q6_reorganise_k.c
 * Run: ./q6 | ./q6 stdin  (input: S K)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXN 100005
typedef struct { int c; unsigned char ch; } E;
static E hp[256]; static int hs;
static int better(E a, E b) { return a.c > b.c || (a.c == b.c && a.ch < b.ch); }
static void push(E x) { int i = hs++; hp[i] = x; while (i && better(hp[i], hp[(i - 1) / 2])) { int p = (i - 1) / 2; E t = hp[i]; hp[i] = hp[p]; hp[p] = t; i = p; } }
static E pop(void) { E top = hp[0]; hp[0] = hp[--hs]; int i = 0;
    for (;;) { int l = 2 * i + 1, r = l + 1, m = i; if (l < hs && better(hp[l], hp[m])) m = l; if (r < hs && better(hp[r], hp[m])) m = r; if (m == i) break; E t = hp[i]; hp[i] = hp[m]; hp[m] = t; i = m; }
    return top; }
/* returns 1 and fills out on success; 0 if impossible */
static int reorganise(const char *s, int K, char *out) {
    int n = strlen(s); if (K <= 1) { strcpy(out, s); return 1; }
    int cnt[256] = {0}; for (int i = 0; i < n; i++) cnt[(unsigned char)s[i]]++;
    hs = 0; for (int c = 0; c < 256; c++) if (cnt[c]) push((E){cnt[c], (unsigned char)c});
    E *q = malloc(sizeof(E) * K); int qh = 0, qt = 0, qs = 0;
    for (int i = 0; i < n; i++) {
        if (!hs) { free(q); out[0] = 0; return 0; }
        E e = pop(); out[i] = e.ch; e.c--; q[qt] = e; qt = (qt + 1) % K; qs++;
        if (qs == K) { E r = q[qh]; qh = (qh + 1) % K; qs--; if (r.c > 0) push(r); }
    }
    out[n] = 0; free(q); return 1;
}
static int valid(const char *s, const char *o, int K) {
    int n = strlen(s); if ((int)strlen(o) != n) return 0;
    int a[256] = {0}, b[256] = {0}; for (int i = 0; i < n; i++) { a[(unsigned char)s[i]]++; b[(unsigned char)o[i]]++; }
    if (memcmp(a, b, sizeof a)) return 0;
    int last[256]; for (int i = 0; i < 256; i++) last[i] = -1000000;
    for (int i = 0; i < n; i++) { unsigned char c = o[i]; if (i - last[c] < K) return 0; last[c] = i; }
    return 1;
}
static int cnt2[256], tot2, K2;
static int dfs(int pos, int *last) {
    if (pos == tot2) return 1;
    for (int c = 0; c < 256; c++) if (cnt2[c] && pos - last[c] >= K2) { int sv = last[c]; last[c] = pos; cnt2[c]--; int ok = dfs(pos + 1, last); cnt2[c]++; last[c] = sv; if (ok) return 1; }
    return 0;
}
static int brute_feasible(const char *s, int K) {
    memset(cnt2, 0, sizeof cnt2); tot2 = strlen(s); K2 = K; for (int i = 0; s[i]; i++) cnt2[(unsigned char)s[i]]++;
    int last[256]; for (int i = 0; i < 256; i++) last[i] = -1000000; return dfs(0, last);
}
int main(int argc, char **argv) {
    static char s[MAXN], o[MAXN]; int K;
    if (argc > 1 && !strcmp(argv[1], "stdin")) { if (scanf("%100000s %d", s, &K) != 2) return 1; }
    else { strcpy(s, "aabbcc"); K = 3; }
    if (reorganise(s, K, o)) printf("S=\"%s\" K=%d  ->  \"%s\"\n", s, K, o); else printf("S=\"%s\" K=%d  ->  \"\" (impossible)\n", s, K);
    if (argc > 1) return 0;
    srand(4); int bad = 0, T = 20000, feas = 0;
    for (int tc = 0; tc < T; tc++) {
        int n = 1 + rand() % 8, A = 1 + rand() % 4; char t[16]; for (int i = 0; i < n; i++) t[i] = 'a' + rand() % A; t[n] = 0;
        int k = 1 + rand() % 5; char r[16]; int got = reorganise(t, k, r), exp = brute_feasible(t, k);
        feas += exp; if (got != exp || (got && !valid(t, r, k))) { if (bad < 3) printf("MISMATCH %s k=%d got=%d exp=%d\n", t, k, got, exp); bad++; }
    }
    printf("Validation vs exhaustive search: %d/%d mismatches (%d feasible cases)\n", bad, T, feas);
    return 0;
}
