# Design and Analysis of Algorithms – Week 2

This directory contains the implementations and experimental analysis for **DAA Lab-02**.

**Course:** Design and Analysis of Algorithms
**Semester:** 3rd Semester
**Instructor:** Dr. Ajaya Kumar Dash
**Date:** August 4, 2026

---

## Directory Structure

```text
WEEK 2/
│
├── Q1/
│   ├── Dictionaryops.md
│   ├── Q1Dictionary.c
│   ├── ResultQ1.csv
│   ├── deletegrowth.png
│   ├── insertgrowth.png
│   ├── maxgrowth.png
│   └── searchgrowth.png
│
├── Q2/
│   ├── Q2.md
│   ├── Q2Mergesortvsmodifiedmergesort.c
│   ├── Q2plot.png
│   └── Q2timings.csv
│
├── Q3/
│   ├── Q3.md
│   ├── Q3Mergingksortedarrays.c
│   ├── Q3plot.png
│   └── Q3timings.csv
│
└── WEEK2README.md
```

---

## Q1. Dictionary Operations

### Objective

Analyze the asymptotic worst-case running time of the following dictionary operations:

* Search
* Insert
* Delete
* Maximum
* Minimum
* Predecessor
* Successor

The operations are analyzed using the following data structures:

1. Unsorted Array
2. Sorted Array
3. Singly Linked Unsorted List
4. Singly Linked Sorted List
5. Doubly Linked Unsorted List
6. Doubly Linked Sorted List

The C implementation experimentally measures the operations for different input sizes and generates data for order-of-growth analysis.

### Files

| File             | Description                               |
| ---------------- | ------------------------------------------ |
| [`Dictionaryops.md`](Q1/Dictionaryops.md) | Write-up / notes on dictionary operations |
| [`Q1Dictionary.c`](Q1/Q1Dictionary.c)     | C implementation of dictionary operations |
| [`ResultQ1.csv`](Q1/ResultQ1.csv)         | Experimental results                      |
| [`deletegrowth.png`](Q1/deletegrowth.png) | Order-of-growth graph — Delete            |
| [`insertgrowth.png`](Q1/insertgrowth.png) | Order-of-growth graph — Insert            |
| [`maxgrowth.png`](Q1/maxgrowth.png)       | Order-of-growth graph — Maximum           |
| [`searchgrowth.png`](Q1/searchgrowth.png) | Order-of-growth graph — Search            |

---

## Q2. Merge Sort vs. Modified Merge Sort

### Objective

Compare the standard Merge Sort algorithm with a modified version that divides the input array into three parts instead of two.

### Standard Merge Sort

The standard Merge Sort recurrence is:

```text
T(n) = 2T(n/2) + O(n)
```

Worst-case running time:

```text
O(n log n)
```

### Modified Merge Sort

The modified algorithm divides the input into three parts and recursively sorts each part before performing a three-way merge.

Its recurrence is:

```text
T(n) = 3T(n/3) + O(n)
```

Worst-case running time:

```text
O(n log n)
```

The program experimentally compares the order of growth of both algorithms.

### Files

| File            | Description                                        |
| ---------------- | --------------------------------------------------- |
| [`Q2.md`](Q2/Q2.md) | Write-up / notes for Q2 |
| [`Q2Mergesortvsmodifiedmergesort.c`](Q2/Q2Mergesortvsmodifiedmergesort.c) | Implementation of standard and modified Merge Sort |
| [`Q2plot.png`](Q2/Q2plot.png)     | Comparison of order-of-growth |
| [`Q2timings.csv`](Q2/Q2timings.csv) | Experimental results |

---

## Q3. Merging k Sorted Arrays

### Objective

Given `k` sorted arrays, each containing `n` elements, merge them into a single sorted array containing `kn` elements.

Two approaches are implemented and analyzed.

### Method 1: Sequential Merging

The arrays are merged sequentially:

```text
A1 + A2
     ↓
Result + A3
     ↓
Result + A4
     ↓
...
     ↓
Final Sorted Array
```

The worst-case running time is:

```text
O(nk²)
```

### Method 2: Pairwise Merging

The arrays are merged in pairs at each level:

```text
A1+A2    A3+A4    A5+A6    A7+A8
   ↓        ↓        ↓        ↓
   A1-A4          A5-A8
        ↓          ↓
          Final Merge
```

The worst-case running time is:

```text
O(nk log k)
```

### Complexity Comparison

| Method             | Worst-Case Running Time |
| ------------------ | ------------------------ |
| Sequential Merging | `O(nk²)`                |
| Pairwise Merging   | `O(nk log k)`           |

Pairwise merging provides a better asymptotic running time than sequential merging as `k` increases.

### Files

| File               | Description                            |
| ------------------- | ---------------------------------------- |
| [`Q3.md`](Q3/Q3.md)               | Write-up / notes for Q3 |
| [`Q3Mergingksortedarrays.c`](Q3/Q3Mergingksortedarrays.c) | Implementation of both merging methods |
| [`Q3plot.png`](Q3/Q3plot.png)     | Comparison of order-of-growth |
| [`Q3timings.csv`](Q3/Q3timings.csv) | Experimental results |

---

## Complexity Summary

| Problem | Algorithm             | Worst-Case Complexity                   |
| ------- | ---------------------- | ----------------------------------------- |
| Q1      | Dictionary Operations | Depends on data structure and operation |
| Q2      | Standard Merge Sort   | `O(n log n)`                            |
| Q2      | Modified Merge Sort   | `O(n log n)`                            |
| Q3      | Sequential Merging    | `O(nk²)`                                |
| Q3      | Pairwise Merging      | `O(nk log k)`                           |

---

## Learning Outcomes

* Analyze asymptotic worst-case running time.
* Compare dictionary operations across different data structures.
* Understand the impact of sorted and unsorted data structures.
* Analyze divide-and-conquer algorithms using recurrence relations.
* Implement and compare standard and modified Merge Sort.
* Analyze different strategies for merging `k` sorted arrays.
* Validate theoretical complexity through experimental measurements.
* Plot and interpret the order of growth of algorithms.

---

## Tools and Technologies

* C
* GCC
* Visual Studio Code
* CSV
* Git
* GitHub

---

## Author

**Ayush Pattanaik**

Design and Analysis of Algorithms
IIIT Bhubaneswar
