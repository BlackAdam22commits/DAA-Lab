# Q6 — Use of Loop Invariants in Sorting (Selection Sort)

## Pseudocode

```
SELECTION-SORT(A, n)
1  for i = 1 to n - 1              // only n-1 iterations needed (see below)
2      minIndex = i
3      for j = i + 1 to n
4          if A[j] < A[minIndex]
5              minIndex = j
6      if minIndex != i
7          exchange A[i] with A[minIndex]
```
(1-indexed, matching the question's `A[1..n]`. The accompanying C file
uses the equivalent 0-indexed version, loop `i = 0 .. n-2`.)

## Loop Invariant

**At the start of each iteration of the outer `for` loop (line 1),
the subarray `A[1..i-1]` consists of the `i-1` smallest elements of the
whole array, sorted in non-decreasing order** (equivalently:
`A[1..i-1]` is sorted AND every element in `A[1..i-1]` is `<=` every
element in `A[i..n]`).

### 1. Initialization
Before the first iteration, `i = 1`, so the subarray `A[1..i-1] =
A[1..0]` is empty. An empty subarray is trivially sorted, and the
"every element `<=` every remaining element" condition holds vacuously
(there is nothing to compare). So the invariant holds at the start.

### 2. Maintenance
Assume the invariant holds at the start of an iteration with a given
`i`: `A[1..i-1]` holds the `i-1` smallest elements, sorted. The loop
body (lines 2-7) scans `A[i..n]` and finds the index `minIndex` of the
smallest element in that range, then swaps it into position `A[i]`.

- Because `A[1..i-1]` already contains the `i-1` smallest elements
  (invariant), and `A[minIndex]` is the smallest value remaining in
  `A[i..n]`, `A[minIndex]` must be the `i`-th smallest element overall.
- Placing it at `A[i]` extends the sorted prefix to `A[1..i]`, which
  still holds the `i` smallest elements in sorted order (the new
  element is `>=` everything already in `A[1..i-1]`, since those were
  strictly the `i-1` smallest, and it is `<=` everything left in
  `A[i+1..n]` by construction of `minIndex`).
- Thus, when `i` is incremented for the next iteration, the invariant
  again holds — now with the (new, larger) value of `i`.

### 3. Termination
The loop runs while `i <= n-1`, i.e. it terminates once `i = n`. By
the invariant (now true with `i = n`), `A[1..n-1]` contains the
`n-1` smallest elements of the array, sorted in non-decreasing order,
and every element in `A[1..n-1]` is `<=` `A[n]` (the one remaining
element). Since `A[1..n-1]` is sorted and `A[n]` is `>=` all of them,
the **entire array `A[1..n]` is sorted** — which is exactly what we
needed to prove correctness.

## Why only the first (n − 1) elements?

After placing the `n-1` smallest elements correctly into `A[1..n-1]`,
only one element remains: `A[n]`. There is nothing left to compare it
against or select — by elimination, whatever value remains at
position `n` **must** be the largest element, and it is already in
the only slot left for it. Running an `i = n` iteration would be a
no-op (`j` would range from `n+1` to `n`, an empty range), so it is
simply skipped for efficiency.

## Running-time analysis

- The **inner loop** at iteration `i` always scans `n - i` elements to
  find the minimum, regardless of the input's initial order.
- Total comparisons:
  `Σ_{i=1}^{n-1} (n - i) = (n-1) + (n-2) + ... + 1 = n(n-1)/2`

This sum does **not** depend on whether the input was already sorted,
reverse sorted, or random — selection sort always performs exactly
`n(n-1)/2` comparisons.

- **Worst-case running time:** `Θ(n²)`
- **Best-case running time:** also `Θ(n²)` — **no better than the
  worst case**, because the *number of comparisons* never changes
  with input order (unlike, say, insertion sort or bubble sort with
  an early-exit flag). The only thing that changes with input order
  is the number of **swaps** (0 for an already-sorted array, up to
  `n-1` for specific adversarial inputs) — but swaps are not the
  dominant cost, comparisons are, so the asymptotic running time stays
  `Θ(n²)` in every case.

## Experimental validation

`selection_sort.c` in this folder:
- Implements the algorithm exactly as above.
- Contains an `invariantHolds()` checker that verifies the loop
  invariant (sorted prefix + prefix `<=` suffix) after every outer
  iteration on a demo array, and reports any violation.
- Measures comparisons and swaps for both an already-sorted array
  (best case) and a reverse-sorted array (worst case) across several
  sizes `n`, confirming that comparisons are identical
  (`n(n-1)/2`) in both cases while swaps differ, matching the analysis
  above.

Run it with:
```
gcc -O2 -Wall -o selection_sort selection_sort.c
./selection_sort
```
