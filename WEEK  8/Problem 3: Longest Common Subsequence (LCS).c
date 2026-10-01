/*
 * DAA Lab-08, Problem 3: Longest Common Subsequence (LCS)
 * Author: Ayush Pattanaik (B525018)
 * Time Theta(mn), Space Theta(mn)
 * Compile: gcc -Wall -O2 -o q3_lcs q3_lcs.c
 * Run:     ./q3_lcs < q3_lcs_sample.txt
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(void) {
  char X[1001], Y[1001];
  if (scanf("%1000s %1000s", X, Y) != 2) return 1;
  int m = strlen(X), n = strlen(Y), W = n + 1;
  int *L = calloc((m + 1) * W, sizeof(int));        /* row 0 / col 0 = 0 */
  for (int i = 1; i <= m; i++)
    for (int j = 1; j <= n; j++)
      L[i*W + j] = (X[i-1] == Y[j-1]) ? L[(i-1)*W + j-1] + 1
                 : (L[(i-1)*W + j] >= L[i*W + j-1] ? L[(i-1)*W + j] : L[i*W + j-1]);
  int len = L[m*W + n];
  char *s = malloc(len + 1); s[len] = '\0';
  for (int i = m, j = n, k = len - 1; i > 0 && j > 0; ) {   /* traceback */
    if (X[i-1] == Y[j-1]) { s[k--] = X[i-1]; i--; j--; }
    else if (L[(i-1)*W + j] >= L[i*W + j-1]) i--;           /* tie: go up */
    else j--;
  }
  printf("LCS length = %d\nLCS        = %s\n", len, s);
  free(L); free(s);
  return 0;
}
