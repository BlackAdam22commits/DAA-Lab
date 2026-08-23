# Q2 — Application of sorting-II

Given two sets S1 and S2 (each of size n) and a target x, determine
whether some `a ∈ S1, b ∈ S2` satisfy `a + b = x`. **O(n log n)**.

## Algorithm

1. Sort S2 — O(n log n).
2. For every element `a` in S1 (n elements), binary-search for `x - a`
   in the sorted S2 — O(log n) per lookup, O(n log n) total.
3. Report the first pair found, or report that none exists.

## Key idea

Sorting one of the two sets converts a brute-force O(n²) pairwise check
into n binary searches, each O(log n).

## Input format (`input.txt`)

```
n
S1[1] S1[2] ... S1[n]
S2[1] S2[2] ... S2[n]
x
```

## Output format (`output.txt`)

The matching pair `a + b = x` if one exists, otherwise a "no pair
found" message.

## Time complexity

**O(n log n)** — dominated by sorting S2 (O(n log n)) plus n binary
searches at O(log n) each (also O(n log n)); the two terms add to
O(n log n).

## Space complexity

**O(n)** — to hold S1 and the sorted copy of S2.

## Compile

```bash
gcc -O2 -Wall -Wextra -o q2_solution q2_solution.c
```

## Run

```bash
./q2_solution < input.txt
```

## Expected result / interpretation

Supplied input: S1 = {4, 9, -3, 15, 22, 1, 30, 7}, S2 = {10, -5, 2, 18,
6, 25, 3, 40}, x = 17. `15 (∈ S1) + 2 (∈ S2) = 17`, which is exactly
what the program reports.

## Files

- `q2_solution.c`
- `input.txt`
- `output.txt`
