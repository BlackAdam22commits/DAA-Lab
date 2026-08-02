# Design and Analysis of Algorithms

**DAA Laboratory Assignments — IIIT Bhubaneswar**

![Language](https://img.shields.io/badge/Language-C-blue?style=flat-square&logo=c)
![Compiler](https://img.shields.io/badge/Compiler-GCC-red?style=flat-square&logo=gnu)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20Windows-green?style=flat-square)
![GitHub](https://img.shields.io/badge/Version%20Control-GitHub-black?style=flat-square&logo=github)

---

# Introduction

This repository contains the laboratory assignments completed for the **Design and Analysis of Algorithms (DAA)** course offered at **International Institute of Information Technology (IIIT), Bhubaneswar**.

The repository focuses on implementing fundamental algorithms in **C**, analyzing their theoretical complexity, and validating their performance experimentally through generated datasets and graphs.

Each assignment contains:

- C source code
- Documentation
- CSV output (where applicable)
- Graphs generated using Microsoft Excel
- Time and Space Complexity Analysis
- Observations and Conclusions

---

# Student Information

| Field | Details |
|-------|---------|
| **Name** | **Ayush Pattanaik** |
| **Roll Number** | **B525018** |
| **Branch** | Computer Engineering |
| **Institute** | IIIT Bhubaneswar |
| **Course** | Design and Analysis of Algorithms Laboratory |
| **Semester** | B.Tech 3rd Semester |
| **Instructor** | Dr. Ajaya Kumar Dash |

---

# Repository Structure

```text
Design-and-Analysis-of-Algorithms/
│
├── README.md
│
└── WEEK 1/
    │
    ├── Q1/
    │   ├── Q1growth.c
    │   ├── Q1.csv
    │   ├── Growth.png
    │   └── README.md
    │
    ├── Q2/
    │   ├── Q2coin toss.c
    │   ├── Q2.csv
    │   ├── Fair Coin.png
    │   ├── Biased Coin.png
    │   └── README.md
    │
    ├── Q3/
    │   ├── Q3bubble sort.c
    │   ├── comparisons.csv
    │   ├── comparisons.png
    │   └── README.md
    │
    ├── Q4/
    │   ├── Q4tower of hanoi.c
    │   ├── Q4.csv
    │   ├── Exponential Graph.png
    │   ├── Log Graph.png
    │   └── README.md
    │
    ├── Q5/
    │   ├── Q5Partition Point.c
    │   └── README.md
    │
    └── Q6/
        ├── Q6Element Uniqeness.c
        └── README.md
```

---

# Week 1 Assignments

The first laboratory introduces the fundamentals of algorithm analysis through six programming assignments. These assignments cover asymptotic analysis, recursion, randomized simulation, brute-force methods, searching techniques, and experimental performance evaluation.

| Question | Topic | Source Code | Documentation |
|----------|-------|-------------|---------------|
| **Q1** | Growth Order of Functions | [Q1growth.c](./WEEK%201/Q1/Q1growth.c) | [Readme.md](./WEEK%201/Q1/Readme.md) |
| **Q2** | Fair vs Biased Coin Toss Simulation | [Q2coin toss.c](./WEEK%201/Q2/Q2coin%20toss.c) | [Q2readme.md](./WEEK%201/Q2/Q2readme.md) |
| **Q3** | Bubble Sort Performance Analysis | [Q3bubble sort.c](./WEEK%201/Q3/Q3bubble%20sort.c) | [Q3readme.md](./WEEK%201/Q3/Q3readme.md) |
| **Q4** | Towers of Hanoi | [Q4tower of hanoi.c](./WEEK%201/Q4/Q4tower%20of%20hanoi.c) | [Q4.md](./WEEK%201/Q4/Q4.md) |
| **Q5** | Partition Point | [Q5Partition Point.c](./WEEK%201/Q5/Q5Partition%20Point.c) | [Q5readme.md](./WEEK%201/Q5/Q5readme.md) |
| **Q6** | Element Uniqueness | [Q6Element Uniqeness.c](./WEEK%201/Q6/Q6Element%20Uniqeness.c) | [Q6readme.md](./WEEK%201/Q6/Q6readme.md) |

---

# Assignment Overview

## Question 1 – Growth Order of Functions

- Compare the asymptotic growth of twelve mathematical functions.
- Arrange them according to increasing order of growth.
- Generate logarithmic values and visualize the results using **Microsoft Excel**.

## Question 2 – Fair vs Biased Coin Toss Simulation

- Simulate fair and biased coin tosses.
- Observe convergence of experimental probability toward theoretical probability.
- Demonstrate the **Law of Large Numbers**.

## Question 3 – Bubble Sort Performance Analysis

- Compare Standard Bubble Sort and Optimized Bubble Sort.
- Count the number of comparisons.
- Analyze performance for increasing input sizes.

## Question 4 – Towers of Hanoi

- Implement the recursive Towers of Hanoi algorithm.
- Verify the recurrence relation.
- Plot the exponential growth of the number of moves.

## Question 5 – Find the Partition Point

- Compare Linear Search and Binary Search.
- Locate the first occurrence of **1** in an array of **0s followed by 1s**.
- Analyze the difference in running time.

## Question 6 – Element Uniqueness

- Determine whether all elements of an array are unique.
- Implement the brute-force solution.
- Analyze its quadratic time complexity.

---

# Experimental Results

| Question | CSV Output | Graph |
|----------|------------|-------|
| **Q1** | [Q1.csv](./WEEK%201/Q1/Q1.csv) | [Growth.png](./WEEK%201/Q1/Growth.png) |
| **Q2** | [Q2.csv](./WEEK%201/Q2/Q2.csv) | [Fair Coin](./WEEK%201/Q2/Fair%20Coin.png) • [Biased Coin](./WEEK%201/Q2/Biased%20Coin.png) |
| **Q3** | [comparisons.csv](./WEEK%201/Q3/comparisons.csv) | [comparisons.png](./WEEK%201/Q3/comparisons.png) |
| **Q4** | [Q4.csv](./WEEK%201/Q4/Q4.csv) | [Exponential Graph](./WEEK%201/Q4/Exponential%20Graph.png) • [Log Graph](./WEEK%201/Q4/Log%20Graph.png) |

---

# Complexity Summary

| Question | Algorithm | Time Complexity | Space Complexity |
|----------|-----------|-----------------|------------------|
| **Q1** | Growth Order Analysis | Depends on implementation | Depends on implementation |
| **Q2** | Coin Toss Simulation | **O(n)** | **O(1)** |
| **Q3** | Optimized Bubble Sort | Best: **O(n)**, Worst: **O(n²)** | **O(1)** |
| **Q3** | Standard Bubble Sort | **O(n²)** | **O(1)** |
| **Q4** | Towers of Hanoi | **O(2ⁿ)** | **O(n)** |
| **Q5** | Linear Search | **O(n)** | **O(1)** |
| **Q5** | Binary Search | **O(log n)** | **O(1)** |
| **Q6** | Element Uniqueness | **O(n²)** | **O(1)** |

---

# Topics Covered

## Algorithm Analysis

- Asymptotic Notation
- Big O, Big Ω and Big Θ
- Growth of Functions
- Time Complexity
- Space Complexity
- Experimental Analysis

## Algorithms

- Bubble Sort
- Optimized Bubble Sort
- Binary Search
- Linear Search
- Towers of Hanoi
- Brute Force
- Recursion

## Mathematical Concepts

- Logarithmic Growth
- Polynomial Growth
- Exponential Growth
- Recurrence Relations

## Experimental Concepts

- Monte Carlo Simulation
- Law of Large Numbers
- CSV Generation
- Graph Plotting using Microsoft Excel

---

# Technologies Used

| Technology | Purpose |
|------------|---------|
| C Programming Language | Algorithm Implementation |
| GCC | Compilation |
| Microsoft Excel | Graph Plotting |
| CSV | Data Storage |
| Git | Version Control |
| GitHub | Repository Hosting |
| Visual Studio Code | Code Editor |

---

# Compilation and Execution

```bash
cd "WEEK 1/Q3"

gcc "Q3bubble sort.c" -o q3

./q3
```

Programs requiring mathematical functions can be compiled as:

```bash
gcc filename.c -o output -lm
```

---

# Clone the Repository

```bash
git clone https://github.com/<your-username>/Design-and-Analysis-of-Algorithms.git

cd Design-and-Analysis-of-Algorithms
```

---

# Repository Conventions

- Each laboratory week is organized into a separate folder.
- Every assignment contains its own source code and documentation.
- CSV files and graphs are included wherever experimental analysis is performed.
- Programs include complexity analysis and observations.

---

# Future Work

Future laboratory assignments will include:

- Merge Sort
- Quick Sort
- Heap Sort
- Divide and Conquer
- Dynamic Programming
- Greedy Algorithms
- Graph Algorithms
- Backtracking

---

# License

This repository is maintained for academic purposes as part of the **Design and Analysis of Algorithms Laboratory** at **IIIT Bhubaneswar**.

---

# Author

**Ayush Pattanaik**

**Roll Number:** B525018

**Computer Engineering**

**IIIT Bhubaneswar**
