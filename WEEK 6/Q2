/* ============================================================
   DAA Lab-06, Q2: 2D square matrix operations and complexities
   ============================================================
   Input representation: n x n matrix stored as a contiguous
   1-D block accessed via a helper macro IDX(i,j,n) = i*n+j
   (row-major), allocated with malloc. This avoids the overhead
   of int** (array of pointers) and keeps memory contiguous,
   which is friendlier for cache performance during O(n^3) work.
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define IDX(i, j, n) ((i) * (n) + (j))

double *alloc_matrix(int n) {
    return malloc((size_t) n * n * sizeof(double));
}

void fill_random(double *M, int n) {
    for (int i = 0; i < n * n; i++)
        M[i] = (rand() % 20) - 10; /* small integers for readability */
}

void print_matrix(const char *label, double *M, int n) {
    printf("%s:\n", label);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%6.1f ", M[IDX(i, j, n)]);
        printf("\n");
    }
}

/* ---------- (i) Matrix Addition ----------
   Worst case: O(n^2) -- every one of the n^2 entries is added once. */
double *matrix_add(double *A, double *B, int n) {
    double *C = alloc_matrix(n);
    for (int i = 0; i < n * n; i++) C[i] = A[i] + B[i];
    return C;
}

/* ---------- (ii) Matrix Multiplication ----------
   Worst case: O(n^3) with the standard triple-loop algorithm.
   (Strassen's divide-and-conquer method achieves O(n^2.807);
   asymptotically faster methods exist but are impractical for
   the sizes used here, so the classic O(n^3) algorithm is used.) */
double *matrix_multiply(double *A, double *B, int n) {
    double *C = calloc((size_t) n * n, sizeof(double));
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++) {
            double a_ik = A[IDX(i, k, n)];
            if (a_ik == 0) continue;
            for (int j = 0; j < n; j++)
                C[IDX(i, j, n)] += a_ik * B[IDX(k, j, n)];
        }
    return C;
}

/* ---------- (iii) Is zero matrix? ----------
   Worst case: O(n^2) -- must inspect every entry (adversary can
   put the only non-zero value in the last cell checked). */
int is_zero_matrix(double *M, int n) {
    for (int i = 0; i < n * n; i++)
        if (M[i] != 0.0) return 0;
    return 1;
}

/* ---------- (iv) Is symmetric? ----------
   Worst case: O(n^2) -- compare M[i][j] with M[j][i] for all
   pairs (only need the upper triangle, still Theta(n^2/2) = O(n^2)). */
int is_symmetric(double *M, int n) {
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (M[IDX(i, j, n)] != M[IDX(j, i, n)]) return 0;
    return 1;
}

/* ---------- (v) Determinant ----------
   Worst case: O(n^3) using Gaussian elimination (LU-style) with
   partial pivoting; the product of the pivots (with sign flips
   for row swaps) gives the determinant.
   (Naive cofactor/Laplace expansion is O(n!) and must be avoided.) */
double determinant(double *M, int n) {
    double *A = alloc_matrix(n);
    for (int i = 0; i < n * n; i++) A[i] = M[i];

    double det = 1.0;
    for (int col = 0; col < n; col++) {
        /* partial pivoting */
        int pivot_row = col;
        double best = fabs(A[IDX(col, col, n)]);
        for (int r = col + 1; r < n; r++) {
            double v = fabs(A[IDX(r, col, n)]);
            if (v > best) { best = v; pivot_row = r; }
        }
        if (best == 0.0) { free(A); return 0.0; } /* singular */

        if (pivot_row != col) {
            for (int j = 0; j < n; j++) {
                double t = A[IDX(col, j, n)];
                A[IDX(col, j, n)] = A[IDX(pivot_row, j, n)];
                A[IDX(pivot_row, j, n)] = t;
            }
            det = -det;
        }

        double pivot_val = A[IDX(col, col, n)];
        det *= pivot_val;

        for (int r = col + 1; r < n; r++) {
            double factor = A[IDX(r, col, n)] / pivot_val;
            for (int j = col; j < n; j++)
                A[IDX(r, j, n)] -= factor * A[IDX(col, j, n)];
        }
    }
    free(A);
    return det;
}

/* ---------- (vi) Transpose in place ----------
   Worst case: O(n^2) -- swap each (i,j)/(j,i) pair once,
   n(n-1)/2 swaps total => O(n^2). Only possible in-place because
   the matrix is square. */
void transpose_in_place(double *M, int n) {
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) {
            double t = M[IDX(i, j, n)];
            M[IDX(i, j, n)] = M[IDX(j, i, n)];
            M[IDX(j, i, n)] = t;
        }
}

