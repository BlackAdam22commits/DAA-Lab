# Q4 — Matrix Multiplication using D&C: Strassen's Method

## Problem
Write a program to multiply two square matrices of size `n × n` using
Strassen's divide-and-conquer method.

## Files
- `strassen.c` — full recursive Strassen implementation, with padding
  support for non-power-of-2 sizes, a naive `O(n^3)` multiplier for
  verification, and a timing comparison.

## Build & Run
```bash
gcc -O2 -Wall -o strassen strassen.c
./strassen
```

## Approach

**Naive multiplication:** `T(n) = O(n^3)` — for each of `n^2` output
cells, sum `n` products.

**Strassen's algorithm:** split each `n × n` matrix into four
`(n/2) × (n/2)` blocks:
```
A = [A11 A12]   B = [B11 B12]
    [A21 A22]       [B21 B22]
```
Instead of the naive **8** block multiplications, Strassen computes
just **7** products using clever linear combinations:

```
M1 = (A11 + A22)(B11 + B22)
M2 = (A21 + A22) B11
M3 = A11 (B12 - B22)
M4 = A22 (B21 - B11)
M5 = (A11 + A12) B22
M6 = (A21 - A11)(B11 + B12)
M7 = (A12 - A22)(B21 + B22)

C11 = M1 + M4 - M5 + M7
C12 = M3 + M5
C21 = M2 + M4
C22 = M1 - M2 + M3 + M6
```
This gives the recurrence `T(n) = 7T(n/2) + O(n^2)`, which by the
Master theorem solves to `T(n) = O(n^log2(7)) ≈ O(n^2.807)`,
asymptotically better than naive `O(n^3)`.

**Implementation details:**
- A **base-case threshold** (`n ≤ 64`) falls back to naive
  multiplication for small blocks, since recursion overhead makes
  Strassen slower than naive at small sizes in practice.
- Non-power-of-2 input sizes are handled by **zero-padding** up to the
  next power of two, multiplying, then cropping the result back down.
- Every result is checked against a naive `O(n^3)` multiplication for
  correctness.

## Complexity
`Θ(n^log2 7) ≈ Θ(n^2.807)` multiplications, vs. `Θ(n^3)` for the naive
method. The asymptotic advantage only becomes apparent for large `n`;
for small/moderate `n` the extra recursion & matrix-addition overhead
can make Strassen comparable to or slower than naive (demonstrated in
the program's timing table).

## Sample output
```
Verification: OK, results match.

Timing comparison (Strassen vs naive), n x n random matrices:
       n          naive (s)       strassen (s)
      50             0.0001             0.0002
     100             0.0005             0.0013
     300             0.0125             0.0679
```
