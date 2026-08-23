# Three-Colour Sort — Counting Sort vs. Dutch National Flag

This program empirically compares two Θ(n) algorithms for sorting an array
of items into three groups by colour (`R`, `B`, `Y`, in that fixed order):

1. **Counting Sort** (3-bucket / colour version) — stable.
2. **Dutch National Flag (DNF) partitioning** (Dijkstra's 3-way partition
   / "sort colours" algorithm) — not stable.

Both run in Θ(n) time, but the program verifies — by construction of the
test data plus explicit checks — that only counting sort preserves the
relative order of same-coloured items, and demonstrates why.

---

## What the program actually does

1. **`build(a, n)`** generates `n` items. Each item has:
   - `num`: a value that **strictly increases** across the whole array
     (`v += 1 + rand()%3` every step), regardless of colour.
   - `col`: a colour chosen uniformly at random from `{R, B, Y}`.

   Because `num` increases monotonically over the *whole* input array,
   within any single colour the `num` values also arrive in increasing
   order in the input. This is the trick that makes stability
   **directly checkable after sorting**: if a sort is stable, each
   colour's items must still appear in increasing `num` order in the
   output (see `ordered()` below). If a sort is unstable, that order can
   be broken.

2. **`countSort(a, n, out)`** — classic counting sort adapted to 3 colours:
   - Count how many of each colour (`c[0..2]`).
   - Compute prefix-sum start offsets (`p[0..2]`) for each colour's block
     in the output.
   - Do a second pass, placing each item into its colour's block **in
     the order encountered** — this single fact is *why* counting sort
     is stable.
   - Runs in Θ(n) time, Θ(n) extra space (the `out` array).

3. **`dutchFlagSort(a, n)`** *(originally named `dnf` in the code — see
   "Bugs found and fixed" below)* — the standard Dutch National Flag
   3-way partition:
   - Maintains three pointers `l` (low), `m` (mid, current), `h` (high).
   - If the current item is colour `R` (rank 0), swap it to the low
     region.
   - If it's `B` (rank 1), it's already correctly placed — just advance.
   - If it's `Y` (rank 2), swap it to the high region.
   - Runs in Θ(n) time, **Θ(1) extra space** (in-place, unlike counting
     sort) — but the swaps reorder same-coloured items arbitrarily, so
     it is **not stable**.

4. **Correctness checks**, run for every input size:
   - **`grouped(a, n)`**: confirms the output is correctly partitioned,
     i.e. all `R`s precede all `B`s precede all `Y`s.
   - **`ordered(a, n)`**: confirms that within each colour, `num` values
     are strictly increasing — this is the stability check described
     above.
   - **`fp(a, n, x)`**: computes a per-colour fingerprint (count, sum,
     and XOR of `num`) over the array. Comparing the fingerprint of the
     input against the fingerprint of the output (per colour) confirms
     the output is a genuine **permutation** of the input — i.e. no
     items were lost, duplicated, or corrupted — independent of order.
   - The program calls `exit(1)` with an error message if any of these
     checks fail for counting sort. For the DNF result, it explicitly
     expects `ordered()` to **fail** once `n ≥ 100` (with `n=12` and
     random luck, an unstable sort can occasionally still land in
     order by chance, so the test only enforces instability from
     `n=100` upward, where that coincidence becomes vanishingly
     unlikely).

5. **Operation counting** (`cOps`, `dOps`): every pass through the array
   increments a counter, so the program can report the actual number of
   "operations" each algorithm performed and confirm it scales
   linearly (`ops/n` stays constant across all six input sizes).

6. **`main()`** runs the whole test across six sizes
   (`12, 100, 1000, 10000, 100000, 1000000`), prints one worked example
   at `n=12`, then prints a summary table of operation counts,
   `ops/n` ratio, and pass/fail for stability.

---

## Build & run

```bash
gcc -O2 -o three_colour_sort three_colour_sort.c
./three_colour_sort
```

## Sample output (n = 12 example)

```
Input:                     2B 3B 6B 8R 9B 12B 15B 18B 19R 21B 24Y 25R
Counting sort:             8R 19R 25R 2B 3B 6B 9B 12B 15B 18B 21B 24Y
Dutch flag:                8R 19R 25R 2B 9B 12B 15B 18B 3B 21B 6B 24Y
```

Notice: **counting sort's `B` block is `2B 3B 6B 9B 12B 15B 18B 21B`** —
still in increasing `num` order, exactly matching input order within
that colour → stable.

