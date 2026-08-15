#include <stdio.h>
#include <stdlib.h>

/*Standard block multiplication needs 8 recursive multiplications: T(n) = 8T(n/2) + O(n²) → O(n³), no improvement. Strassen's trick reformulates the 4 output blocks using only 7 cleverly-combined products (M1..M7) instead of 8:

T(n) = 7T(n/2) + O(n²) → O(n^log₂7) ≈ O(n^2.807)*/

int **alloc(int n)
{
  int **m = malloc(n * sizeof(int *));
  for (int i = 0; i < n; i++)
    m[i] = calloc(n, sizeof(int));
  return m;
}
void freeMat(int **m, int n)
{
  for (int i = 0; i < n; i++)
    free(m[i]);
  free(m);
}
void add(int **A, int **B, int **C, int n)
{
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      C[i][j] = A[i][j] + B[i][j];
}
void sub(int **A, int **B, int **C, int n)
{
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      C[i][j] = A[i][j] - B[i][j];
}

void bruteMultiply(int **A, int **B, int **C, int n)
{
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
    {
      int s = 0;
      for (int k = 0; k < n; k++)
        s += A[i][k] * B[k][j];
      C[i][j] = s;
    }
}

void strassen(int **A, int **B, int **C, int n)
{
  if (n <= 64)
  {
    bruteMultiply(A, B, C, n);
    return;
  } // base case for speed
  int h = n / 2;
  int **A11 = alloc(h), **A12 = alloc(h), **A21 = alloc(h), **A22 = alloc(h);
  int **B11 = alloc(h), **B12 = alloc(h), **B21 = alloc(h), **B22 = alloc(h);
  for (int i = 0; i < h; i++)
    for (int j = 0; j < h; j++)
    {
      A11[i][j] = A[i][j];
      A12[i][j] = A[i][j + h];
      A21[i][j] = A[i + h][j];
      A22[i][j] = A[i + h][j + h];
      B11[i][j] = B[i][j];
      B12[i][j] = B[i][j + h];
      B21[i][j] = B[i + h][j];
      B22[i][j] = B[i + h][j + h];
    }
  int **T1 = alloc(h), **T2 = alloc(h);
  int **M1 = alloc(h), **M2 = alloc(h), **M3 = alloc(h), **M4 = alloc(h), **M5 = alloc(h), **M6 = alloc(h), **M7 = alloc(h);

  add(A11, A22, T1, h);
  add(B11, B22, T2, h);
  strassen(T1, T2, M1, h);
  add(A21, A22, T1, h);
  strassen(T1, B11, M2, h);
  sub(B12, B22, T2, h);
  strassen(A11, T2, M3, h);
  sub(B21, B11, T2, h);
  strassen(A22, T2, M4, h);
  add(A11, A12, T1, h);
  strassen(T1, B22, M5, h);
  sub(A21, A11, T1, h);
  add(B11, B12, T2, h);
  strassen(T1, T2, M6, h);
  sub(A12, A22, T1, h);
  add(B21, B22, T2, h);
  strassen(T1, T2, M7, h);

  for (int i = 0; i < h; i++)
    for (int j = 0; j < h; j++)
    {
      C[i][j] = M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j];
      C[i][j + h] = M3[i][j] + M5[i][j];
      C[i + h][j] = M2[i][j] + M4[i][j];
      C[i + h][j + h] = M1[i][j] - M2[i][j] + M3[i][j] + M6[i][j];
    }
  int **mats[] = {A11, A12, A21, A22, B11, B12, B21, B22, T1, T2, M1, M2, M3, M4, M5, M6, M7};
  for (int i = 0; i < 17; i++)
    freeMat(mats[i], h);
}

int main(void)
{
  int n = 256;
  int **A = alloc(n), **B = alloc(n), **C = alloc(n), **Ref = alloc(n);
  srand(7);
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
    {
      A[i][j] = rand() % 10;
      B[i][j] = rand() % 10;
    }
  strassen(A, B, C, n);
  bruteMultiply(A, B, Ref, n);
  int ok = 1;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      if (C[i][j] != Ref[i][j])
        ok = 0;
  printf("Matches brute-force result: %s\n", ok ? "YES" : "NO");
  return 0;
}
