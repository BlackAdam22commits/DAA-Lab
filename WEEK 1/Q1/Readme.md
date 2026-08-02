# DAA Q1 – Ordering Functions by Rate of Growth

## Overview

This project implements a **Merge Sort-based solution** to order mathematical functions according to their asymptotic growth rates. The program compares functions using both **numerical evaluation** and **theoretical time complexity analysis**, demonstrating how constants and lower-order terms become insignificant as the input size increases.

The project also exports the computed data to a CSV file, allowing the growth curves to be visualized in Excel or any graphing software.

---

## Features

- Evaluates **12 mathematical functions** for multiple input sizes.
- Performs **stable Merge Sort** to rank functions numerically.
- Performs **theoretical ordering** using asymptotic growth classes.
- Compares numerical and theoretical rankings.
- Displays rank changes across different values of **n**.
- Generates an ASCII visualization of function growth.
- Exports graph-ready data to **growth_data.csv**.
- Reports overall **time and space complexity**.

---

## Functions Compared

- 1/n
- log₂(n)
- 12√n
- 50n^0.5
- n^0.51
- (2^32)n
- nlog₂(n)
- 100n² + 6n
- n² − 324
- 2n³
- n^(log₂n)
- 3ⁿ

---

## Test Values

The functions are evaluated for:

- n = 10²
- n = 10³
- n = 10⁴
- n = 10⁵
- n = 10⁶
- n = 10⁷
- n = 10⁸

---

## Methodology

### 1. Numerical Ranking

Each function is evaluated for every test value.

Instead of directly comparing very large values such as

- 3ⁿ
- n^(log₂n)

the program compares their **log₁₀ magnitudes**, preventing floating-point overflow while preserving the ordering.

The results are then sorted using **Merge Sort**.

---

### 2. Theoretical Ranking

Functions are grouped into asymptotic complexity classes:

| Category | Growth |
|----------|--------|
| Reciprocal | 1/n |
| Logarithmic | log n |
| Polynomial | n^k |
| Linearithmic | n log n |
| Super Polynomial | n^(log n) |
| Exponential | 3ⁿ |

Constants and lower-order terms are ignored according to asymptotic analysis.

---

## Output

The program produces:

- Numerical rankings for every value of n
- Rank movement table
- Final asymptotic ordering
- ASCII growth visualization
- Comparison between numerical and theoretical ordering
- Time and space complexity analysis
- **growth_data.csv** for plotting graphs in Excel

---

## CSV Output

The generated CSV contains:

```
function,category,n=100,n=1000,...,n=100000000
```

Each value represents

```
log10(f(n))
```

which allows large functions such as **3ⁿ** to be plotted without overflow.

---

## Build

Compile using GCC:

```bash
gcc q1_growth_order.c -lm -Wall -Wextra
```

Run:

```bash
./a.out
```

---

## Time Complexity

Let

- F = Number of functions
- T = Number of test values

Evaluation:

```
O(F × T)
```

Merge Sort:

```
O(F log F)
```

performed for each test value.

Overall:

```
O(T × F log F)
```

---

## Space Complexity

```
O(F × T)
```

Additional Merge Sort buffer:

```
O(F)
```

---

## Sample Growth Order

```
1/n
<
log₂(n)
<
√n
<
n^0.51
<
(2^32)n
<
nlog₂(n)
<
n²
<
2n³
<
n^(log₂n)
<
3ⁿ
```

---

## Learning Outcomes

- Merge Sort implementation from scratch
- Asymptotic analysis of algorithms
- Comparing empirical and theoretical growth rates
- Handling floating-point overflow using logarithms
- Exporting computational data for visualization in Excel
- Understanding why constants and lower-order terms are ignored in Big-O notation

---

## Author

**Ayush Pattanaik**

**Course:** Design and Analysis of Algorithms (DAA)
