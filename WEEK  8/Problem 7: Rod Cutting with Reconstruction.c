/*
 * DAA Lab-08, Problem 7: Rod Cutting with Reconstruction
 * Author: Ayush Pattanaik (B525018)
 * Time Theta(n^2), Space Theta(n)
 * Compile: gcc -Wall -O2 -o q7_rod_cutting q7_rod_cutting.c
 * Run:     ./q7_rod_cutting < q7_rod_cutting_sample.txt
 */
#include <stdio.h>
#include <stdlib.h>
int main(void) {
  int n;
  if (scanf("%d", &n) != 1 || n < 1) return 1;
  int *p = malloc((n + 1) * sizeof(int)), *r = calloc(n + 1, sizeof(int)), *cut = calloc(n + 1, sizeof(int));
  for (int i = 1; i <= n; i++) scanf("%d", &p[i]);   /* p[i]: price of length i */
  for (int j = 1; j <= n; j++) {
    r[j] = -1;
    for (int i = 1; i <= j; i++)                     /* first piece has length i */
      if (p[i] + r[j - i] > r[j]) { r[j] = p[i] + r[j - i]; cut[j] = i; }
  }
  printf("Maximum revenue = %d\nPieces:", r[n]);
  for (int j = n; j > 0; j -= cut[j]) printf(" %d", cut[j]);
  printf("\n");
  free(p); free(r); free(cut);
  return 0;
}
