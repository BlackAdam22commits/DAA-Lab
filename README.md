# Design and Analysis of Algorithms

**DAA Laboratory Assignments — IIIT Bhubaneswar**

[![C](https://img.shields.io/badge/Language-C-A8B9CC?style=flat-square&logo=c&logoColor=white)](#)
[![GCC](https://img.shields.io/badge/Compiler-GCC-FE7A16?style=flat-square&logo=gnu&logoColor=white)](#)
[![Analysis](https://img.shields.io/badge/Analysis-Asymptotic%20Analysis-6E4AFF?style=flat-square)](#)
[![CSV](https://img.shields.io/badge/Data-CSV-1D9E5E?style=flat-square)](#)
[![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20Windows-4EAA25?style=flat-square&logo=linux&logoColor=white)](#)

---

# Introduction

This repository contains the laboratory assignments completed as part of the **Design and Analysis of Algorithms (DAA)** course at **IIIT Bhubaneswar**.

Each laboratory assignment is organized week-wise, and every question is maintained in its own folder containing:

- C source code
- README documentation
- Generated CSV files (where applicable)
- Graphs created using Microsoft Excel

The objective of these assignments is not only to implement algorithms but also to analyze their performance using asymptotic analysis and empirical observations.

---

# Student Information

| Field | Details |
|-------|---------|
| **Name** | **Ayush Pattanaik** |
| **Student ID** | **B525018** |
| **Branch** | **Computer Engineering (CE)** |
| **Institute** | **IIIT Bhubaneswar** |
| **Course** | **Design and Analysis of Algorithms Laboratory** |
| **Semester** | **B.Tech 3rd Semester** |
| **Instructor** | **Dr. Ajaya Kumar Dash** |

---

# About the Repository

- Implemented entirely in **C**.
- Compiled using **GCC**.
- Organized week-wise for better readability.
- Every question is placed inside its own folder.
- Programs generate CSV files wherever experimental analysis is required.
- Graphs are plotted using **Microsoft Excel**.
- Each question includes separate documentation explaining the algorithm, complexity, observations, and outputs.

---

# Repository Structure

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

# Lab Index

| Week | Topics Covered | Questions |
|------|----------------|----------|
| **Week 1** | Growth of Functions, Coin Toss Simulation, Bubble Sort Analysis, Towers of Hanoi, Partition Point, Element Uniqueness | **6** |

---

# WEEK 1

| Question | Topic | Source Code | Documentation |
|----------|-------|-------------|---------------|
| **Q1** | Ordering Functions by Growth Rate | `Q1/Q1growth.c` | `Q1/README.md` |
| **Q2** | Fair vs Biased Coin Toss Simulation | `Q2/Q2coin toss.c` | `Q2/Q2readme.md` |
| **Q3** | Bubble Sort Performance Analysis | `Q3/Q3bubble sort.c` | `Q3/Q3readme.md` |
| **Q4** | Towers of Hanoi | `Q4/Q4tower of hanoi.c` | `Q4/Q4.md` |
| **Q5** | Find the Partition Point | `Q5/Q5Partition Point.c` | `Q5/Q5readme.md` |
| **Q6** | Element Uniqueness | `Q6/Q6Element Uniqeness.c` | `Q6/Q6readme.md` |

---

# Results and Artifacts

| Question | CSV File | Graph |
|----------|----------|-------|
| **Q1** | `Q1/Q1.csv` | Generated using Microsoft Excel |
| **Q2** | `Q2/Q2.csv` | `fair coin (1).png`, `Biased coin.png` |
| **Q3** | `Q3/comparisons.csv` | `comparisons.png` |
| **Q4** | `Q4/Q4.csv` | `Toh exponential graph.png`, `Toh log graph.png` |

---

# Complexity Summary

| Question | Algorithm | Time Complexity | Space Complexity |
|-----------|-----------|-----------------|------------------|
| **Q1** | Growth Order Analysis | Depends on implementation | Depends on implementation |
| **Q2** | Coin Toss Simulation | **O(n)** | **O(1)** |
| **Q3** | Optimized Bubble Sort | **Best:** O(n) <br> **Worst:** O(n²) | O(1) |
| **Q3** | Standard Bubble Sort | **O(n²)** | O(1) |
| **Q4** | Towers of Hanoi | **O(2ⁿ)** | **O(n)** |
| **Q5** | Linear Search | **O(n)** | O(1) |
| **Q5** | Binary Search | **O(log n)** | O(1) |
| **Q6** | Brute Force Element Uniqueness | **O(n²)** | O(1) |

---

# Topics Covered

## Algorithm Analysis

- Asymptotic Notation (Big-O, Big-Ω, Big-Θ)
- Growth of Functions
- Time Complexity
- Space Complexity
- Best, Average and Worst Case Analysis
- Counting Comparisons

## Algorithms

- Bubble Sort
- Optimized Bubble Sort
- Binary Search
- Linear Search
- Brute Force Technique
- Recursive Algorithms
- Towers of Hanoi

## Concepts

- Growth Rate of Functions
- Monte Carlo Simulation
- Law of Large Numbers
- Recurrence Relations
- Partition Point
- Element Uniqueness
- Experimental Performance Analysis

---

# Technologies Used

| Tool | Purpose |
|------|---------|
| **C (C99/C11)** | Programming Language |
| **GCC** | Compiler |
| **C Standard Library** | stdio.h, stdlib.h, math.h, time.h |
| **CSV** | Data Storage |
| **Microsoft Excel** | Graph Plotting |
| **PNG** | Graph Images |
| **VS Code** | Code Editor |
| **Git & GitHub** | Version Control |

---

# Compilation and Execution

Example:

```bash
cd WEEK\ 1/Q1

gcc Q1growth.c -o q1 -lm

./q1
```

Similarly, navigate to the respective question folder and compile the desired program.

General compilation command:

```bash
gcc filename.c -o output -lm
```

Run:

```bash
./output
```

---

# Getting Started

Clone the repository:

```bash
git clone https://github.com/<your-username>/Design-and-Analysis-of-Algorithms.git

cd Design-and-Analysis-of-Algorithms
```

If GCC is not installed:

Ubuntu/Debian

```bash
sudo apt update
sudo apt install build-essential
```

---

# Repository Conventions

- Each laboratory week has its own directory.
- Every question is maintained inside an individual folder.
- Each question folder contains:
  - Source code
  - README documentation
  - CSV files (where applicable)
  - Graphs (where applicable)
- Programs include complexity analysis wherever applicable.
- Graphs are generated using Microsoft Excel from the CSV data.

---

# Future Work

Upcoming laboratory assignments will include:

- Merge Sort
- Quick Sort
- Heap Sort
- Divide and Conquer
- Dynamic Programming
- Greedy Algorithms
- Graph Algorithms

---

# License

This repository is maintained for academic purposes as part of the **Design and Analysis of Algorithms Laboratory** at **IIIT Bhubaneswar**.

The code may be used for learning and reference purposes only.

---

**Maintained by**

**Ayush Pattanaik**  
**B525018**  
**Computer Engineering (CE)**  
**IIIT Bhubaneswar**
