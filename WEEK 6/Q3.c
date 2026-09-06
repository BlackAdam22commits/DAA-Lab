/* ============================================================
   DAA Lab-06, Q3: Convolution of two vectors in O(n log n)
   ============================================================
   C[k] = sum_{j=0}^{m-1} A[j] * B[k-j],   len(A)=m, len(B)=n, n>=m
   Result C has length m + n - 1.

   Direct evaluation of the sum is O(m*n) = O(n^2) in the worst
   case (m = n). We instead view A and B as coefficient vectors of
   polynomials; convolution IS polynomial multiplication. The
   standard O(n log n) divide-and-conquer algorithm for polynomial
   multiplication is the FFT (Cooley-Tukey), which:
     1) evaluates both polynomials at the n-th roots of unity
        using a recursive divide-and-conquer (splitting into
        even/odd coefficients) -- T(n) = 2T(n/2) + O(n) = O(n log n)
     2) multiplies the point-wise values -- O(n)
     3) interpolates back via the inverse FFT (same D&C structure)
        -- O(n log n)
   Total: O(n log n).

   Input representation: vectors A, B given as arrays of doubles
   (real coefficients); internally we work with an array of
   `complex_t` (custom struct, since we avoid relying on a
   particular <complex.h> ABI) padded to the next power of two
   length >= m+n-1.
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct { double re, im; } complex_t;

static complex_t c_add(complex_t a, complex_t b) { return (complex_t){a.re + b.re, a.im + b.im}; }
static complex_t c_sub(complex_t a, complex_t b) { return (complex_t){a.re - b.re, a.im - b.im}; }
static complex_t c_mul(complex_t a, complex_t b) {
    return (complex_t){ a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re };
}

/* ---------- Recursive divide-and-conquer FFT (Cooley-Tukey) ----------
   T(n) = 2T(n/2) + O(n)  =>  O(n log n) by the master theorem
   (a=2, b=2, f(n)=O(n) => case 2 => O(n log n)).
   `invert` = 1 computes the inverse transform (without the final
   1/n scaling, which the caller applies once). */
void fft(complex_t *a, int n, int invert) {
    if (n == 1) return;

    complex_t *even = malloc((n / 2) * sizeof(complex_t));
    complex_t *odd  = malloc((n / 2) * sizeof(complex_t));
    for (int i = 0; i < n / 2; i++) {
        even[i] = a[2 * i];
        odd[i]  = a[2 * i + 1];
    }

    fft(even, n / 2, invert);
    fft(odd, n / 2, invert);

    double angle = 2 * M_PI / n * (invert ? -1 : 1);
    complex_t w = {1.0, 0.0};
    complex_t wn = {cos(angle), sin(angle)};

    for (int i = 0; i < n / 2; i++) {
        complex_t t = c_mul(w, odd[i]);
        a[i]           = c_add(even[i], t);
        a[i + n / 2]   = c_sub(even[i], t);
        w = c_mul(w, wn);
    }
    free(even);
    free(odd);
}

int next_pow2(int x) {
    int p = 1;
    while (p < x) p <<= 1;
    return p;
}

/* ---------- Convolution via FFT: O(n log n) ---------- */
double *convolve(double *A, int m, double *B, int n, int *out_len) {
    int result_len = m + n - 1;
    int size = next_pow2(result_len);

    complex_t *fa = calloc(size, sizeof(complex_t));
    complex_t *fb = calloc(size, sizeof(complex_t));
    for (int i = 0; i < m; i++) fa[i].re = A[i];
    for (int i = 0; i < n; i++) fb[i].re = B[i];

    fft(fa, size, 0);
    fft(fb, size, 0);

    for (int i = 0; i < size; i++) fa[i] = c_mul(fa[i], fb[i]); /* O(n) */

    fft(fa, size, 1); /* inverse FFT */

    double *C = malloc(result_len * sizeof(double));
    for (int i = 0; i < result_len; i++)
        C[i] = fa[i].re / size; /* undo the 1/n scaling here */

    free(fa); free(fb);
    *out_len = result_len;
    return C;
}

/* ---------- Naive O(m*n) convolution, used only to validate ---------- */
double *convolve_naive(double *A, int m, double *B, int n, int *out_len) {
    int result_len = m + n - 1;
    double *C = calloc(result_len, sizeof(double));
    for (int j = 0; j < m; j++)
        for (int k = 0; k < n; k++)
            C[j + k] += A[j] * B[k];
    *out_len = result_len;
    return C;
}

void print_vec(const char *label, double *v, int len) {
    printf("%s: [", label);
    for (int i = 0; i < len; i++) printf("%.2f%s", v[i], i == len - 1 ? "" : ", ");
    printf("]\n");
}

int main(void) {
    /* small correctness demo */
    double A[] = {1, 2, 3};        /* m = 3 */
    double B[] = {0, 1, 0.5, 2};   /* n = 4 */
    int m = 3, n = 4;

    int len1, len2;
    double *c_fft   = convolve(A, m, B, n, &len1);
    double *c_naive = convolve_naive(A, m, B, n, &len2);

    print_vec("A", A, m);
    print_vec("B", B, n);
    print_vec("C (FFT, O(n log n))", c_fft, len1);
    print_vec("C (naive, O(m*n), for validation)", c_naive, len2);

    double max_err = 0;
    for (int i = 0; i < len1; i++) {
        double e = fabs(c_fft[i] - c_naive[i]);
        if (e > max_err) max_err = e;
    }
    printf("Max abs error between FFT and naive result: %.2e\n", max_err);

    free(c_fft); free(c_naive);

    /* ---------------- empirical timing ---------------- */
    printf("\n--- Empirical timing (FFT grows ~n log n, naive grows ~n^2) ---\n");
    srand((unsigned) time(NULL));
    for (int size = 1024; size <= 8192; size *= 2) {
        double *X = malloc(size * sizeof(double));
        double *Y = malloc(size * sizeof(double));
        for (int i = 0; i < size; i++) { X[i] = rand() % 10; Y[i] = rand() % 10; }

        int lf, ln;
        clock_t t0 = clock();
        double *cf = convolve(X, size, Y, size, &lf);
        clock_t t1 = clock();
        double *cn = convolve_naive(X, size, Y, size, &ln);
        clock_t t2 = clock();

        printf("n=%5d  FFT: %.6fs   naive: %.6fs\n",
               size, (double)(t1 - t0) / CLOCKS_PER_SEC, (double)(t2 - t1) / CLOCKS_PER_SEC);

        free(X); free(Y); free(cf); free(cn);
    }
    return 0;
}
