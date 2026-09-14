# DAA Lab-07 — Puzzle Algorithms (C Programs)

BTech (CS-B and CE), 3rd Semester — Instructor: Dr. Ajaya Kumar Dash

Seven standalone C programs, `Q1.c` through `Q7.c`, one per puzzle. Each
file is self-contained (no shared headers) and can be compiled individually
with a plain C compiler. Full algorithm write-ups, formulas, complexity
analysis, and verification evidence for all seven are in
`DAA_Lab07_Report.docx`; this file just covers how to build and run each
program.

## Build

Each program is a single `.c` file. Compile with:

```bash
gcc -O2 -Wall -o Q1 Q1.c
```

or build all seven at once:

```bash
for i in 1 2 3 4 5 6 7; do
    gcc -O2 -Wall -o "Q$i" "Q$i.c"
done
```

## Programs

| File | Puzzle | Run example |
|------|--------|-------------|
| `Q1.c` | Invert the coin-triangle | `./Q1` → enter number of rows `n` (e.g. `4`) |
| `Q2.c` | Super egg testing experiment | `./Q2` → enter eggs `E` then floors `F` (e.g. `2` then `100`) |
| `Q3.c` | Reve's puzzle (4-peg Tower of Hanoi) | `./Q3` → enter number of disks `n` (e.g. `8`); optionally print the full move sequence |
| `Q4.c` | Security switches | `./Q4` → enter number of switches `n`; optionally print the full move sequence |
| `Q5.c` | Hitting a moving target | `./Q5` → enter number of hiding spots `n` (n ≥ 3) |
| `Q6.c` | The best time to be alive | `./Q6` → enter number of scientists `n`, then birth/death year pairs |
| `Q7.c` | Matrix Chain Multiplication | `./Q7` → enter number of matrices `n`, then the `n+1` dimensions |

## What each program does and prints

**Q1 — Coin-triangle**
Models an *n*-row triangle on a hex/axial lattice, finds the 180°-rotation
shift that maximizes overlap with the original, and reports the minimum
number of coin moves. Prints the search result, cross-checks it against the
closed form `moves = floor(n(n+1)/6)`, and lists the actual coins to move.

**Q2 — Super egg drop**
Builds the DP table `dp[t][e]` = max floors resolvable with `t` drops and
`e` eggs, reports the minimum guaranteed number of droppings, and (for
`E = 2`) cross-checks against the closed form `smallest m with m(m+1)/2 ≥ F`.
Prints the DP table for small inputs.

**Q3 — Reve's puzzle**
Computes the optimal Frame–Stewart split point and move count for every
disk count `1..n`, reports the answer for `n` disks (33 for `n = 8`), and
can print the complete, verified-legal move sequence.

**Q4 — Security switches**
Recognizes the puzzle as the classical Chinese Rings problem, computes the
minimum move count via the recurrence `f(n) = 2f(n-1) + (n mod 2)` (closed
form `floor(2^(n+1)/3)`), and can simulate/print the full optimal toggle
sequence.

**Q5 — Moving target**
Generates the guaranteed-hit shooting sequence (`2,3,...,n-1` then a second
pass, forward if `n` is odd or backward if `n` is even), and exhaustively
verifies the guarantee by tracking the full reachable-position set of an
adversarial target.

**Q6 — Best time to be alive**
Reads birth/death year pairs, sweeps sorted birth/death events (deaths
before births in a tie, per the problem's rule), and reports the maximum
number of scientists alive at once and the year it first occurs.

**Q7 — Matrix Chain Multiplication**
Standard O(n³) DP over chain intervals; reports the minimum number of
scalar multiplications and the optimal parenthesization, and prints the
cost table for small inputs.

## Notes

- All programs read input interactively from stdin (prompts are printed);
  pipe input for scripted testing, e.g. `printf "4\n" | ./Q1`.
- `Q3` and `Q4` ask an extra yes/no question about printing the full move
  sequence — answer `1` for yes or `0` for no.
- All seven were compiled with `gcc -O2 -Wall` (no warnings) and their
  outputs were cross-checked against brute-force/BFS ground truth or known
  textbook results before being finalized (see the report for details).
