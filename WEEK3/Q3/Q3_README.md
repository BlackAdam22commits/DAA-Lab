# Q3 — Maximum and Minimum using Divide & Conquer (≤ 3n/2 comparisons)

## Problem
Find the maximum and minimum elements of an array of size `n` using a
divide-and-conquer approach, such that the total number of comparisons
is bounded by `3n/2`. Implement and validate.

## Files
- `maxmin.c` — implements the D&C max/min algorithm alongside a naive
  linear-scan version, counts comparisons for both, and writes
  `maxmin_results.csv`.
- `maxmin_results.csv` — generated on running the program.

## Build & Run
```bash
gcc -O2 -Wall -o maxmin maxmin.c
./maxmin
```

## Approach

**Naive scan:** compare every element against the running max *and*
running min separately → `2(n − 1)` comparisons.

**Divide and conquer (element-pairing form)** — this form hits the
*tight* bound `⌈3n/2⌉ − 2` for **every** `n`, not just powers of two
(a naive "always split into two halves recursively down to single
elements" approach can overshoot this bound by a small constant for
some non-power-of-two sizes, which is why pairing is used instead):

- **DIVIDE:** group the elements into `⌊n/2⌋` pairs (plus one leftover
  element if `n` is odd).
- **CONQUER:** for each pair, **one comparison** determines the pair's
  local winner (larger) and local loser (smaller).
- **COMBINE:** the overall maximum can only be among the local
  winners, and the overall minimum only among the local losers — so we
  maintain a running max/min while scanning the pairs, using 2 more
  comparisons per pair (1 to update max, 1 to update min).

### Comparison count
| Step | Comparisons |
|---|---|
| Pairing | `⌊n/2⌋` |
| Update running max per pair | `⌊n/2⌋ − 1` (first pair seeds it) |
| Update running min per pair | `⌊n/2⌋ − 1` (first pair seeds it) |
| **Total (n even)** | `n/2 + (n/2 − 1) + (n/2 − 1) = 3n/2 − 2` |
| **Total (n odd)** | one extra element folded in with 2 comparisons → `⌈3n/2⌉ − 2` |

## Complexity
`Θ(n)` time, exactly `⌈3n/2⌉ − 2` comparisons — a fixed ~25% savings
over the naive `2n − 2` scan, for every `n`.

## Sample output
```
Demo array of size 10 -> max = 90, min = 1, comparisons = 13
Tight bound ceil(3n/2) - 2 = 13

         n            D&C cmp          naive cmp   bound ceil(3n/2)-2
         2                  1                  2                    1
        10                 13                 18                   13
      1000               1498               1998                 1498
   1000000            1499998            1999998              1499998
```
The D&C comparison count matches the theoretical bound **exactly** at
every tested size — see the generated graph
`../graphs/Q3_maxmin_comparisons.png` in the parent project folder.
