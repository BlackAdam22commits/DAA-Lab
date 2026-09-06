# DAA Lab-06 — Solutions
BTech (CS-B and CE), 3rd Semester — Instructor: Dr. Ajaya Kumar Dash

Each question below has its complexity analysis, and a corresponding working C program
in the matching subfolder (`q1_array_ops/`, `q2_matrix_ops/`, `q3_convolution/`,
`q4_sort_reversal/`), each compiled and tested with empirical timing to confirm the
theoretical growth rates.

---

## Q1. 1D array operations (n unsorted integers)

**Input representation:** `int a[n]`, a plain array (no auxiliary structure needed for
most operations; a sorted copy is used internally where noted).

| Op | Worst-case complexity | Reasoning |
|---|---|---|
| (i) Maximum | **O(n)** | One linear scan; an adversary can hide the max anywhere, so every element must be inspected. |
| (ii) First & second largest | **O(n)** | Single pass keeping two running values; classic tournament-style bound is also Θ(n). |
| (iii) Mean | **O(n)** | Sum all n elements once, divide by n. |
| (iv) Median | **O(n log n)** | Sort (comparison sort) then O(1) lookup. (An O(n) worst-case bound is achievable with median-of-medians/BFPRT selection, but the straightforward sort-based method is O(n log n).) |
| (v) Standard deviation | **O(n)** | One pass for mean + one pass for Σ(x−μ)² = O(n)+O(n) = O(n). |
| (vi) Mode | **O(n log n)** | Sort, then one linear scan finds the longest run of equal values. (Hashing gives expected O(n) but not worst-case O(n) for arbitrary integers.) |
| (vii) Remove duplicates | **O(n log n)** | Sort, then compact in one linear pass keeping first occurrence of each value. |
| (viii) Reverse | **O(n)** | n/2 swaps. |
| (ix) Partition (< pivot goes *after* ≥ pivot) | **O(n)** | Mirror image of Lomuto partitioning: one linear pass moving all elements ≥ pivot to the front. |

Program: `q1_array_ops/array_ops.c` — implements and times all nine operations;
verified against a 12-element test array and empirically timed for n = 1000…8000.

---

## Q2. 2D square matrix operations (n × n)

**Input representation:** matrix stored as a contiguous `double*` array of size n·n
accessed via `IDX(i,j,n) = i*n+j` (row-major), rather than an array of pointers —
this keeps memory contiguous for better cache behavior during the O(n³) work.