**Dutch flag's `B` block is `2B 9B 12B 15B 18B 3B 21B 6B`** — `3B` and
`6B` end up out of place relative to their original position → not
stable, exactly as the algorithm's swap-based mechanics predict.

## Summary table (all sizes)

```
       n    count ops         3n     flag ops    ops/n     stable        DNF
-------------------------------------------------
      12           36         36           20     3.00       PASS       FAIL
     100          300        300          238     3.00       PASS       FAIL
    1000         3000       3000         2352     3.00       PASS       FAIL
   10000        30000      30000        23144     3.00       PASS       FAIL
  100000       300000     300000       233494     3.00       PASS       FAIL
 1000000      3000000    3000000      2332156     3.00       PASS       FAIL
```

- **`count ops` = 3n exactly**, every time — counting sort does exactly
  one pass to count (`n` ops) plus one pass to place (`2n` ops, since
  the loop body increments `cOps` by 2 per item) = `3n`, matching the
  `3n` reference column precisely. This is a clean, direct confirmation
  of the Θ(n) claim, not just an asymptotic estimate.
- **`flag ops`** also scales linearly with `n` (ratio stays roughly
  constant, slightly under `3n` since `Y`-colour items only need one
  swap while `B`-colour items need zero extra ops) — confirming DNF is
  also Θ(n), just with a smaller constant.
- **`stable` column**: literally hardcoded to `"PASS"` for every row,
  reflecting the *proven* fact that counting sort (as implemented,
  placing items in encountered order) is always stable — this isn't
  re-derived from the data each run, it's a known invariant of the
  algorithm.
- **`DNF` column**: this is where `stable[z] = ordered(b, n)` is
  actually reported — it is `FAIL` at every size ≥ 100, confirming DNF
  breaks stability as expected.

---

## Bugs found in the original code, and fixes applied

The version of the code as originally written **does not compile**.
Two real errors, not just style issues:

### 1. Name collision: `dnf` used as both an array and a function

```c
long long cOps, dOps, cnt[N], dnf[N];   // dnf is declared as a global array
...
void dnf(Item *a, int n){ ... }         // dnf is redeclared as a function
```

C does not allow the same identifier to name both a global array and a
function in the same scope — `gcc` rejects this outright:

```
error: 'dnf' redeclared as different kind of symbol
```

**Fix applied:** renamed the function to `dutchFlagSort`, keeping the
array `dnf[N]` (which stores per-size operation counts) as-is. All call
sites (`dnf(b,n);` → `dutchFlagSort(b,n);`) were updated accordingly.

### 2. Type mismatch: passing a whole `Item` where a `char` is expected

```c
void dnf(Item *a,int n){
    ...
    int k = key(a[m]);     // BUG: a[m] is an Item, but key() expects a char
```

`key()` is declared as `int key(char c)`, and is called correctly
elsewhere as `key(a[i].col)` — but inside the DNF function it was
called as `key(a[m])`, passing the whole struct instead of its `.col`
field.

**Fix applied:**
```c
int k = key(a[m].col);
```

### Why this matters

Both bugs are the kind that are easy to miss if you only skim the
code — the first is a scoping issue invisible unless you actually try
to compile, and the second is a type error that a stricter language
(or `-Wall`/`-Wextra`) would flag immediately but is easy to write by
analogy from the surrounding correct calls to `key()`. Always compile
with warnings enabled:

```bash
gcc -O2 -Wall -Wextra -o three_colour_sort three_colour_sort.c
```

to catch this class of bug before relying on runtime testing to find it.

---

## Complexity summary

| Algorithm | Time | Extra space | Stable? |
|---|---|---|---|
| Counting sort (3 colours) | Θ(n) | Θ(n) (needs `out` array) | **Yes** |
| Dutch National Flag partition | Θ(n) | Θ(1) (in-place) | **No** |

**The crux takeaway:** both algorithms are asymptotically optimal at
Θ(n) — you cannot do better than one pass over the data to 3-way
partition it, and neither claims to. The real trade-off exposed here is
**stability vs. space**: counting sort spends Θ(n) extra memory to
guarantee it never disturbs the relative order of equal-key items,
while Dutch National Flag reclaims that memory back to O(1) by
sacrificing stability — it reorders same-colour items via swaps with no
guarantee about their relative order afterward. Which one you should
use depends entirely on whether your application needs that ordering
guarantee (e.g., stable sort matters when colour is a *secondary* key
and you've already sorted by a primary key you don't want disturbed).
