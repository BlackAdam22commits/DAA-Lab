# DAA Lab-04 — Q5: Application of Sorting-V

## Problem Statement

Given `n` intervals `(x_i, y_i)`, merge all overlapping intervals.

### Example

**Input:**

```text
{(1,3), (2,6), (8,10), (7,18)}
```

**Output:**

```text
{(1,6), (7,18)}
```

The program also treats **touching intervals** as overlapping. For example:

```text
(1,5) and (5,10)
```

are merged into:

```text
(1,10)
```

---

## Algorithm Used

The solution uses a **sorting-based approach**.

### Step 1: Sort the intervals

Sort all intervals in ascending order according to their starting point `x_i`.

For example:

```text
Input:
(10,20), (1,30), (5,8)

After sorting:
(1,30), (5,8), (10,20)
```

### Step 2: Merge using a linear scan

Maintain a current interval:

```text
[curStart, curEnd]
```

For each subsequent interval `(x,y)`:

* If `x <= curEnd`, the intervals overlap or touch.

  * Extend `curEnd` if `y` is larger.
* Otherwise, the current interval is complete.

  * Store it in the output.
  * Start a new current interval.

At the end of the scan, store the final current interval.

---

## Algorithm

```text
MERGE-INTERVALS(intervals, n)

1. Sort intervals according to starting point.
2. Set current interval = first sorted interval.
3. For each remaining interval:
      a. If its start <= current end:
            current end = max(current end, interval end)
      b. Otherwise:
            add current interval to output
            current interval = current interval
4. Add the final current interval to output.
5. Return the merged intervals.
```

---

## Time Complexity

### Sorting

The intervals are sorted using `qsort()`.

```text
O(n log n)
```

### Merging

After sorting, the program performs a single pass through the intervals.

```text
O(n)
```

### Overall

Therefore:

```text
O(n log n) + O(n)
= O(n log n)
```

The overall time complexity is:

**O(n log n)**

The sorting operation dominates the linear merging step.

---

## Space Complexity

The program creates a copy of the input intervals before sorting:

```c
Interval *sorted = malloc(n * sizeof(Interval));
```

The output array also requires space for at most `n` intervals.

Therefore, the additional working space is:

**O(n)**

---

## Input Format

The program currently uses predefined test cases inside `main()` rather than reading intervals from standard input.

Each interval is represented using:

```c
Interval = {start, end}
```

For example:

```c
Interval t1[] = {{1, 3}, {2, 6}, {8, 10}, {7, 18}};
```

The number of intervals is supplied separately:

```c
runTest(..., t1, 4);
```

---

## Output Format

For every test case, the program displays:

1. Test-case description
2. Input intervals
3. Merged output intervals

Example:

```text
--- Test 1: lab sheet example (expect {(1,6),(7,18)}) ---
Input : {(1,3), (2,6), (8,10), (7,18)}
Output: {(1,6), (7,18)}
```

---

# Test Cases

The program contains six test cases designed to verify different situations.

## Test 1 — Lab Sheet Example

Input:

```text
{(1,3), (2,6), (8,10), (7,18)}
```

Expected output:

```text
{(1,6), (7,18)}
```

This verifies the example given in the problem statement.

---

## Test 2 — No Overlapping Intervals

Input:

```text
{(1,2), (5,6), (10,12)}
```

Expected output:

```text
{(1,2), (5,6), (10,12)}
```

Since none of the intervals overlap, they remain unchanged.

---

## Test 3 — All Intervals Merge

Input:

```text
{(1,4), (2,5), (3,6), (4,8)}
```

Expected output:

```text
{(1,8)}
```

All four intervals overlap directly or indirectly and therefore form one merged interval.

---

## Test 4 — Touching Endpoints

Input:

```text
{(1,5), (5,10)}
```

Expected output:

```text
{(1,10)}
```

The implementation considers intervals with equal endpoints to overlap because it uses:

```c
if (sorted[i].x <= cur.y)
```

---

