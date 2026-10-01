/*
 * DAA Lab-08, Problem 1: Minimum Coin Change
 * Author: Ayush Pattanaik (B525018)
 * Time Theta(nV), Space Theta(V)
 * Compile: gcc -Wall -O2 -o q1_min_coin_change q1_min_coin_change.c
 * Run:     ./q1_min_coin_change < q1_min_coin_change_sample.txt
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
int main(void) {
  int n, V;
  if (scanf("%d %d", &n, &V) != 2 || n < 1 || V < 0) return 1;
  int *c = malloc(n * sizeof(int));
  for (int i = 0; i < n; i++) scanf("%d", &c[i]);
  int *dp = malloc((V + 1) * sizeof(int));
  dp[0] = 0;                              /* base case: 0 coins for amount 0 */
  for (int v = 1; v <= V; v++) {
    dp[v] = INT_MAX;                      /* INT_MAX = "unreachable"        */
    for (int i = 0; i < n; i++)
      if (c[i] > 0 && c[i] <= v && dp[v - c[i]] != INT_MAX && dp[v - c[i]] + 1 < dp[v])
        dp[v] = dp[v - c[i]] + 1;
  }
  printf("%d\n", dp[V] == INT_MAX ? -1 : dp[V]);
  free(c); free(dp);
  return 0;
}
