/*
 * Q5: Multiply special-pattern square matrices using D&C - O(n^2)
 * ---------------------------------------------------------------
 * A "special" n x n matrix (n = 2^k) has the recursive block form
 *
 *        M = [ M1  M2 ]
 *            [ M2  M1 ]
 *
 * where M1 and M2 are themselves (n/2)x(n/2) special matrices of the
 * exact same form, all the way down to single scalars at n = 1.
 *
 * KEY FACT: such a matrix is completely determined by its FIRST ROW.
 * Concretely, if a[0..n-1] is the first row of M, then for any i, j:
 *
 *        M[i][j] = a[ i XOR j ]
 *
 * (an easy induction on the recursive block definition shows this -
 * flipping the top bit of i or j swaps between the M1 and M2 block,
 * exactly matching how "i XOR j" changes when the top bit of i or j
 * flips). So we can represent the WHOLE n x n matrix compactly using
 * only its n-length defining array "a".
 *
 * PRODUCT OF TWO SUCH MATRICES IS ALSO SPECIAL:
 * If A[i][j] = a[i^j] and B[i][j] = b[i^j], then
 *   C[i][j] = sum_k A[i][k]*B[k][j] = sum_k a[i^k] * b[k^j]
 * substitute m = i^k (so k = i^m, and k^j = i^m^j):
 *   C[i][j] = sum_m a[m] * b[(i^j)^m] = (a XOR-conv b) [i^j]
 * so C is special too, with defining array  c = a (xor-conv) b.
 *
 * DIVIDE AND CONQUER for the XOR-convolution c = a (xor-conv) b:
 * split a = (a1,a2), b = (b1,b2) (each length n/2, a1/a2 are simply
 * the defining arrays of blocks M1/M2). Then it is straightforward to
 * show:
 *     c1 = (a1 xorconv b1) + (a2 xorconv b2)
 *     c2 = (a1 xorconv b2) + (a2 xorconv b1)
 *     c  = (c1, c2)
 * giving the recurrence   T(n) = 4*T(n/2) + O(n)   (4 half-size
 * XOR-convolutions + O(n) additions), which solves by the Master
 * theorem to T(n) = Theta(n^2) -- exactly the required bound, and MUCH
 * better than the Theta(n^3) a naive full matrix multiply would cost.
 *
 * This program:
 *   1. builds special matrices from a random defining array,
 *   2. multiplies them with the O(n^2) D&C xor-convolution algorithm,
 *   3. expands both inputs to full n x n matrices and multiplies them
 *      the standard O(n^3) way, purely to VALIDATE correctness,
 *   4. times both approaches to show the O(n^2) method scaling better.
 *
 * Compile: gcc -O2 -Wall -o special_matrix special_matrix.c
 * Run:     ./special_matrix
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef long long ll;

/* xor-convolution via divide & conquer: c[0..n-1] = a xorconv b
   requires n to be a power of two. Runs in Theta(n^2). */
void xorConvolveDC(ll *a, ll *b, ll *c, int n) {
    if (n == 1) {
        c[0] = a[0] * b[0];
        return;
    }
    int h = n / 2;
    ll *a1 = a, *a2 = a + h;
    ll *b1 = b, *b2 = b + h;
    ll *c1 = c, *c2 = c + h;

    ll *t1 = malloc(h * sizeof(ll)); /* a1 xorconv b1 */
    ll *t2 = malloc(h * sizeof(ll)); /* a2 xorconv b2 */
    ll *t3 = malloc(h * sizeof(ll)); /* a1 xorconv b2 */
    ll *t4 = malloc(h * sizeof(ll)); /* a2 xorconv b1 */

    xorConvolveDC(a1, b1, t1, h);
    xorConvolveDC(a2, b2, t2, h);
    xorConvolveDC(a1, b2, t3, h);
    xorConvolveDC(a2, b1, t4, h);

    for (int i = 0; i < h; i++) {
        c1[i] = t1[i] + t2[i];
        c2[i] = t3[i] + t4[i];
    }
    free(t1); free(t2); free(t3); free(t4);
}

/* Multiply two special matrices, given by their defining arrays a, b
   (length n), producing the defining array c (length n) of the
   product. This is the O(n^2) D&C algorithm required by the question. */
void multiplySpecial(ll *a, ll *b, ll *c, int n) {
    xorConvolveDC(a, b, c, n);
}

/* --------- helpers to build & verify against a full matrix --------- */

/* expand a defining array (length n) into a full n x n matrix using
   M[i][j] = a[i XOR j] */
ll **expandToFullMatrix(ll *a, int n) {
    ll **M = malloc(n * sizeof(ll *));
    for (int i = 0; i < n; i++) {
        M[i] = malloc(n * sizeof(ll));
        for (int j = 0; j < n; j++)
            M[i][j] = a[i ^ j];
    }
    return M;
}

/* check that a full matrix genuinely has the recursive
   [[M1,M2],[M2,M1]] pattern at every level (sanity check on our
   "expand" construction) */
