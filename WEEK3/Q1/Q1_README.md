# Q1 — Binary Search vs Ternary Search

## Problem
Given a sorted array of size `n`, search for an element `x` using:
1. **Binary search** — splits the current interval into 2 nearly equal
   halves at each step.
2. **Ternary search** — splits the current interval into 3 nearly
   equal parts at each step.

Implement both, and justify/validate (experimentally) that binary
search performs better than ternary search.

## Files
- `search_compare.c` — implements both algorithms with a comparison
  counter, runs a small correctness demo, then runs an experiment
  across many array sizes and writes `search_results.csv`.
- `search_results.csv` — generated on running the program (worst-case
  average comparison counts for each tested `n`).

## Build & Run
```bash
gcc -O2 -Wall -o search_compare search_compare.c
./search_compare
```

## Approach
- **Binary search**: at each step, compute `mid`, compare `arr[mid]`
  with `x`. Up to **2 comparisons per level**, over `log2(n)` levels.
- **Ternary search**: at each step, compute `mid1` and `mid2`
  (dividing the range into 3 parts), compare `x` against both. Up to
  **4 comparisons per level**, over `log3(n)` levels.
- Both algorithms count every comparison made against an array
  element via a `comparisons` counter passed by reference.
- The experiment searches for a value guaranteed **not present** in
  the array (forcing the algorithm to shrink the interval all the way
  down to empty), which produces the worst-case comparison count, and
  averages this over several query points for stability.

## Why binary search wins (analysis)
| | Levels | Comparisons/level | Total (worst case) |
|---|---|---|---|
| Binary  | `log2 n` | up to 2 | `≈ 2 log2 n` |
| Ternary | `log3 n` | up to 4 | `≈ 4 log3 n = 4 · (log2 n / log2 3) ≈ 2.52 log2 n` |

Even though ternary search needs *fewer levels* (`log3 n < log2 n`),
it does *more work per level*, and `2 log2 n < 2.52 log2 n` for all
`n > 1` — so binary search always wins on total comparisons.

## Sample output
```
Demo array size n = 15, searching for x = 22
Binary  search -> found at index 10, comparisons = 7
Ternary search -> found at index 10, comparisons = 2

         n       avg binary cmp      avg ternary cmp
        10                    7                    9
        50                   11                   14
       100                   13                   16
      1000                   20                   24
   1000000                   40                   47
   5000000                   44                   51
```
(Binary search's count stays consistently below ternary search's
across every tested size — see the generated graph
`../graphs/Q1_binary_vs_ternary.png` in the parent project folder.)

## Complexity
- Binary search: `O(log2 n)` time, `O(log2 n)` comparisons (constant
  factor ≈ 2).
- Ternary search: `O(log3 n)` time, but `O(log3 n)` comparisons with a
  larger constant factor (≈ 4), making its total comparison count
  higher than binary search's despite fewer recursive levels.
