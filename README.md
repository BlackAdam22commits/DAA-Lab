# Design and Analysis of Algorithms

**DAA Laboratory Assignments — IIIT Bhubaneswar**

![Language](https://img.shields.io/badge/Language-C-blue?style=flat-square&logo=c)
![Compiler](https://img.shields.io/badge/Compiler-GCC-red?style=flat-square&logo=gnu)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20Windows-green?style=flat-square)
![GitHub](https://img.shields.io/badge/Version%20Control-GitHub-black?style=flat-square&logo=github)

---

## Introduction

This repository contains the laboratory assignments completed for the **Design and Analysis of Algorithms (DAA)** course offered at **International Institute of Information Technology (IIIT), Bhubaneswar**.

The assignments focus on implementing fundamental algorithms in **C**, studying their theoretical properties, and experimentally validating their performance through practical implementation. Several programs generate CSV files that are later visualized using **Microsoft Excel**, allowing a comparison between theoretical and empirical results.

Each assignment contains:

- C source code
- Documentation
- CSV output (where applicable)
- Graphs generated using Microsoft Excel
- Time and Space Complexity Analysis
- Observations and Conclusions

---

## Student Information

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

## Repository Structure

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

## Week 1 Assignments

The first laboratory introduces the fundamentals of algorithm analysis through six programming assignments. These assignments cover asymptotic analysis, recursion, searching techniques, brute-force algorithms, randomized simulations, and empirical performance evaluation.

| Question | Topic | Source Code | Documentation |
|----------|-------|-------------|---------------|
| **Q1** | Growth Order of Functions | [Q1growth.c](./WEEK%201/Q1/Q1growth.c) | [README](./WEEK%201/Q1/README.md) |
| **Q2** | Fair vs Biased Coin Toss Simulation | [Q2coin toss.c](./WEEK%201/Q2/Q2coin%20toss.c) | [README](./WEEK%201/Q2/README.md) |
| **Q3** | Bubble Sort Performance Analysis | [Q3bubble sort.c](./WEEK%201/Q3/Q3bubble%20sort.c) | [README](./WEEK%201/Q3/README.md) |
| **Q4** | Towers of Hanoi | [Q4tower of hanoi.c](./WEEK%201/Q4/Q4tower%20of%20hanoi.c) | [README](./WEEK%201/Q4/README.md) |
| **Q5** | Partition Point using Binary Search | [Q5Partition Point.c](./WEEK%201/Q5/Q5Partition%20Point.c) | [README](./WEEK%201/Q5/README.md) |
| **Q6** | Element Uniqueness Problem | [Q6Element Uniqeness.c](./WEEK%201/Q6/Q6Element%20Uniqeness.c) | [README](./WEEK%201/Q6/README.md) |

---

## Assignment Overview

### Question 1 – Growth Order of Functions

Study and compare the asymptotic growth of different mathematical functions by arranging them according to their growth rates for sufficiently large values of **n**. The computed values are stored in a CSV file and plotted using Microsoft Excel.

---

### Question 2 – Fair vs Biased Coin Toss Simulation

Simulate fair and biased coin tosses for a large number of trials. Observe how the experimental probabilities converge to their theoretical values and verify the **Law of Large Numbers**.

---

### Question 3 – Bubble Sort Performance Analysis

Implement both the Standard Bubble Sort and the Optimized Bubble Sort. Compare their performances by counting the number of element comparisons for arrays of different sizes.

---

### Question 4 – Towers of Hanoi

Implement the recursive Towers of Hanoi algorithm. Count the number of moves required for different numbers of disks and verify the recurrence relation.

---

### Question 5 – Find the Partition Point

Given an array consisting of consecutive **0s** followed by **1s**, determine the first occurrence of **1** using both Linear Search and Binary Search, and compare their efficiencies.

---

### Question 6 – Element Uniqueness

Determine whether all elements in an array are unique using the brute-force approach and analyze its computational complexity.

---

## Experimental Results

| Question | CSV Output | Graph |
|----------|------------|-------|
| **Q1** | [Q1.csv](./WEEK%201/Q1/Q1.csv) | Generated using Microsoft Excel |
| **Q2** | [Q2.csv](./WEEK%201/Q2/Q2.csv) | [Fair Coin](./WEEK%201/Q2/Fair%20Coin.png) • [Biased Coin](./WEEK%201/Q2/Biased%20Coin.png) |
| **Q3** | [comparisons.csv](./WEEK%201/Q3/comparisons.csv) | [comparisons.png](./WEEK%201/Q3/comparisons.png) |
| **Q4** | [Q4.csv](./WEEK%201/Q4/Q4.csv) | [Exponential Graph](./WEEK%201/Q4/Exponential%20Graph.png) • [Log Graph](./WEEK%201/Q4/Log%20Graph.png) |

---

## Complexity Summary

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

## Concepts Covered

### Algorithm Analysis

- Asymptotic Notation (Big O, Big Ω, Big Θ)
- Time Complexity
- Space Complexity
- Growth of Functions
- Experimental Performance Analysis

### Algorithms

- Bubble Sort
- Optimized Bubble Sort
- Binary Search
- Linear Search
- Brute Force
- Recursive Algorithms
- Towers of Hanoi

### Mathematical Concepts

- Logarithmic Growth
- Polynomial Growth
- Exponential Growth
- Recurrence Relations

### Experimental Concepts

- Monte Carlo Simulation
- Law of Large Numbers
- CSV Data Generation
- Graph Plotting using Microsoft Excel

---

## Technologies Used

| Technology | Purpose |
|------------|---------|
| C Programming Language | Algorithm Implementation |
| GCC Compiler | Compilation |
| Microsoft Excel | Data Visualization |
| CSV | Experimental Data Storage |
| Git | Version Control |
| GitHub | Repository Hosting |
| Visual Studio Code | Code Editor |

---

## Compilation and Execution

Navigate to the desired question folder.

Example:

```bash
cd "WEEK 1/Q3"

gcc "Q3bubble sort.c" -o q3

./q3
```

Programs using mathematical functions should be compiled using:

```bash
gcc filename.c -o output -lm
```

---

## Clone the Repository

```bash
git clone https://github.com/<your-username>/Design-and-Analysis-of-Algorithms.git

cd Design-and-Analysis-of-Algorithms
```

---

## Repository Conventions

- Each laboratory week has its own directory.
- Every assignment is organized into a separate folder.
- Each assignment folder contains the source code, documentation, generated CSV files, and graphs (where applicable).
- Programs include complexity analysis and experimental observations whenever required.

---

## Future Work

Future laboratory assignments will include:

- Merge Sort
- Quick Sort
- Heap Sort
- Divide and Conquer
- Dynamic Programming
- Greedy Algorithms
- Backtracking
- Graph Algorithms
- Minimum Spanning Trees
- Shortest Path Algorithms

---

## License

This repository is maintained for academic and learning purposes as part of the **Design and Analysis of Algorithms Laboratory** at **IIIT Bhubaneswar**.

---

## Author

**Ayush Pattanaik**

**Roll Number:** B525018

**Computer Engineering**

**IIIT Bhubaneswar**
