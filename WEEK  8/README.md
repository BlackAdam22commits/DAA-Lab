# DAA Lab-08: Dynamic Programming in C

**Author:** Ayush Pattanaik (B525018)  
**Course:** Design and Analysis of Algorithms, BTech (CS-B and CE), 3rd Semester, IIIT Bhubaneswar  
**Instructor:** Dr. Ajaya Kumar Dash  

## Programs

| Q | Problem | Complexity |
|---|---|---|
| 1 | Minimum Coin Change | Time Theta(nV), Space Theta(V) |
| 2 | Coin Change: Total Number of Ways | Time Theta(nV), Space Theta(V) |
| 3 | Longest Common Subsequence (LCS) | Time Theta(mn), Space Theta(mn) |
| 4 | Longest Increasing Subsequence | Time Theta(n^2), Space Theta(n) |
| 5 | Maximum Sum Increasing Subsequence | Time Theta(n^2), Space Theta(n) |
| 6 | Edit Distance with Traceback | Time Theta(mn), Space Theta(mn) |
| 7 | Rod Cutting with Reconstruction | Time Theta(n^2), Space Theta(n) |
| 8 | Optimal Binary Search Tree (OBST) | Time Theta(n^3), Space Theta(n^2) |
| 9 | Collatz Conjecture | No proven bound; interval mode uses Theta(b) space |

## Build and run

Each program has a matching sample input. Compile a program with GCC and feed it the sample input:

```bash
gcc -Wall -O2 -o <executable> <source>
./<executable> < <sample input>
```

Question 8 needs the math library: add `-lm` when compiling.

## Input formats

- **Q1, Q2:** `n V`, then the `n` coin values.
- **Q3, Q6:** two whitespace-free strings.
- **Q4, Q5, Q7:** `n`, then `n` integers (array elements, or prices for Q7).
- **Q8:** `n`, then `p1..pn`, then `q0..qn` (all probabilities must sum to 1).
- **Q9:** `1 n` for one trajectory, or `2 a b` for interval analysis.
