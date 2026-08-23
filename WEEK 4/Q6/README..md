## Problem

Given a set **S** of *n* intervals:

```id="l1g8rj"
(l₁, r₁), (l₂, r₂), ..., (lₙ, rₙ)
```

find a **point p** on the number line that lies in the **maximum number of intervals**.

> Endpoints are inclusive: a point equal to `lᵢ` or `rᵢ` is considered inside the interval.

---

## Example

Input:

```id="d2k5q9"
S = {(10, 40), (20, 60), (50, 90), (15, 70)}
```

Output:

```id="q0x7fw"
p = 50 (covered by 3 intervals)
```

---

## Key Idea (Sweep Line Technique)

Transform each interval into **events**:

* `(lᵢ, +1)` → interval starts
* `(rᵢ, -1)` → interval ends

Then:

1. Sort all events by position
2. Traverse and maintain a running count
3. Track the maximum count and corresponding point

---

## Important Detail

Since endpoints are **inclusive**, when two events have the same coordinate:

* **Process start (+1) before end (-1)**

---

## Approach

1. Create an array of size `2n`:

   * Add `(lᵢ, +1)`
   * Add `(rᵢ, -1)`

2. Sort events:

   * First by coordinate
   * If equal → `+1` before `-1`

3. Traverse:

   * Maintain `current_count`
   * Update `max_count` and `best_point`

---

## Algorithm

```id="gk3p8m"
Create events array E of size 2n

For each interval:
  Add (lᵢ, +1)
  Add (rᵢ, -1)

Sort E:
  By coordinate
  If tie → start before end

current = 0
max = 0
best_point = -1

For each event:
  current += event.type
  If current > max:
    max = current
    best_point = event.coordinate
```

---

## Complexity

| Metric           | Value      |
| ---------------- | ---------- |
| Time Complexity  | O(n log n) |
| Space Complexity | O(n)       |

---

## Why This Works

* Sorting orders all interval boundaries
* Prefix sum (running count) gives active intervals
* Maximum value corresponds to maximum overlap

---

## Edge Cases

* Single interval → any point inside works
* Fully overlapping intervals → max = n
* Non-overlapping → max = 1
* Multiple valid points → any one is acceptable

---

## Conclusion

Efficient solution using:

* Sweep line technique
* Event sorting
* Prefix sum logic

Avoids brute-force checking of all points.