## Test 5 — Nested Interval

Input:

```text
{(10,20), (1,30), (5,8)}
```

After sorting:

```text
{(1,30), (5,8), (10,20)}
```

Expected output:

```text
{(1,30)}
```

The intervals `(5,8)` and `(10,20)` are completely contained within `(1,30)`.

---

## Test 6 — Single Interval

Input:

```text
{(1,1)}
```

Expected output:

```text
{(1,1)}
```

This verifies that the algorithm correctly handles the smallest non-empty input.

---

# Program Structure

The program is organized into the following components.

### `Interval`

```c
typedef struct {
    int x, y;
} Interval;
```

Represents an interval with starting point `x` and ending point `y`.

### `cmpIntervalByStart()`

```c
int cmpIntervalByStart(const void *a, const void *b)
```

Comparison function used by `qsort()` to sort intervals according to their starting points.

### `mergeIntervals()`

```c
int mergeIntervals(const Interval *intervals, int n, Interval *out)
```

Main algorithm responsible for sorting and merging the intervals.

### `printIntervals()`

Displays intervals in a readable format.

### `runTest()`

Runs an individual test case and prints its input and output.

### `main()`

Defines and executes the six test cases.

---

# Compilation

The program is written in standard C.

Using GCC:

```bash
gcc q5_merge_intervals.c -o q5_merge_intervals
```

For additional compiler warnings:

```bash
gcc -Wall -Wextra -std=c11 q5_merge_intervals.c -o q5_merge_intervals
```

---

# Execution

Run the compiled program using:

```bash
./q5_merge_intervals
```

On Windows using MinGW:

```bash
q5_merge_intervals.exe
```

---

# Expected Overall Output

The program should produce results equivalent to:

```text
--- Test 1: lab sheet example (expect {(1,6),(7,18)}) ---
Input : {(1,3), (2,6), (8,10), (7,18)}
Output: {(1,6), (7,18)}

--- Test 2: no overlaps ---
Input : {(1,2), (5,6), (10,12)}
Output: {(1,2), (5,6), (10,12)}

--- Test 3: all merge into one ---
Input : {(1,4), (2,5), (3,6), (4,8)}
Output: {(1,8)}

--- Test 4: touching endpoints merge ---
Input : {(1,5), (5,10)}
Output: {(1,10)}

--- Test 5: nested interval absorbed ---
Input : {(10,20), (1,30), (5,8)}
Output: {(1,30)}

--- Test 6: single interval ---
Input : {(1,1)}
Output: {(1,1)}
```

---

# Correctness Explanation

After sorting, intervals are processed from left to right according to their starting points.

At any point during the scan, `cur` represents the union of all intervals processed so far that belong to the current overlapping group.

If the next interval starts at or before `cur.y`, it overlaps or touches the current interval. Therefore, it can safely be merged by extending the right endpoint to:

```text
max(cur.y, next.y)
```

If the next interval starts after `cur.y`, there is a gap between the two intervals. Therefore, the current interval cannot overlap with any later interval that starts even further to the right, so it can safely be added to the output.

Thus, every overlapping group is merged into exactly one interval, and all non-overlapping groups remain separate.

---

# Important Assumption

The program assumes that each interval is represented as:

```text
(x, y) where x <= y
```

It also assumes that **touching intervals are considered overlapping**, meaning:

```text
(1,5) and (5,10) → (1,10)
```

If the lab instructor intends strictly overlapping intervals only, the condition:

```c
sorted[i].x <= cur.y
```

should instead be:

```c
sorted[i].x < cur.y
```

---

# Conclusion

The program solves the interval-merging problem using a sorting-based algorithm.

The key idea is to first sort intervals by their starting points and then perform a single linear scan to merge overlapping intervals.

The resulting complexity is:

```text
Time Complexity  : O(n log n)
Space Complexity : O(n)
```

The implementation includes multiple test cases covering normal, non-overlapping, completely overlapping, touching, nested, and single-interval scenarios.
