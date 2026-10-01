/*
 * DAA Lab-08, Problem 6: Edit Distance with Traceback
 * Author: Ayush Pattanaik (B525018)
 * Time Theta(mn), Space Theta(mn)
 * Compile: gcc -Wall -O2 -o q6_edit_distance q6_edit_distance.c
 * Run:     ./q6_edit_distance < q6_edit_distance_sample.txt
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int min3(int a, int b, int c) { int m = a < b ? a : b; return m < c ? m : c; }
int main(void) {
  char A[1001], B[1001];
  if (scanf("%1000s %1000s", A, B) != 2) return 1;
  int m = strlen(A), n = strlen(B), W = n + 1;
  int *D = malloc((m + 1) * W * sizeof(int));
  for (int i = 0; i <= m; i++) D[i*W] = i;           /* delete i characters */
  for (int j = 0; j <= n; j++) D[j] = j;             /* insert j characters */
  for (int i = 1; i <= m; i++)
    for (int j = 1; j <= n; j++)
      D[i*W + j] = min3(D[(i-1)*W + j] + 1,          /* delete A[i]         */
                        D[i*W + j-1] + 1,            /* insert B[j]         */
                        D[(i-1)*W + j-1] + (A[i-1] != B[j-1]));  /* match/sub */
  printf("Edit distance = %d\nEdit script (A -> B):\n", D[m*W + n]);
  char *op = malloc(m + n + 1); int *oi = malloc((m+n+1)*sizeof(int)), *oj = malloc((m+n+1)*sizeof(int)), k = 0;
  for (int i = m, j = n; i > 0 || j > 0; ) {         /* traceback from (m,n) */
    int d = D[i*W + j];
    if (i && j && A[i-1] == B[j-1] && d == D[(i-1)*W + j-1])     op[k] = 'M', oi[k] = --i, oj[k] = --j;
    else if (i && j && d == D[(i-1)*W + j-1] + 1)                op[k] = 'S', oi[k] = --i, oj[k] = --j;
    else if (i && d == D[(i-1)*W + j] + 1)                       op[k] = 'D', oi[k] = --i, oj[k] = j;
    else                                                         op[k] = 'I', oi[k] = i, oj[k] = --j;
    k++;
  }
  for (int t = k - 1; t >= 0; t--) {                 /* print in forward order */
    if (op[t] == 'M') printf("  Match      %c\n", A[oi[t]]);
    if (op[t] == 'S') printf("  Substitute %c -> %c\n", A[oi[t]], B[oj[t]]);
    if (op[t] == 'D') printf("  Delete     %c\n", A[oi[t]]);
    if (op[t] == 'I') printf("  Insert     %c\n", B[oj[t]]);
  }
  free(D); free(op); free(oi); free(oj);
  return 0;
}
