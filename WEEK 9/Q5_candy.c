/*
 * Q5 - Candy distribution (bi-directional slope greedy)
 *
 * INPUT REPRESENTATION: array r[0..n-1] of ratings; output array c[] of candies.
 *
 * ALGORITHM: two passes.
 *   left->right : c[i] = c[i-1]+1 if r[i] > r[i-1], else 1
 *   right->left : c[i] = max(c[i], c[i+1]+1) if r[i] > r[i+1]
 *   Each pass enforces one neighbour constraint with the least possible value,
 *   so c[i] is the pointwise minimum satisfying both; the sum is minimal.
 *   (Equivalent O(1)-space "up/down/peak" slope scan is also implemented.)
 *
 * COMPLEXITY: two linear scans => O(n) time, O(n) space (O(1) extra for slope scan).
 *
 * VALIDATION: fixed-point relaxation (raise a child until no rule is violated),
 *   which converges to the unique minimal solution; compared on random inputs.
 * Build: gcc -O2 -Wall -o q5 Q5_candy.c
 * Run: ./q5 | ./q5 stdin  (input: n then n ratings)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXN 100005
static long long two_pass(const int *r, int n, int *c) {
    for (int i = 0; i < n; i++) c[i] = 1;
    for (int i = 1; i < n; i++) if (r[i] > r[i - 1]) c[i] = c[i - 1] + 1;
    for (int i = n - 2; i >= 0; i--) if (r[i] > r[i + 1] && c[i] < c[i + 1] + 1) c[i] = c[i + 1] + 1;
    long long s = 0; for (int i = 0; i < n; i++) s += c[i]; return s;
}
static long long slope(const int *r, int n) { /* O(1) extra space */
    if (!n) return 0; long long tot = 1; int up = 0, down = 0, peak = 0;
    for (int i = 1; i < n; i++) {
        if (r[i] > r[i - 1]) { up++; peak = up; down = 0; tot += 1 + up; }
        else if (r[i] == r[i - 1]) { up = down = peak = 0; tot += 1; }
        else { down++; up = 0; tot += 1 + down - (peak >= down ? 1 : 0); }
    }
    return tot;
}
static long long brute(const int *r, int n) {
    int c[64]; for (int i = 0; i < n; i++) c[i] = 1; int ch = 1;
    while (ch) { ch = 0; for (int i = 0; i < n; i++) {
        if (i > 0 && r[i] > r[i - 1] && c[i] <= c[i - 1]) { c[i] = c[i - 1] + 1; ch = 1; }
        if (i < n - 1 && r[i] > r[i + 1] && c[i] <= c[i + 1]) { c[i] = c[i + 1] + 1; ch = 1; } } }
    long long s = 0; for (int i = 0; i < n; i++) s += c[i]; return s;
}
int main(int argc, char **argv) {
    static int r[MAXN], c[MAXN]; int n;
    if (argc > 1 && !strcmp(argv[1], "stdin")) { if (scanf("%d", &n) != 1) return 1; for (int i = 0; i < n; i++) if (scanf("%d", &r[i]) != 1) return 1; }
    else { n = 7; int t[] = {1, 0, 2, 5, 3, 2, 2}; memcpy(r, t, sizeof t); }
    long long s = two_pass(r, n, c);
    printf("Candies:"); for (int i = 0; i < n; i++) printf(" %d", c[i]); printf("\nMinimum total = %lld (slope scan: %lld)\n", s, slope(r, n));
    if (argc > 1) return 0;
    srand(2); int bad = 0, T = 20000;
    for (int tc = 0; tc < T; tc++) { int m = 1 + rand() % 12, a[16], cc[16]; for (int i = 0; i < m; i++) a[i] = rand() % 6;
        long long x = two_pass(a, m, cc); if (x != brute(a, m) || x != slope(a, m)) bad++; }
    printf("Validation vs fixed-point relaxation: %d/%d mismatches\n", bad, T);
    return 0;
}
