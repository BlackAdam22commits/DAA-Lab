# Find the Partition Point – DAA Lab (Week 1, Question 5)

## Overview

This project implements two approaches to find the **partition point** in a binary array consisting of a sequence of **0's followed by 1's**.

Example:

```
Input  : [0, 0, 0, 0, 1, 1, 1, 1]
Output : Partition Index = 4
```

The project compares the efficiency of:

- **Linear Search** – Scans the array sequentially.
- **Binary Search** – Exploits the sorted (monotonic) nature of the array to locate the first `1`.

The number of comparisons made by each algorithm is recorded to demonstrate the performance difference.

---

## Features

- Implementation of Linear Search (`O(n)`)
- Implementation of Binary Search (`O(log n)`)
- Tests on both small and large arrays
- Counts the number of comparisons performed
- Demonstrates the advantage of Binary Search on sorted data
- Prints detailed comparison results

---

## Problem Statement

Given a binary array where all **0's appear before all 1's**, determine the index of the **first occurrence of 1** (the partition point).

Example:

```
Array : 0 0 0 0 1 1 1 1 1
Index : 0 1 2 3 4 5 6 7 8

Partition Point = 4
```

---

## Algorithms Used

### Linear Search

- Starts from the first element.
- Checks each element until the first `1` is found.
- Suitable for unsorted data but inefficient for large arrays.

**Time Complexity:** `O(n)`

---

### Binary Search

- Utilizes the sorted structure of the array.
- Repeatedly divides the search space in half.
- Finds the first occurrence of `1` efficiently.

**Time Complexity:** `O(log n)`

---

## Test Cases

### Small Test

```
Array:
0 0 0 0 1 1 1 1 1
```

The program prints:

- Partition index
- Comparisons using Linear Search
- Comparisons using Binary Search

---

### Large Test

- Array Size: **1,000,000**
- Partition Point: **n / 3**

The program compares the number of comparisons performed by both algorithms.

---

## Sample Output

```
Small Test Case

Linear Search : Index = 4
Comparisons   : 5

Binary Search : Index = 4
Comparisons   : 3

----------------------------------

Large Test Case

Array Size      : 1000000
Actual Boundary : 333333

Linear Search Comparisons : 333334
Binary Search Comparisons : 20
```

---

## Build and Run

Compile using GCC:

```bash
gcc partition_point.c -o partition
```

Run:

```bash
./partition
```

---

## Complexity Analysis

| Algorithm | Time Complexity | Space Complexity |
|-----------|----------------:|-----------------:|
| Linear Search | O(n) | O(1) |
| Binary Search | O(log n) | O(1) |

---

## Observations

- Linear Search examines elements one by one until the first `1` is found.
- Binary Search dramatically reduces the number of comparisons by repeatedly halving the search space.
- As the input size increases, Binary Search becomes significantly more efficient.
- The experiment highlights why Binary Search is preferred whenever the input data is sorted.

---

## Files

| File | Description |
|------|-------------|
| `partition_point.c` | C implementation of Linear Search and Binary Search |
| `README.md` | Project documentation |

---

## Author

**Ayush Pattanaik**  
**Course:** Design and Analysis of Algorithms (DAA)
