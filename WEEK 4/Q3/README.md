## Problem

Given a set **S** of *n* integers and an integer **T**, determine whether there exist **k elements** such that:

```
a₁ + a₂ + ... + a_k = T
```

---

## Key Idea

This is a generalization of:

* 2-sum
* 3-sum
* k-sum

We solve it using:

* **Sorting**
* **Recursion (reduce k → k-1)**
* **Two-pointer technique for base case (k = 2)**

---

## Approach

1. Sort the array → O(n log n)
2. Fix one element and recursively solve (k-1)-sum on remaining elements
3. Base case:

   * When k = 2 → use **two-pointer technique** in O(n)

---

## Algorithm

### kSum(arr, n, k, T):

* If k == 2:

  * Use two pointers (left, right)
  * Check if pair sums to T
* Else:

  * For each element arr[i]:

    * Recursively call:

      ```
      kSum(arr[i+1...], k-1, T - arr[i])
      ```

---

## Time Complexity

Recurrence:

```
T(n, k) = n * T(n, k-1)
```

Base case:

```
T(n, 2) = O(n)
```

Final:

```
O(n^(k-1))
```

Including sorting:

```
O(n^(k-1) + n log n) ≈ O(n^(k-1))
```

---

## Space Complexity

* Recursive stack: **O(k)**

---

## Example

### Input

```
S = {1, 2, 3, 4, 5}
k = 3
T = 9
```

### Output

```
Exists
```

### Explanation

```
2 + 3 + 4 = 9
```

---

## Why This Works

* Sorting enables efficient two-pointer search
* Reduces k-sum to (k-1)-sum recursively
* Avoids brute-force O(n^k)

---

## Conclusion

Efficient k-sum solution using:

* Sorting
* Recursion
* Two-pointer optimization

Scales much better than naive approach for moderate k.
