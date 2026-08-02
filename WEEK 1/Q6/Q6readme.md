# Element Uniqueness Check – DAA Lab (Week 1, Question 6)

## Overview

This project implements a simple **Element Uniqueness Check** using the brute-force approach in C. The program determines whether all elements in an array are unique by comparing every pair of elements.

The implementation demonstrates the straightforward approach to solving the problem and analyzes its time and space complexity.

---

## Features

- Accepts an array of integers from the user
- Displays the entered array
- Detects duplicate elements using pairwise comparison
- Reports whether all elements are unique
- Includes time and space complexity analysis
- Explains the limitations of the brute-force approach

---

## Problem Statement

Given an array of integers, determine whether all elements are **unique**.

### Example 1

```
Input:
1 4 6 8 10

Output:
All elements are unique.
```

### Example 2

```
Input:
2 5 8 5 9

Output:
Duplicate elements are present.
```

---

## Algorithm

1. Read the number of elements.
2. Input all array elements.
3. Compare each element with every subsequent element.
4. If any two elements are equal, report that duplicates exist.
5. Otherwise, report that all elements are unique.

---

## Build and Run

Compile the program using GCC:

```bash
gcc element_uniqueness.c -o uniqueness
```

Run:

```bash
./uniqueness
```

---

## Sample Output

```
===== Element Uniqueness Checker =====

Enter the number of elements: 5

Enter 5 integers:
10
20
30
40
20

Entered Array:
10 20 30 40 20

Result: Duplicate elements found.

Time Complexity  : O(n²)
Space Complexity : O(1)
```

---

## Complexity Analysis

| Complexity | Value |
|------------|-------|
| Time Complexity | **O(n²)** |
| Space Complexity | **O(1)** |

---

## Observations

- The algorithm compares every element with all remaining elements.
- It is easy to implement and requires no extra memory.
- The number of comparisons grows rapidly as the array size increases.
- For larger datasets, techniques such as **sorting** or **hashing** can detect duplicates more efficiently.

---

## Files

| File | Description |
|------|-------------|
| `element_uniqueness.c` | C implementation of the Element Uniqueness Check |
| `README.md` | Project documentation |

---

## Author

**Ayush Pattanaik**  
**Course:** Design and Analysis of Algorithms (DAA)
