/*
 * DAA Lab-08, Problem 4: Longest Increasing Subsequence
 * Author: Ayush Pattanaik (B525018)
 * Time Theta(n^2), Space Theta(n)
 * Compile: gcc -Wall -O2 -o q4_lis q4_lis.c
 * Run:     ./q4_lis < q4_lis_sample.txt
 */
#include <stdio.h>
#include <stdlib.h>
int main(void) {
  int n;
  if (scanf("%d", &n) != 1 || n < 0) return 1;
  if (n == 0) { printf("LIS length = 0\n"); return 0; }
  int *a = malloc(n * sizeof(int)), *d = malloc(n * sizeof(int)), *p = malloc(n * sizeof(int));
  int best = 0, bi = 0;
  for (int i = 0; i < n; i++) scanf("%d", &a[i]);
  for (int i = 0; i < n; i++) {
    d[i] = 1; p[i] = -1;                      /* d[i]: LIS ending at a[i] */
    for (int j = 0; j < i; j++)
      if (a[j] < a[i] && d[j] + 1 > d[i]) { d[i] = d[j] + 1; p[i] = j; }
    if (d[i] > best) { best = d[i]; bi = i; }
  }
  printf("LIS length = %d\nOne LIS    =", best);
  int *seq = malloc(best * sizeof(int)), k = best;
  for (int i = bi; i != -1; i = p[i]) seq[--k] = a[i];
  for (int i = 0; i < best; i++) printf(" %d", seq[i]);
  printf("\n");
  free(a); free(d); free(p); free(seq);
  return 0;
}
