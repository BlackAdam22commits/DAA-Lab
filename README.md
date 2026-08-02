# Design and Analysis of Algorithms

**DAA Laboratory Assignments — IIIT Bhubaneswar**

![Language](https://img.shields.io/badge/Language-C-blue?style=flat-square&logo=c)
![Compiler](https://img.shields.io/badge/Compiler-GCC-red?style=flat-square&logo=gnu)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20Windows-green?style=flat-square)
![GitHub](https://img.shields.io/badge/Repository-GitHub-black?style=flat-square&logo=github)

---

## Introduction

This repository contains the laboratory assignments completed as part of the **Design and Analysis of Algorithms (DAA)** course at **IIIT Bhubaneswar**.

The objective of this repository is to implement classical algorithms, analyze their computational complexity, and experimentally verify theoretical concepts using practical implementations. Several assignments generate empirical data in CSV format, which is later visualized using Microsoft Excel to better understand algorithmic behavior and growth.

Each assignment includes:

- Well-documented C source code
- Individual README documentation
- Generated CSV files (where applicable)
- Graphs plotted using Microsoft Excel
- Time and space complexity analysis
- Experimental observations

The repository will continue to grow as more laboratory assignments are completed throughout the semester.

---

## Student Information

| Field | Details |
|-------|---------|
| **Name** | **Ayush Pattanaik** |
| **Roll Number** | **B525018** |
| **Branch** | Computer Engineering |
| **Institute** | International Institute of Information Technology (IIIT), Bhubaneswar |
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
└── WEEK 1
    │
    ├── Q1
    │   ├── Q1growth.c
    │   ├── Q1.csv
    │   └── README.md
    │
    ├── Q2
    │   ├── Q2coin toss.c
    │   ├── Q2.csv
    │   ├── fair coin (1).png
    │   ├── Biased coin.png
    │   └── Q2readme.md
    │
    ├── Q3
    │   ├── Q3bubble sort.c
    │   ├── comparisons.csv
    │   ├── comparisons.png
    │   └── Q3readme.md
    │
    ├── Q4
    │   ├── Q4tower of hanoi.c
    │   ├── Q4.csv
    │   ├── Toh exponential graph.png
    │   ├── Toh log graph.png
    │   └── Q4.md
    │
    ├── Q5
    │   ├── Q5Partition Point.c
    │   └── Q5readme.md
    │
    └── Q6
        ├── Q6Element Uniqeness.c
        └── Q6readme.md
```

---

## Week 1 Overview

The first laboratory introduces the fundamentals of algorithm analysis through six programming assignments. These problems emphasize asymptotic analysis, recursion, searching, brute-force techniques, randomized simulation, and empirical performance evaluation.

| Question | Topic | Source Code | Documentation |
|----------|-------|-------------|---------------|
| **Q1** | Growth Order of Functions | [Q1growth.c](./WEEK%201/Q1/Q1growth.c) | [README](./WEEK%201/Q1/README.md) |
| **Q2** | Fair vs Biased Coin Toss Simulation | [Q2coin toss.c](./WEEK%201/Q2/Q2coin%20toss.c) | [Q2readme.md](./WEEK%201/Q2/Q2readme.md) |
| **Q3** | Bubble Sort Performance Analysis | [Q3bubble sort.c](./WEEK%201/Q3/Q3bubble%20sort.c) | [Q3readme.md](./WEEK%201/Q3/Q3readme.md) |
| **Q4** | Towers of Hanoi | [Q4tower of hanoi.c](./WEEK%201/Q4/Q4tower%20of%20hanoi.c) | [Q4.md](./WEEK%201/Q4/Q4.md) |
| **Q5** | Find the Partition Point | [Q5Partition Point.c](./WEEK%201/Q5/Q5Partition%20Point.c) | [Q5readme.md](./WEEK%201/Q5/Q5readme.md) |
| **Q6** | Element Uniqueness | [Q6Element Uniqeness.c](./WEEK%201/Q6/Q6Element%20Uniqeness.c) | [Q6readme.md](./WEEK%201/Q6/Q6readme.md) |

---

## Assignment Summary

### Q1 – Growth Order of Functions

Study and compare the asymptotic growth of different mathematical functions by arranging them in increasing order for sufficiently large values of **n**. The generated data is exported as a CSV file and visualized using Microsoft Excel.

### Q2 – Fair vs Biased Coin Toss Simulation

Simulate fair and biased coin tosses for a large number of trials to observe how the experimental probability converges to the theoretical probability. The simulation demonstrates the **Law of Large Numbers**.

### Q3 – Bubble Sort Performance Analysis

Compare the standard Bubble Sort algorithm with an optimized implementation that terminates early if the array becomes sorted. The number of comparisons performed by both algorithms is recorded and plotted.

### Q4 – Towers of Hanoi

Implement the recursive solution for the Towers of Hanoi problem and verify the recurrence relation by counting the number of moves required for different numbers of discs.

### Q5 – Find the Partition Point

Locate the first occurrence of **1** in a binary array containing consecutive **0s** followed by **1s**. The assignment compares Linear Search and Binary Search.

### Q6 – Element Uniqueness

Determine whether an array contains duplicate elements using the brute-force approach by comparing every pair of elements.

---

## Results and Artifacts

| Question | CSV Output | Graph |
|----------|------------|-------|
| **Q1** | [Q1.csv](./WEEK%201/Q1/Q1.csv) | Generated using Microsoft Excel |
| **Q2** | [Q2.csv](./WEEK%201/Q2/Q2.csv) | [Fair Coin](./WEEK%201/Q2/fair%20coin%20(1).png) • [Biased Coin](./WEEK%201/Q2/Biased%20coin.png) |
| **Q3** | [comparisons.csv](./WEEK%201/Q3/comparisons.csv) | [comparisons.png](./WEEK%201/Q3/comparisons.png) |
| **Q4** | [Q4.csv](./WEEK%201/Q4/Q4.csv) | [Exponential Graph](./WEEK%201/Q4/Toh%20exponential%20graph.png) • [Log Graph](./WEEK%201/Q4/Toh%20log%20graph.png) |

---

## Complexity Summary

| Question | Algorithm | Time Complexity | Space Complexity |
|----------|-----------|-----------------|------------------|
| **Q1** | Growth Order Analysis | Depends on implementation | Depends on implementation |
| **Q2** | Coin Toss Simulation | **O(n)** | **O(1)** |
| **Q3** | Optimized Bubble Sort | Best: **O(n)** • Worst: **O(n²)** | **O(1)** |
| **Q3** | Standard Bubble Sort | **O(n²)** | **O(1)** |
| **Q4** | Towers of Hanoi | **O(2ⁿ)** | **O(n)** |
| **Q5** | Linear Search | **O(n)** | **O(1)** |
| **Q5** | Binary Search | **O(log n)** | **O(1)** |
| **Q6** | Element Uniqueness (Brute Force) | **O(n²)** | **O(1)** |

---

## Topics Covered

### Algorithm Analysis

- Asymptotic Notation (Big O, Big Ω and Big Θ)
- Growth of Functions
- Time Complexity Analysis
- Space Complexity Analysis
- Best, Average and Worst Case Analysis
- Experimental Performance Analysis

### Algorithms

- Bubble Sort
- Optimized Bubble Sort
- Linear Search
- Binary Search
- Recursive Algorithms
- Towers of Hanoi
- Brute Force Technique

### Mathematical Concepts

- Logarithmic Growth
- Polynomial Growth
- Exponential Growth
- Recurrence Relations
- Function Ordering

### Experimental Concepts

- Monte Carlo Simulation
- Law of Large Numbers
- Algorithm Performance Comparison
- CSV Data Generation
- Graph Plotting using Microsoft Excel

---

## Technologies Used

| Technology | Purpose |
|------------|---------|
| **C (C99/C11)** | Programming Language |
| **GCC** | Compiler |
| **C Standard Library** | Standard I/O, Memory Management, Mathematical Functions and Random Number Generation |
| **CSV** | Storing Experimental Data |
| **Microsoft Excel** | Graph Plotting and Data Visualization |
| **VS Code** | Code Editor |
| **Git** | Version Control |
| **GitHub** | Repository Hosting |

---

## Compilation and Execution

Navigate to the required assignment folder and compile using GCC.

Example:

```bash
cd "WEEK 1/Q3"

gcc "Q3bubble sort.c" -o q3

./q3
```

Programs using mathematical functions can be compiled as:

```bash
gcc filename.c -o output -lm
```

---

## Clone Repository

```bash
git clone https://github.com/<your-username>/Design-and-Analysis-of-Algorithms.git

cd Design-and-Analysis-of-Algorithms
```

---

## Repository Conventions

- Every laboratory week has its own directory.
- Every assignment is organized into an individual folder.
- Each folder contains:
  - Source code
  - Documentation
  - CSV outputs (where applicable)
  - Graphs (where applicable)
- Programs include complexity analysis and observations wherever applicable.

---

## Future Work

The repository will be expanded with future laboratory assignments covering topics such as:

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

This repository is maintained for academic purposes as part of the **Design and Analysis of Algorithms Laboratory** at **IIIT Bhubaneswar**. The source code is intended for learning and reference only.

---

## Author

**Ayush Pattanaik**  
**Roll Number:** B525018  
**Branch:** Computer Engineering  
**Institute:** IIIT Bhubaneswar
