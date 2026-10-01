/*
 * DAA Lab-08, Problem 5: Maximum Sum Increasing Subsequence
 * Author: Ayush Pattanaik (B525018)
 * Time Theta(n^2), Space Theta(n)
 * Compile: gcc -Wall -O2 -o q5_msis q5_msis.c
 * Run:     ./q5_msis < q5_msis_sample.txt
 */
#include <stdio.h>
#include <stdlib.h>
int main(void) {
  int n;
  if (scanf("%d", &n) != 1 || n < 1) return 1;
  long long *a = malloc(n * sizeof(long long)), *s = malloc(n * sizeof(long long));
  int *p = malloc(n * sizeof(int)), bi = 0;
  for (int i = 0; i < n; i++) scanf("%lld", &a[i]);
  for (int i = 0; i < n; i++) {
    s[i] = a[i]; p[i] = -1;                   /* s[i]: max-sum IS ending at a[i] */
    for (int j = 0; j < i; j++)
      if (a[j] < a[i] && s[j] + a[i] > s[i]) { s[i] = s[j] + a[i]; p[i] = j; }
    if (s[i] > s[bi]) bi = i;
  }
  int len = 0;
  for (int i = bi; i != -1; i = p[i]) len++;
  long long *seq = malloc(len * sizeof(long long));
  for (int i = bi, k = len; i != -1; i = p[i]) seq[--k] = a[i];
  printf("Max sum = %lld\nSubsequence =", s[bi]);
  for (int k = 0; k < len; k++) printf(" %lld", seq[k]);
  printf("\n");
  free(a); free(s); free(p); free(seq);
  return 0;
}
