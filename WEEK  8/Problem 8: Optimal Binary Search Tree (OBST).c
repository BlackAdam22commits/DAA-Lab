/*
 * DAA Lab-08, Problem 8: Optimal Binary Search Tree (OBST)
 * Author: Ayush Pattanaik (B525018)
 * Time Theta(n^3), Space Theta(n^2)
 * Compile: gcc -Wall -O2 -o q8_obst q8_obst.c -lm
 * Run:     ./q8_obst < q8_obst_sample.txt
 */
#include <stdio.h>
#include <math.h>
#define N 51
double p[N], q[N], e[N + 2][N + 1], w[N + 2][N + 1];
int root[N + 1][N + 1];
void show(int i, int j, int par, const char *side) {
  if (i > j) { printf("  d%d is the %s child of k%d\n", j, side, par); return; }
  int r = root[i][j];
  if (!par) printf("  k%d is the root\n", r);
  else      printf("  k%d is the %s child of k%d\n", r, side, par);
  show(i, r - 1, r, "left"); show(r + 1, j, r, "right");
}
int main(void) {
  int n; double sum = 0;
  if (scanf("%d", &n) != 1 || n < 1 || n >= N) return 1;
  for (int i = 1; i <= n; i++) { scanf("%lf", &p[i]); sum += p[i]; }
  for (int i = 0; i <= n; i++) { scanf("%lf", &q[i]); sum += q[i]; }
  if (fabs(sum - 1.0) > 1e-9) { printf("Error: probabilities must sum to 1 (got %f)\n", sum); return 1; }
  for (int i = 1; i <= n + 1; i++) { e[i][i-1] = q[i-1]; w[i][i-1] = q[i-1]; }
  for (int l = 1; l <= n; l++)
    for (int i = 1; i + l - 1 <= n; i++) {
      int j = i + l - 1;
      e[i][j] = INFINITY;
      w[i][j] = w[i][j-1] + p[j] + q[j];
      for (int r = i; r <= j; r++) {
        double t = e[i][r-1] + e[r+1][j] + w[i][j];
        if (t < e[i][j]) { e[i][j] = t; root[i][j] = r; }
      }
    }
  printf("Minimum expected search cost = %.2f\nOptimal tree:\n", e[1][n]);
  show(1, n, 0, "");
  return 0;
}
