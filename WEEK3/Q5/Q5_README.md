# Q5 — Multiply Special-Pattern Square Matrices in O(n²) using D&C

## Problem
Two `n × n` matrices (`n = 2^k`) are given, each with the recursive
block structure:
```
M = [ M1  M2 ]
    [ M2  M1 ]
```
where `M1` and `M2` are themselves `(n/2) × (n/2)` matrices with the
*same* recursive structure, all the way down to single scalars. Give a
divide-and-conquer, `O(n²)`, algorithm to multiply two such matrices,
and validate its complexity.

## Files
- `special_matrix.c` — builds special matrices from a compact
  "defining array", multiplies them with the O(n²) algorithm, verifies
  against a naive O(n³) full-matrix multiplication, and times both.

## Build & Run
```bash
gcc -O2 -Wall -o special_matrix special_matrix.c
./special_matrix
```

## Key insight

A "special" matrix of this recursive form is **fully determined by its
first row**. If `a[0..n-1]` is the first row of `M`, then for any
`i, j`:
```
M[i][j] = a[i XOR j]
```
This follows by induction on the recursive block definition: flipping
the top bit of `i` or `j` switches between the `M1` and `M2` block —
exactly matching how `i XOR j` changes when the top bit of `i` or `j`
flips.

## The product is also special

If `A[i][j] = a[i^j]` and `B[i][j] = b[i^j]`, then:
```
C[i][j] = Σ_k A[i][k]·B[k][j] = Σ_k a[i^k]·b[k^j]
```
Substituting `m = i^k` (so `k = i^m`, and `k^j = i^m^j`):
```
C[i][j] = Σ_m a[m]·b[(i^j)^m] = (a xorconv b)[i^j]
```
So `C` is special too, with defining array `c = a xorconv b` — an
**XOR-convolution** of the two length-`n` defining arrays.

## Divide and conquer for the XOR-convolution

Split `a = (a1, a2)`, `b = (b1, b2)` (each length `n/2` — simply the
defining arrays of the `M1`/`M2` sub-blocks). Then:
```
c1 = (a1 xorconv b1) + (a2 xorconv b2)
c2 = (a1 xorconv b2) + (a2 xorconv b1)
c  = (c1, c2)
```
This gives the recurrence:
```
T(n) = 4·T(n/2) + O(n)
```
which, by the Master theorem (`n^(log2 4) = n²` dominates the `O(n)`
term), solves to:
```
T(n) = Θ(n²)
```
— exactly the required bound, and dramatically better than the
`Θ(n³)` a naive full-matrix multiplication would cost.

## Validation performed by the program
1. Builds special matrices from random defining arrays.
2. Expands them to full `n × n` matrices and checks the recursive
   `[[M1,M2],[M2,M1]]` pattern holds at every level (sanity check).
3. Multiplies via the `O(n²)` D&C XOR-convolution.
4. Multiplies the expanded full matrices via naive `O(n³)`
   multiplication and checks the two results match exactly.
5. Confirms the **result** also satisfies the special pattern (as
   proven above).
6. Times both approaches across `n = 8 … 1024`.

## Sample output
```
Verification: OK, results match.
Pattern check on result C: valid special pattern (as proven)

Timing: O(n^2) D&C (on defining arrays) vs O(n^3) naive (on full matrices)
       n    O(n^2) time (s)    O(n^3) time (s)
     256            0.00085            0.00818
     512            0.00344          (skipped)
    1024            0.01338          (skipped)
```
(O(n³) runs are skipped past n=256 to keep the demo fast — the growth
rate is already clear.)
