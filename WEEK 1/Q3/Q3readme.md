# WEEK 1 – Question 3: Bubble Sort Performance Analysis

## Objective

Implement two variants of the Bubble Sort algorithm in C and compare their performance by counting the total number of element comparisons.

- **Optimized Bubble Sort** – Stops execution early if no swaps occur during a pass, indicating that the array is already sorted.
- **Standard Bubble Sort** – Executes all **(n - 1)** passes irrespective of whether the array becomes sorted before the final pass.

---

## Methodology

Random arrays of different sizes are generated for testing.

For each input size:

1. A random array is generated.
2. An identical copy of the array is created.
3. The optimized Bubble Sort is applied to one copy.
4. The standard Bubble Sort is applied to the other copy.
5. The number of comparisons performed by each algorithm is recorded.
6. The collected data is written to **comparisons.csv**.

---

## Implementation

### Optimized Bubble Sort

- Uses a **swap flag** to monitor whether any elements are exchanged during a pass.
- If no swaps occur, the algorithm concludes that the array is sorted and terminates immediately.

### Standard Bubble Sort

- Continues executing all **(n - 1)** passes regardless of the state of the array.
- Does not include any early termination condition.

---

## Graph

The data generated in **comparisons.csv** was imported into **Microsoft Excel** to create a graph comparing the number of comparisons made by both Bubble Sort implementations.

**Bubble Sort Comparison**

---

## Observations

- For randomly generated arrays, both implementations perform a similar number of comparisons.
- The optimized version offers better performance when the input array is already sorted or nearly sorted because it avoids unnecessary passes.
- The standard version performs the same number of passes for every input, even if the array becomes sorted earlier.

---

## Files

| File | Description |
|------|-------------|
| `Performance_analysis_of_bubble_sort.c` | C program implementing both Bubble Sort algorithms |
| `comparisons.csv` | CSV file containing the comparison results |
| `comparisons.png` | Graph generated from the CSV data using Microsoft Excel |
| `Performance_analysis_of_bubble_sort.md` | Documentation for Week 1 – Question 3 |
