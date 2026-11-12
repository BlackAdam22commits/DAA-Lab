/*
 * Q10 - Greedy Superstring (shortest common superstring, SCS) - exploration
 *
 * SCS is NP-hard, so the exact optimum is only computed by brute force for tiny n.
 *
 * INPUT REPRESENTATION: set of n strings, first made SUBSTRING-FREE (drop duplicates
 *   and strings contained in another; they never change the optimum). Then
 *   ov[i][j] = longest proper suffix of s_i that is a prefix of s_j (n x n matrix).
 *   A superstring = an ordering of the strings; its length is
 *       sum |s_i|  -  sum of ov between consecutive strings.
 *
 * GREEDY ALGORITHM: repeatedly merge the ordered pair (i,j) with maximum overlap
 *   (ties: smallest i, then smallest j), until one string is left.
 *   Complexity of this simple version: n-1 merges x O(n^2) pair scans x overlap
 *   cost O(L) => O(n^3 L) (L = max length). Linear-time O(N) (N = total length)
 *   implementations exist using Aho-Corasick / suffix structures.
 * EXACT (reference): try all n! orderings => O(n! * n).
 *
 * STATUS OF THE CONJECTURE (see README): ratio 2 is attained by
 *   {c(ab)^m, (ab)^m c, (ba)^m}; a Sept-2026 preprint (Shibata, arXiv 2609.01365)
 *   claims a family with ratio -> 9/4 using strings of EVEN length k >= 10.
 *   The 9/4 instances are far larger than anything exhaustive search can reach, so
 *   this program only (a) reproduces the ratio-2 family and (b) searches random
 *   small instances; it does NOT re-verify the new claim.
 *
 * Build: gcc -O2 -Wall -o q10 Q10_greedy_superstring.c
 * Run: ./q10 | ./q10 stdin  (input: n then n strings)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXN 12
#define MAXL 256
static char S[MAXN][MAXL]; static int n;

static int overlap(const char *a, const char *b) {
    int la = strlen(a), lb = strlen(b), m = la < lb ? la : lb;
    for (int k = m; k > 0; k--) if (!strncmp(a + la - k, b, k)) return k;
    return 0;
}
static int prepare(char in[][MAXL], int cnt, char out[][MAXL]) {  /* substring-free set */
    int m = 0;
    for (int i = 0; i < cnt; i++) { int drop = 0;
        for (int j = 0; j < cnt && !drop; j++) if (i != j) {
            if (strstr(in[j], in[i]) && (strcmp(in[i], in[j]) || j < i)) drop = 1; }
        if (!drop) strcpy(out[m++], in[i]); }
    return m;
}
static int greedy(char s[][MAXL], int m, char *res) {
    char t[MAXN][MAXL]; int k = m; for (int i = 0; i < m; i++) strcpy(t[i], s[i]);
    while (k > 1) {
        int bi = 0, bj = 1, bo = -1;
        for (int i = 0; i < k; i++) for (int j = 0; j < k; j++) if (i != j) { int o = overlap(t[i], t[j]); if (o > bo) { bo = o; bi = i; bj = j; } }
        char merged[MAXL]; snprintf(merged, MAXL, "%s%s", t[bi], t[bj] + bo);
        int lo = bi < bj ? bi : bj, hi = bi < bj ? bj : bi;
        strcpy(t[lo], merged); if (hi != k - 1) strcpy(t[hi], t[k - 1]); k--;
    }
    strcpy(res, t[0]); return strlen(res);
}
static int best, ovm[MAXN][MAXN], tl, mm;
static void perm(int *p, int k, int len) {
    if (k == mm) { if (len < best) best = len; return; }
    for (int i = k; i < mm; i++) { int t = p[k]; p[k] = p[i]; p[i] = t;
        int add = k ? (int)strlen(S[p[k]]) - ovm[p[k - 1]][p[k]] : (int)strlen(S[p[k]]);
        perm(p, k + 1, len + add); t = p[k]; p[k] = p[i]; p[i] = t; }
}
static int optimal(char s[][MAXL], int m) {
    memcpy(S, s, sizeof(char) * MAXL * m); mm = m;
    for (int i = 0; i < m; i++) for (int j = 0; j < m; j++) ovm[i][j] = i == j ? 0 : overlap(S[i], S[j]);
    int p[MAXN]; for (int i = 0; i < m; i++) p[i] = i; best = 1 << 30; perm(p, 0, 0); return best;
}
static void report(char in[][MAXL], int cnt, const char *name) {
    char s[MAXN][MAXL], g[MAXL]; int m = prepare(in, cnt, s); int gl = greedy(s, m, g), ol = optimal(s, m);
    printf("%-28s greedy=%3d optimal=%3d ratio=%.3f\n", name, gl, ol, (double)gl / ol);
}
int main(int argc, char **argv) {
    char in[MAXN][MAXL];
    if (argc > 1 && !strcmp(argv[1], "stdin")) { if (scanf("%d", &n) != 1 || n > MAXN) return 1; for (int i = 0; i < n; i++) if (scanf("%255s", in[i]) != 1) return 1; report(in, n, "input"); return 0; }
    printf("Classic ratio->2 family {c(ab)^m, (ab)^m c, (ba)^m}:\n");
    for (int m = 1; m <= 12; m += 3) {
        char a[MAXL] = "c", b[MAXL] = "", c[MAXL] = ""; for (int i = 0; i < m; i++) { strcat(a, "ab"); strcat(b, "ab"); strcat(c, "ba"); } strcat(b, "c");
        strcpy(in[0], a); strcpy(in[1], b); strcpy(in[2], c); char nm[32]; snprintf(nm, 32, "m=%d", m); report(in, 3, nm);
    }
    srand(21); double worst = 1; int viol = 0, T = 20000; char wit[MAXN][MAXL]; int wn = 0;
    for (int tc = 0; tc < T; tc++) {
        int cnt = 2 + rand() % 5; for (int i = 0; i < cnt; i++) { int L = 2 + rand() % 5; for (int j = 0; j < L; j++) in[i][j] = 'a' + rand() % 2; in[i][L] = 0; }
        char s[MAXN][MAXL], g[MAXL]; int m = prepare(in, cnt, s); if (m < 2) continue;
        int gl = greedy(s, m, g), ol = optimal(s, m); double r = (double)gl / ol;
        if (r > worst) { worst = r; wn = cnt; memcpy(wit, in, sizeof(in)); } if (r > 2.0) viol++;
    }
    printf("Random search (%d binary instances, n<=6, len<=6): worst ratio %.3f, instances with ratio>2: %d\n", T, worst, viol);
    if (wn) { printf("  worst instance:"); for (int i = 0; i < wn; i++) printf(" %s", wit[i]); printf("\n"); }
    return 0;
}