int checkPattern(ll **M, int n) {
    if (n == 1) return 1;
    int h = n / 2;
    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++) {
            if (M[i][j] != M[i + h][j + h]) return 0;             /* diag blocks equal */
            if (M[i][j + h] != M[i + h][j]) return 0;             /* off-diag blocks equal */
        }
    /* recursively check M1 (top-left) and M2 (top-right) */
    ll **M1 = malloc(h * sizeof(ll *));
    ll **M2 = malloc(h * sizeof(ll *));
    for (int i = 0; i < h; i++) {
        M1[i] = malloc(h * sizeof(ll));
        M2[i] = malloc(h * sizeof(ll));
        for (int j = 0; j < h; j++) { M1[i][j] = M[i][j]; M2[i][j] = M[i][j + h]; }
    }
    int ok = checkPattern(M1, h) && checkPattern(M2, h);
    for (int i = 0; i < h; i++) { free(M1[i]); free(M2[i]); }
    free(M1); free(M2);
    return ok;
}

ll **naiveMultiplyFull(ll **A, ll **B, int n) {
    ll **C = malloc(n * sizeof(ll *));
    for (int i = 0; i < n; i++) {
        C[i] = calloc(n, sizeof(ll));
        for (int k = 0; k < n; k++) {
            ll a = A[i][k];
            for (int j = 0; j < n; j++)
                C[i][j] += a * B[k][j];
        }
    }
    return C;
}

void freeFull(ll **M, int n) { for (int i = 0; i < n; i++) free(M[i]); free(M); }

void printArr(ll *a, int n) {
    for (int i = 0; i < n; i++) printf("%4lld ", a[i]);
    printf("\n");
}

int main(void) {
    printf("=========================================================\n");
    printf(" Q5: O(n^2) D&C Multiplication of Special-Pattern Matrices\n");
    printf("=========================================================\n\n");
    srand(3);

    /* ---- small demo, n = 8 ---- */
    int n = 8;
    ll a[8], b[8];
    for (int i = 0; i < n; i++) { a[i] = rand() % 9; b[i] = rand() % 9; }

    printf("Defining array a (first row of A): "); printArr(a, n);
    printf("Defining array b (first row of B): "); printArr(b, n);

    ll **A = expandToFullMatrix(a, n);
    ll **B = expandToFullMatrix(b, n);
    printf("\nFull matrix A:\n");
    for (int i = 0; i < n; i++) printArr(A[i], n);
    printf("\nFull matrix B:\n");
    for (int i = 0; i < n; i++) printArr(B[i], n);

    printf("\nPattern check A: %s, B: %s\n",
           checkPattern(A, n) ? "valid special pattern" : "INVALID",
           checkPattern(B, n) ? "valid special pattern" : "INVALID");

    ll c[8];
    multiplySpecial(a, b, c, n);
    ll **C_fast = expandToFullMatrix(c, n);
    ll **C_naive = naiveMultiplyFull(A, B, n);

    printf("\nProduct C = A x B via O(n^2) D&C xor-convolution:\n");
    for (int i = 0; i < n; i++) printArr(C_fast[i], n);
    printf("\nProduct C = A x B via naive O(n^3) full multiply (verification):\n");
    for (int i = 0; i < n; i++) printArr(C_naive[i], n);

    int mismatch = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (C_fast[i][j] != C_naive[i][j]) mismatch = 1;
    printf("\nVerification: %s\n\n", mismatch ? "*** MISMATCH! ***" : "OK, results match.");
    printf("Pattern check on result C: %s\n\n",
           checkPattern(C_fast, n) ? "valid special pattern (as proven)" : "INVALID");

    freeFull(A, n); freeFull(B, n); freeFull(C_fast, n); freeFull(C_naive, n);

    /* ---- timing: O(n^2) special method vs O(n^3) naive full method ---- */
    printf("Timing: O(n^2) D&C (on defining arrays) vs O(n^3) naive (on full matrices)\n");
    printf("%8s %18s %18s\n", "n", "O(n^2) time (s)", "O(n^3) time (s)");
    int sizes[] = {8, 16, 32, 64, 128, 256, 512, 1024};
    for (size_t s = 0; s < sizeof(sizes) / sizeof(sizes[0]); s++) {
        int sz = sizes[s];
        ll *aa = malloc(sz * sizeof(ll));
        ll *bb = malloc(sz * sizeof(ll));
        ll *cc = malloc(sz * sizeof(ll));
        for (int i = 0; i < sz; i++) { aa[i] = rand() % 9; bb[i] = rand() % 9; }

        clock_t t0 = clock();
        multiplySpecial(aa, bb, cc, sz);
        double fastTime = (double)(clock() - t0) / CLOCKS_PER_SEC;

        double naiveTime = -1;
        if (sz <= 256) { /* skip huge O(n^3) runs to keep this fast */
            ll **AA = expandToFullMatrix(aa, sz);
            ll **BB = expandToFullMatrix(bb, sz);
            t0 = clock();
            ll **CC = naiveMultiplyFull(AA, BB, sz);
            naiveTime = (double)(clock() - t0) / CLOCKS_PER_SEC;
            freeFull(AA, sz); freeFull(BB, sz); freeFull(CC, sz);
        }

        if (naiveTime >= 0)
            printf("%8d %18.5f %18.5f\n", sz, fastTime, naiveTime);
        else
            printf("%8d %18.5f %18s\n", sz, fastTime, "(skipped)");

        free(aa); free(bb); free(cc);
    }

    printf("\nObservation: the O(n^2) D&C algorithm (built on the xor-\n");
    printf("convolution recurrence T(n)=4T(n/2)+O(n)) stays fast even at\n");
    printf("n=1024, while the naive O(n^3) full-matrix approach already\n");
    printf("becomes noticeably slower by n=256.\n");
    return 0;
}

