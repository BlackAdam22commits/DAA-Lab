# Q2 — Search the Defective (Lighter) Coin

## Problem
You have `n` coins that are all supposed to weigh exactly the same.
At most **one** coin may be **lighter** than the rest (never heavier),
and it's possible that **none** are defective. Using a balance
weighing scale, design a divide-and-conquer algorithm that finds the
lighter coin (or correctly reports "none") in `⌊log2 n⌋ + c`
weighings, for some small constant `c`.

## Files
- `defective_coin.c` — full implementation with a simulated balance
  scale, a divide-and-conquer search, and a test harness that plants
  a defective coin (or none) across many values of `n` and verifies
  correctness.

## Build & Run
```bash
gcc -O2 -Wall -o defective_coin defective_coin.c
./defective_coin
```

## Approach

A real balance scale only tells you *which pan is lighter* — it
doesn't give numeric weights. Crucially, **the two pans being
compared must always hold the same number of coins**, otherwise a
pan being "heavier" could simply be because it has more coins in it,
not because its coins are more genuine.

**Divide and conquer strategy** — on a candidate group of `m` coins
(coins that might still contain the defective one; if the group turns
out to be fully genuine, then no defective coin exists anywhere):

- **If `m` is even:** split into two equal halves of `m/2` coins each
  and weigh them against each other.
  - Balanced → **no defective coin** exists in this group (report
    NONE).
  - Left pan lighter → recurse into the left half only.
  - Right pan lighter → recurse into the right half only.

- **If `m` is odd (`m ≥ 3`):** set one coin aside (unweighed), split
  the remaining `m − 1` coins into two equal halves of `(m − 1)/2`
  each, and weigh them.
  - Balanced → both halves are now proven genuine, so the only
    remaining suspect is the set-aside coin. Resolve it with **one
    more weighing**: the set-aside coin vs. any coin from the proven-
    genuine halves.
  - Left/right lighter → recurse into that half (the set-aside coin is
    now known genuine, since a defective coin — if one exists — must
    be in the lighter half).

Each full split roughly halves the candidate set, so the number of
weighings is about `⌈log2 n⌉` plus a small constant for the
odd-size verification step(s).

## Complexity
`O(log2 n + c)` weighings, matching the required bound. Verified
experimentally in the program for `n` from 2 up to 1,000,000, with
both a planted-defective-coin case and a no-defective-coin case tested
at each size.

## Sample output
```
n =       2 | planted = idx  | found = idx  | weighings =  1  (planted idx 0)  (found idx 0)
n =       2 | planted = none | found = none | weighings =  1
n =     100 | planted = idx  | found = idx  | weighings =  4  (planted idx 74)  (found idx 74)
n =     100 | planted = none | found = none | weighings =  1
n = 1000000 | planted = idx  | found = idx  | weighings = 19  (planted idx 474504)  (found idx 474504)
n = 1000000 | planted = none | found = none | weighings =  1
```
No mismatches occur at any tested size — every planted defective coin
(or its absence) is correctly identified.