/* ---------- (vii) Dominant eigenvalue / eigenvector ----------
   Worst case: O(k * n^2) where k is the number of power-iteration
   steps until convergence -- each iteration is one matrix-vector
   product (O(n^2)) followed by O(n) normalization.
   (Finding ALL eigenvalues exactly, e.g. via the QR algorithm,
   costs O(n^3) per iteration and is not required here; the power
   method gives the dominant eigenvalue/eigenvector, which is the
   standard approach for large matrices.) */
double power_iteration(double *M, int n, double *eigenvector, int max_iter, double tol) {
    double *v = malloc(n * sizeof(double));
    double *v_new = malloc(n * sizeof(double));
    for (int i = 0; i < n; i++) v[i] = 1.0; /* initial guess */

    double eigenvalue = 0.0;
    for (int iter = 0; iter < max_iter; iter++) {
        /* v_new = M * v  -- O(n^2) */
        for (int i = 0; i < n; i++) {
            double sum = 0.0;
            for (int j = 0; j < n; j++)
                sum += M[IDX(i, j, n)] * v[j];
            v_new[i] = sum;
        }
        /* normalize -- O(n) */
        double norm = 0.0;
        for (int i = 0; i < n; i++) norm += v_new[i] * v_new[i];
        norm = sqrt(norm);
        if (norm < 1e-15) break;
        for (int i = 0; i < n; i++) v_new[i] /= norm;

        /* Rayleigh quotient estimate of eigenvalue -- O(n^2) */
        double num = 0.0, den = 0.0;
        for (int i = 0; i < n; i++) {
            double mv = 0.0;
            for (int j = 0; j < n; j++) mv += M[IDX(i, j, n)] * v_new[j];
            num += v_new[i] * mv;
            den += v_new[i] * v_new[i];
        }
        double new_eigenvalue = num / den;

        double diff = fabs(new_eigenvalue - eigenvalue);
        eigenvalue = new_eigenvalue;
        for (int i = 0; i < n; i++) v[i] = v_new[i];

        if (diff < tol) break;
    }
    for (int i = 0; i < n; i++) eigenvector[i] = v[i];
    free(v); free(v_new);
    return eigenvalue;
}

int main(void) {
    srand((unsigned) time(NULL));
    int n = 4;

    double *A = alloc_matrix(n);
    double *B = alloc_matrix(n);
    fill_random(A, n);
    fill_random(B, n);

    print_matrix("Matrix A", A, n);
    print_matrix("Matrix B", B, n);

    double *sum = matrix_add(A, B, n);
    print_matrix("(i) A + B", sum, n);

    double *prod = matrix_multiply(A, B, n);
    print_matrix("(ii) A x B", prod, n);

    printf("(iii) A is zero matrix? %s\n", is_zero_matrix(A, n) ? "Yes" : "No");
    printf("(iv)  A is symmetric?   %s\n", is_symmetric(A, n) ? "Yes" : "No");

    /* build a symmetric matrix to show a positive example, and to feed
       the power-iteration eigen solver a nicer (symmetric -> real
       eigenvalues) matrix */
    double *S = alloc_matrix(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            S[IDX(i, j, n)] = A[IDX(i, j, n)] + A[IDX(j, i, n)]; /* A + A^T is symmetric */
    printf("(iv)  (A + A^T) is symmetric? %s\n", is_symmetric(S, n) ? "Yes" : "No");

    printf("(v)   det(A) = %.4f\n", determinant(A, n));

    double *T = alloc_matrix(n);
    for (int i = 0; i < n * n; i++) T[i] = A[i];
    transpose_in_place(T, n);
    print_matrix("(vi) A transposed in place", T, n);

    double eigenvector[4];
    double eigenvalue = power_iteration(S, n, eigenvector, 1000, 1e-9);
    printf("(vii) Dominant eigenvalue of (A+A^T) ~= %.4f\n", eigenvalue);
    printf("(vii) Corresponding eigenvector ~= [");
    for (int i = 0; i < n; i++) printf("%.4f%s", eigenvector[i], i == n - 1 ? "" : ", ");
    printf("]\n");

    /* ---------------- empirical timing ---------------- */
    printf("\n--- Empirical timing (grows as expected with n) ---\n");
    for (int size = 50; size <= 200; size *= 2) {
        double *X = alloc_matrix(size);
        double *Y = alloc_matrix(size);
        fill_random(X, size);
        fill_random(Y, size);

        clock_t t0 = clock();
        double *Z = matrix_multiply(X, Y, size);
        clock_t t1 = clock();
        printf("n=%4d  multiply: %.6fs (O(n^3))\n", size, (double)(t1 - t0) / CLOCKS_PER_SEC);

        free(X); free(Y); free(Z);
    }

    free(A); free(B); free(sum); free(prod); free(S); free(T);
    return 0;
}
