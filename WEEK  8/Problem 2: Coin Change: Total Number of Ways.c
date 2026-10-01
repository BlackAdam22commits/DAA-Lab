/*
 * DAA Lab-08, Problem 2: Coin Change: Total Number of Ways
 * Author: Ayush Pattanaik (B525018)
 * Time Theta(nV), Space Theta(V)
 * Compile: gcc -Wall -O2 -o q2_coin_change_ways q2_coin_change_ways.c
 * Run:     ./q2_coin_change_ways < q2_coin_change_ways_sample.txt
 */
#include <stdio.h>
#include <stdlib.h>
int main(void) {
  int n, V;
  if (scanf("%d %d", &n, &V) != 2 || n < 1 || V < 0) return 1;
  int *c = malloc(n * sizeof(int));
  for (int i = 0; i < n; i++) scanf("%d", &c[i]);
  unsigned long long *ways = calloc(V + 1, sizeof(unsigned long long));
  ways[0] = 1;                            /* one way to make 0: take nothing */
  for (int i = 0; i < n; i++)             /* coin in OUTER loop => combinations */
    for (int v = c[i]; v <= V; v++)
      ways[v] += ways[v - c[i]];
  printf("%llu\n", ways[V]);
  free(c); free(ways);
  return 0;
}
