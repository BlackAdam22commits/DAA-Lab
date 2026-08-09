# Q1 – Dictionary Operations

## Problem Statement

Consider a Dictionary (`D`) abstract data type that permits access to data items by their key.

The following are the primary operations supported by the dictionary:

- `Search(D, k)` – Given a search key `k`, return a pointer to the element whose key is `k`, if it exists.
- `Insert(D, x)` – Add a data item `x` to the dictionary.
- `Delete(D, x)` – Remove the data item pointed to by `x`.
- `Max(D)` – Retrieve the item with the largest key.
- `Min(D)` – Retrieve the item with the smallest key.
- `Predecessor(D, x)` – Retrieve the item whose key is immediately before the key of `x`.
- `Successor(D, x)` – Retrieve the item whose key is immediately after the key of `x`.

The objective is to determine and experimentally validate the asymptotic worst-case running time of these operations for different data structures.

## Data Structures

The following implementations are considered:

1. Unsorted Array
2. Sorted Array
3. Singly Linked Unsorted List
4. Singly Linked Sorted List
5. Doubly Linked Unsorted List
6. Doubly Linked Sorted List

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

## Notes

- In an unsorted array, insertion and deletion are considered `O(1)` when the insertion position or pointer to the element is already available.
- In a sorted array, insertion and deletion can require shifting elements, resulting in `O(n)`.
- Searching a sorted array can use binary search and takes `O(log n)`.
- In a singly linked sorted list, deleting a known node requires locating its predecessor, resulting in `O(n)`.
- In a doubly linked list, deletion of a known node is `O(1)` because the previous and next pointers are directly available.
- In sorted linked lists, minimum and maximum can be accessed directly according to the maintained ordering.
- In unsorted structures, finding the predecessor or successor in sorted order requires examining the elements and takes `O(n)`.

## Experimental Validation

A C program is used to validate the theoretical complexity of the dictionary operations.

The program:

1. Creates dictionaries using different data structures.
2. Performs the required dictionary operations.
3. Measures the operation counts for different input sizes.
4. Stores the experimental results in a CSV file.
5. Generates plots showing the order of growth.
6. Compares the experimental results with the theoretical complexity.

## Files

```text
Q1/
├── Q1Dictionary.c
├── ResultQ1.csv
├── deletegrowth.png
├── insertgrowth.png
├── maxgrowth.png
├── searchgrowth.png
