/*
 * DAA Lab-08, Problem 9: Collatz Conjecture (single value and interval analysis)
 * Author: Ayush Pattanaik (B525018)
 * No proven bound; interval mode Theta(b) space
 * Compile: gcc -Wall -O2 -o q9_collatz q9_collatz.c
 * Run:     ./q9_collatz < q9_collatz_sample.txt
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
typedef unsigned long long u64;
typedef struct { u64 steps, peak; int overflow; } Info;
#define LIMIT ((ULLONG_MAX - 1) / 3)        /* n > LIMIT  =>  3n+1 overflows */

Info trajectory(u64 n, int print) {
  Info r = { 0, n, 0 };
  if (print) printf("%llu", n);
  while (n != 1) {
    if (n % 2 == 0) n /= 2;
    else { if (n > LIMIT) { r.overflow = 1; if (print) printf("\n"); return r; } n = 3 * n + 1; }
    r.steps++; if (n > r.peak) r.peak = n;
    if (print) printf(" -> %llu", n);
  }
  if (print) printf("\n");
  return r;
}
void analyse_interval(u64 a, u64 b) {       /* memoised on values <= b */
  unsigned *st = calloc(b + 1, sizeof(unsigned));
  u64 *pk = calloc(b + 1, sizeof(u64)), bestN = a, bestS = 0, bestP = 0, peakN = a;
  pk[1] = 1;
  for (u64 s = 2; s <= b; s++) {
    u64 x = s, k = 0, hi = s;
    while (x >= s) {                        /* until we fall below s */
      if (x % 2 == 0) x /= 2;
      else { if (x > LIMIT) { fprintf(stderr, "overflow at %llu\n", s); exit(1); } x = 3*x + 1; }
      k++; if (x > hi) hi = x;
    }
    st[s] = k + st[x]; pk[s] = hi > pk[x] ? hi : pk[x];
    if (s >= a && st[s] > bestS) { bestS = st[s]; bestN = s; }
    if (s >= a && pk[s] > bestP) { bestP = pk[s]; peakN = s; }
  }
  printf("Longest chain : n=%llu (%llu steps)\nHighest peak  : n=%llu (peak %llu)\n", bestN, bestS, peakN, bestP);
  free(st); free(pk);
}
int main(void) {
  int mode; u64 a, b;
  if (scanf("%d %llu", &mode, &a) != 2 || a < 1) return 1;
  if (mode == 1) {
    Info r = trajectory(a, 1);
    printf("steps=%llu peak=%llu overflow=%s\n", r.steps, r.peak, r.overflow ? "YES" : "no");
  } else if (scanf("%llu", &b) == 1 && b >= a) analyse_interval(a, b);
  return 0;
}
