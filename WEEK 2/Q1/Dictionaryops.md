# Q1 – Dictionary Operations

## Problem Statement

Consider a Dictionary (`D`) abstract data type that permits access to data items by their key.

The following are the primary operations supported by the dictionary:

- `Search(D, k)` – Given a search key `k`, return a pointer to the element in the dictionary whose key is `k`, if one exists.
- `Insert(D, x)` – Add a data item `x` to the dictionary.
- `Delete(D, x)` – Given a pointer `x` to a data item in the dictionary, remove it from the dictionary.
- `Max(D)` / `Min(D)` – Retrieve the item with the largest or smallest key.
- `Predecessor(D, x)` / `Successor(D, x)` – Retrieve the item whose key is immediately before or after the key of `x` in sorted order.

The objective is to determine the asymptotic worst-case running time of all seven operations when the dictionary is implemented using different data structures.

---

## Data Structures

The following implementations are considered:

1. Unsorted Array
2. Sorted Array
3. Singly Linked Unsorted List
4. Singly Linked Sorted List
5. Doubly Linked Unsorted List
6. Doubly Linked Sorted List

---

## Asymptotic Worst-Case Analysis

Let `n` be the number of elements in the dictionary.

| Data Structure | Search | Insert | Delete | Max | Min | Predecessor | Successor |
|---|---:|---:|---:|---:|---:|---:|---:|
| Unsorted Array | O(n) | O(1) | O(1) | O(n) | O(n) | O(n) | O(n) |
| Sorted Array | O(log n) | O(n) | O(n) | O(1) | O(1) | O(1) | O(1) |
| Singly Linked Unsorted List | O(n) | O(1) | O(1) | O(n) | O(n) | O(n) | O(n) |
| Singly Linked Sorted List | O(n) | O(n) | O(n) | O(1) | O(1) | O(n) | O(1) |
| Doubly Linked Unsorted List | O(n) | O(1) | O(1) | O(n) | O(n) | O(n) | O(n) |
| Doubly Linked Sorted List | O(n) | O(n) | O(1) | O(1) | O(1) | O(1) | O(1) |

### Notes

- For an unsorted array, insertion and deletion are considered `O(1)` when the location/pointer of the element is already available and ordering does not need to be maintained.
- For a sorted array, insertion and deletion may require shifting elements, resulting in `O(n)`.
- Searching a sorted array can be performed using binary search in `O(log n)`.
- In a singly linked sorted list, deletion requires locating the predecessor of the given node, resulting in `O(n)` in the worst case.
- In a doubly linked list, deletion of a known node is `O(1)` because the predecessor and successor pointers are directly available.
- In sorted linked lists, minimum and maximum can be accessed according to the maintained ordering.
- For unsorted structures, predecessor and successor in sorted order require examining the elements and therefore take `O(n)` time.

---

## Experimental Validation

A C program is implemented to validate the theoretical complexity of the dictionary operations.

The program:

1. Creates dictionaries using different data structures.
2. Performs the seven dictionary operations.
3. Measures the number of operations for different input sizes.
4. Stores the experimental results.
5. Plots the order of growth of the operations.
6. Compares the experimental results with the theoretical asymptotic complexity.

---

## Files

```text
Q1/
│
├── Q1Dictionary.c
├── dictionary.csv
├── dictionary.png
└── README.md
