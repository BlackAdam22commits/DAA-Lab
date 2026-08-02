/*
 * DAA Lab-01, Q4: Towers of Hanoi (ToH)
 * ----------------------------------------
 * Recurrence: T(n) = 2*T(n-1) + 1,  T(0) = 0
 * Closed form: T(n) = 2^n - 1
 *
 * Approach: recursively simulate the moves (counting them, not
 * printing every single one for large n since that would be huge),
 * then also print the actual move sequence for a small n so you can
 * see the recursive structure. We use 'long long' since 2^n-1
 * overflows a normal int/long around n=32-63.
 */

#include <stdio.h>

long long move_count = 0;

// prints moves only when print_moves is nonzero (kept small n only)
void hanoi(int n, char from, char aux, char to, int print_moves) {
    if (n == 0) return;
    hanoi(n - 1, from, to, aux, print_moves);
    move_count++;
    if (print_moves) printf("Move disc %d from %c to %c\n", n, from, to);
    hanoi(n - 1, aux, from, to, print_moves);
}

int main(void) {
    // 1) show the actual moves for a small case, n = 4
    printf("=== Move sequence for n = 4 discs ===\n");
    move_count = 0;
    hanoi(4, 'A', 'B', 'C', 1);
    printf("Total moves for n=4: %lld\n\n", move_count);

    // 2) count moves for n = 1..20 and write to CSV for plotting
    FILE *fp = fopen("hanoi_data.csv", "w");
    if (!fp) { perror("fopen"); return 1; }
    fprintf(fp, "n,moves\n");

    printf("=== Moves required vs n ===\n");
    printf("%-6s %-15s %-15s\n", "n", "Moves (sim)", "Formula 2^n-1");
    for (int n = 1; n <= 20; n++) {
        move_count = 0;
        hanoi(n, 'A', 'B', 'C', 0); // don't print, just count
        long long formula = (1LL << n) - 1; // 2^n - 1
        printf("%-6d %-15lld %-15lld\n", n, move_count, formula);
        fprintf(fp, "%d,%lld\n", n, move_count);
    }
    fclose(fp);

    printf("\nData written to hanoi_data.csv (plot n vs moves -- it's exponential!)\n");
    printf("Conclusion: moves grow as T(n) = 2^n - 1, i.e. EXPONENTIAL time\n");
    printf("complexity O(2^n). Doubling n does not double the work --\n");
    printf("it roughly SQUARES it. This is why ToH is intractable for large\n");
    printf("n (e.g. n=64 would take billions of years even at 1M moves/sec).\n");

    return 0;
}
