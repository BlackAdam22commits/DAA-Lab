/* ============================================================
   Problem 7: Matrix Chain Multiplication (MCM)
   ============================================================
   INPUT REPRESENTATION
   ---------------------
   n matrices A1..An are represented compactly by an array
   dims[0..n] of n+1 integers, where matrix Ai has dimensions
   dims[i-1] x dims[i]. This is the standard compact encoding
   (avoids storing n separate (rows,cols) pairs, since consecutive
   matrices must share the inner dimension anyway).

   ALGORITHM (dynamic programming over chain intervals)
   -------------------------------------------------------
   Let m[i][j] = minimum number of scalar multiplications needed to
   compute the product Ai * A(i+1) * ... * Aj.
      m[i][i] = 0
      m[i][j] = min over k in [i, j-1] of
                m[i][k] + m[k+1][j] + dims[i-1]*dims[k]*dims[j]
   We fill the table by increasing chain length L = 2..n, and also
   record s[i][j] = the split point k that achieves the minimum, so
   that the optimal parenthesization can be reconstructed afterward.

   COMPLEXITY
   ----------
   There are O(n^2) sub-problems (i,j pairs); each takes O(n) time
   to try every split k -> total O(n^3) time, O(n^2) space. This is
   the well known, optimal (for this DP formulation) complexity for
   MCM; the naive fully-parenthesized brute force would cost
   Omega(4^n / n^1.5) (the n-th Catalan number), so the DP is an
   exponential-to-polynomial improvement.
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void printOptimalParens(int **s, int i, int j, char *label) {
    if (i == j) {
        printf("A%d", i);
    } else {
        printf("(");
        printOptimalParens(s, i, s[i][j], label);
        printOptimalParens(s, s[i][j] + 1, j, label);
        printf(")");
    }
}

int main(void) {
    int n;
    printf("Enter number of matrices (n): ");
    if (scanf("%d", &n) != 1 || n <= 0) { printf("Invalid input\n"); return 1; }

    int *dims = malloc((n + 1) * sizeof(int));
    printf("Enter the %d dimensions p0 p1 ... p%d (matrix Ai is p(i-1) x p(i)):\n", n + 1, n);
    for (int i = 0; i <= n; i++) {
        if (scanf("%d", &dims[i]) != 1 || dims[i] <= 0) { printf("Invalid input\n"); return 1; }
    }

    long long **m = malloc((n + 1) * sizeof(long long *));
    int **s = malloc((n + 1) * sizeof(int *));
    for (int i = 0; i <= n; i++) {
        m[i] = calloc(n + 1, sizeof(long long));
        s[i] = calloc(n + 1, sizeof(int));
    }

    for (int len = 2; len <= n; len++) {
        for (int i = 1; i <= n - len + 1; i++) {
            int j = i + len - 1;
            m[i][j] = LLONG_MAX;
            for (int k = i; k < j; k++) {
                long long cost = m[i][k] + m[k + 1][j] +
                                  (long long)dims[i - 1] * dims[k] * dims[j];
                if (cost < m[i][j]) {
                    m[i][j] = cost;
                    s[i][j] = k;
                }
            }
        }
    }

    printf("\nMinimum number of scalar multiplications = %lld\n", m[1][n]);
    printf("Optimal parenthesization: ");
    printOptimalParens(s, 1, n, NULL);
    printf("\n");

    if (n <= 12) {
        printf("\nDP cost table m[i][j] (0 = not used):\n     ");
        for (int j = 1; j <= n; j++) printf("%8d", j);
        printf("\n");
        for (int i = 1; i <= n; i++) {
            printf("i=%-3d", i);
            for (int j = 1; j <= n; j++) {
                if (j >= i) printf("%8lld", m[i][j]); else printf("%8s", "-");
            }
            printf("\n");
        }
    }

    for (int i = 0; i <= n; i++) { free(m[i]); free(s[i]); }
    free(m); free(s); free(dims);
    return 0;
}