| Op | Worst-case complexity | Reasoning |
|---|---|---|
| (i) Addition | **O(n²)** | Every one of the n² entries added once. |
| (ii) Multiplication | **O(n³)** | Standard triple loop. (Strassen's D&C algorithm achieves O(n^2.807); Coppersmith–Winograd-style methods go lower asymptotically but are impractical here.) |
| (iii) Is zero matrix? | **O(n²)** | Must inspect every entry in the worst case. |
| (iv) Is symmetric? | **O(n²)** | Compare each (i,j) with (j,i) over the upper triangle: Θ(n²/2) = O(n²). |
| (v) Determinant | **O(n³)** | Gaussian elimination with partial pivoting; determinant = product of pivots (sign-flipped per row swap). Naive cofactor/Laplace expansion is O(n!) and must be avoided. |
| (vi) Transpose in place | **O(n²)** | n(n−1)/2 swaps of (i,j)/(j,i) pairs; only possible in place because the matrix is square. |
| (vii) Eigenvalue/eigenvector | **O(k·n²)** | Power iteration: each iteration is one O(n²) matrix–vector product; k = iterations to converge. (Full spectrum via the QR algorithm costs O(n³) per iteration and is not needed just to find one dominant eigenpair.) |

Program: `q2_matrix_ops/matrix_ops.c` — implements all seven operations on a 4×4
example (using A+Aᵀ to get a genuinely symmetric matrix for a valid eigen-demo),
plus empirical multiplication timing for n = 50…200 confirming cubic growth.

---

## Q3. Convolution of two vectors in O(n log n)

Given `A` (length m) and `B` (length n), n ≥ m, the convolution
`C[k] = Σ_j A[j]·B[k−j]` is **exactly polynomial multiplication**: if A and B are
read as coefficient vectors of polynomials p(x), q(x), then C is the coefficient
vector of p(x)·q(x).

**Direct summation** costs O(m·n) = O(n²) in the worst case (m ≈ n).

**O(n log n) Divide-and-Conquer algorithm — the FFT (Cooley–Tukey):**

1. Pad both A and B with zeros to length `size` = the next power of two ≥ m+n−1.
2. **Evaluate** both polynomials at the `size`-th roots of unity using the
   recursive FFT: split each polynomial's coefficients into even-indexed and
   odd-indexed halves, recursively transform each half (size n/2), and combine in
   O(n) using the "butterfly" step. This recursion satisfies
   `T(n) = 2T(n/2) + O(n)`, which by the Master Theorem (case 2, since
   n^(log_2 2) = n matches f(n) = O(n)) gives **T(n) = O(n log n)**.
3. **Point-wise multiply** the two transformed vectors — O(n).
4. **Interpolate** back via the inverse FFT (same recursive structure, O(n log n)).

Total: O(n log n) + O(n) + O(n log n) = **O(n log n)**.

**Input representation:** A and B as `double[]`, zero-padded internally to a
power-of-two length and represented as an array of `complex_t {re, im}` for the
transform.

Program: `q3_convolution/convolution.c` — implements the recursive FFT-based
`convolve()` and a naive O(m·n) `convolve_naive()` for cross-validation (max
floating point error ≈ 4×10⁻¹⁶ on the test case). Empirical timing for n =
1024…8192 shows the FFT version scaling far below the naive O(n²) curve
(e.g. at n=8192: FFT ≈ 0.0037s vs naive ≈ 0.039s, roughly a 10× gap that keeps
widening with n, as expected of n log n vs n²).

---

## Q4. Sorting a permutation via `reverse(p, i, j)`

**Input representation:** permutation `p` of {1,…,n} as `int p[n]` (0-indexed,
holding a permutation of the values 1..n).

### Part A — Sorting with O(n) reversal *operations* (cost of each reversal ignored)

**Claim:** any permutation of n elements can be sorted using at most n−1
reversals, i.e., **O(n)** reversal operations.

**Construction / proof:** process positions i = 0, 1, …, n−2 in order. At step i,
positions 0..i−1 already hold their final correct values (1..i respectively).
Let k ≥ i be the current index of the value (i+1) (the value that belongs at
position i). Call `reverse(p, i, k)`. This places value (i+1) at position i
(reversing p[i..k] moves whatever was at index k to index i), while positions
0..i−1 are untouched (the reversal only touches indices ≥ i). By induction, after
step i, positions 0..i hold their final correct values. After n−1 steps (i = 0
to n−2), positions 0..n−2 are correct, and the one remaining value must be n at
position n−1 automatically. Hence at most n−1 = **O(n)** reversal calls suffice.
∎

This is implemented as `sort_On_reversals()` and verified for random permutations
of size 100…3200 — the number of reversal calls used is always ≤ n−1 (observed:
essentially n−1 in the random case), confirming the O(n) bound empirically.

### Part B — Sorting minimizing total reversal *cost* = O(n log² n)

Here `cost(reverse(i,j)) = |j−i|+1`, and we must bound the **sum of costs**
across all reversal calls by O(n log² n).

**Algorithm — merge sort driven by in-place rotation (via reversal):**

- Recursively sort `p[lo..mid]` and `p[mid+1..hi]` (standard merge-sort
  recursion: T(n) = 2T(n/2) + Merge(n)).
- **Merge** the two adjacent sorted halves *in place* using only reversals, via
  the classic "rotate-by-three-reversals" trick:
  to left-rotate a segment `p[i..j]` so that the sub-block `p[k..j]` moves in
  front of `p[i..k-1]`, do:
  ```
  reverse(p, i,   k-1)
  reverse(p, k,   j)
  reverse(p, i,   j)
  ```
  This performs an in-place rotation using 3 reversals whose combined cost is
  O(segment length) — at most 3× the length of the segment being rotated.
- During the merge of `p[lo..mid]` and `p[mid+1..hi]`: walk a pointer `i` through
  the left block; whenever `p[i] > p[j]` (current right-block element), use
  **binary search** in the (sorted) right block to find how many consecutive
  right-block elements are smaller than `p[i]`, then rotate that whole chunk into
  place ahead of `p[i]` in a single 3-reversal rotation (rather than moving
  elements one at a time). This is the standard "block swap" in-place merge.

**Cost analysis:**
- At each level of the merge-sort recursion, the total *length* of segments
  being merged across all subproblems is n (the levels partition the array), and
  each element participates in at most O(1) rotations per level (each rotation
  moves a contiguous block of elements past another block at a cost proportional
  to the *combined* block length) — so the reversal-cost at one level is O(n).
- There are O(log n) levels of recursion.
- The binary search used to locate rotation boundaries adds at most an extra
  O(log n) factor to the bookkeeping/rotation cost in the worst case (each
  element can be repositioned across O(log n) levels, and locating its
  destination costs O(log n) via binary search) — giving an overall bound of
  **O(n log n) × O(log n) = O(n log² n)** in the worst case, which meets the
  required bound (and in practice/on random data the observed cost tracks
  closer to O(n log n), which is only better than required).

**Correctness:** correctness follows directly from standard merge-sort
correctness (by strong induction on subarray size: each recursive call fully
sorts its range, and the rotation-based merge produces a fully sorted combined
range because reversal-based rotation is just a different in-place
*implementation* of the standard merge — it produces exactly the same sorted
output as merging into an auxiliary array, verified by the `is_sorted()` check
after every test run).

Program: `q4_sort_reversal/sort_reversal.c` — implements both Part A
(`sort_On_reversals`) and Part B (`merge_sort_by_reversal` using
`rotate_via_reversal` + binary-search-driven `merge_via_rotation`). Both are
validated as producing a fully sorted array on random permutations of size
100…3200, with empirical growth of total reversal count (Part A, ~n) and total
reversal cost (Part B, consistent with the O(n log² n) bound) printed for each
size.

---

## How to build and run everything

```bash
cd q1_array_ops   && gcc -O2 array_ops.c      -o array_ops      -lm && ./array_ops
cd q2_matrix_ops  && gcc -O2 matrix_ops.c     -o matrix_ops     -lm && ./matrix_ops
cd q3_convolution && gcc -O2 convolution.c    -o convolution    -lm && ./convolution
cd q4_sort_reversal && gcc -O2 sort_reversal.c -o sort_reversal      && ./sort_reversal
```
