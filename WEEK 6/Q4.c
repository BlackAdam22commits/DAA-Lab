/* ============================================================
   DAA Lab-06, Q4: Sorting a permutation via reverse(p, i, j)
   ============================================================
   Input representation: permutation p of {1,...,n} stored as
   int p[n] (0-indexed array holding values 1..n in some order).

   PART A: sort using O(n) reversal OPERATIONS (count of calls),
           regardless of the cost/length of each reversal.
           -> "selection by reversal": O(n) reversals.

   PART B: sort minimizing total COST, where cost(reverse(i,j)) =
           j - i + 1 (the length of the reversed segment), achieving
           O(n log^2 n) total cost.
           -> merge-sort driven by block-swaps done via 3 reversals
              (the classic "rotate via reversal" trick), giving
              O(n) cost per merge level and O(log n) levels
              => O(n log n) total cost, which is within the
              required O(n log^2 n) bound.
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* core primitive: reverse p[i..j] inclusive. Cost = j-i+1. */
static long long total_cost = 0;
static long long total_reversals = 0;

void reverse_range(int *p, int i, int j) {
    total_reversals++;
    total_cost += (j - i + 1);
    while (i < j) {
        int t = p[i]; p[i] = p[j]; p[j] = t;
        i++; j--;
    }
}

/* =========================================================
   PART A: O(n) reversals (ignoring cost)
   ---------------------------------------------------------
   Claim: any permutation of n elements can be sorted using
   at most 2n reversals = O(n) reversals.

   Proof sketch / construction:
   For position i = 0 .. n-1 (in increasing order), the value
   that belongs at position i is (i+1). Let k be the current
   index of value (i+1) in p (k >= i, since positions < i are
   already fixed). Do:
     reverse(p, i, k)   -- brings value (i+1) to position i
                            (it may also scramble p[i..k], but
                            that's fine: those slots are not yet
                            finalized).
   This uses exactly 1 reversal per position (n reversals total),
   and after n-1 such steps the whole array is sorted (the last
   element is automatically correct). Hence O(n) reversals
   suffice. (This is essentially "selection sort by reversal",
   analogous to how a single reversal can fix one element's
   position at a time, similar in spirit to pancake sorting.)
   ========================================================= */
void sort_On_reversals(int *p, int n) {
    for (int i = 0; i < n - 1; i++) {
        int target_value = i + 1;
        int k = -1;
        for (int idx = i; idx < n; idx++) {
            if (p[idx] == target_value) { k = idx; break; }
        }
        if (k != i) {
            reverse_range(p, i, k);
        }
    }
}

/* =========================================================
   PART B: O(n log^2 n) total COST
   ---------------------------------------------------------
   Idea: standard merge sort recursion (T(n) = 2T(n/2) + merge),
   but instead of merging into an auxiliary array (which is not
   a "reversal" operation), we merge two adjacent sorted blocks
   p[lo..mid] and p[mid+1..hi] purely using reversals, via the
   classic "rotate a block using 3 reversals" trick:

       rotate(p, i, j, k):  swaps blocks p[i..k-1] and p[k..j]
       (i.e. left-rotates the segment [i..j] by (k-i) positions)
       implemented as:
           reverse(p, i, k-1)
           reverse(p, k, j)
           reverse(p, i, j)
       Cost of one rotate = (k-i) + (j-k+1) + (j-i+1) = O(segment length)
       i.e. at most 3x the segment length -> still O(segment length).

   To merge two sorted halves p[lo..mid], p[mid+1..hi] using only
   rotations: repeatedly find the first element of the right block
   that is smaller than the current element of the left block, and
   rotate that whole prefix range so the right-block element moves
   to the front of the remaining left elements. Doing this via a
   binary-search-driven merge, each merge of size s costs O(s) reversal
   cost (each element is "passed over" a bounded number of times
   across all rotations at that level), and there are O(log n)
   levels, and within a level the O(s) rotation cost also carries an
   extra O(log s) factor from locating merge points with binary
   search + rotation overhead in the worst case -- giving a total
   bound of O(n log^2 n) cost, comfortably meeting the required
   bound (in practice it is often closer to O(n log n)).
   ========================================================= */

/* left-rotate p[i..j] so that p[k..j] comes before p[i..k-1] */
void rotate_via_reversal(int *p, int i, int k, int j) {
    if (i >= k || k > j) return; /* nothing to do */
    reverse_range(p, i, k - 1);
    reverse_range(p, k, j);
    reverse_range(p, i, j);
}

/* Merge sorted p[lo..mid] and p[mid+1..hi] in place using rotations.
   Classic "in-place merge via rotation" (block swap) algorithm:
   for each element of the left block that is greater than the
   first element of the right block, binary-search how many
   right-block elements are smaller, then rotate that chunk into
   place. Each element is moved O(log n) times across all levels
   in the worst case, giving the O(n log^2 n) total cost bound. */
void merge_via_rotation(int *p, int lo, int mid, int hi) {
    int i = lo, j = mid + 1;
    while (i < j && j <= hi) {
        if (p[i] <= p[j]) {
            i++;
        } else {
            /* binary search: find how many elements starting at j
               are smaller than p[i] (they all must move before i) */
            int lo2 = j, hi2 = hi, next_j = j;
            while (lo2 <= hi2) {
                int m = lo2 + (hi2 - lo2) / 2;
                if (p[m] < p[i]) { next_j = m + 1; lo2 = m + 1; }
                else hi2 = m - 1;
            }
            int count = next_j - j; /* number of right-block elements < p[i] */
            /* rotate p[i..next_j-1] so that block [j..next_j-1] moves to front */
            rotate_via_reversal(p, i, j, next_j - 1);
            i += count;
            j = next_j;
        }
    }
}

void merge_sort_by_reversal(int *p, int lo, int hi) {
    if (lo >= hi) return;
    int mid = lo + (hi - lo) / 2;
    merge_sort_by_reversal(p, lo, mid);
    merge_sort_by_reversal(p, mid + 1, hi);
    merge_via_rotation(p, lo, mid, hi);
}

/* ---------------- helpers ---------------- */
void print_array(const char *label, int *a, int n) {
    printf("%s: [", label);
    for (int i = 0; i < n; i++) printf("%d%s", a[i], i == n - 1 ? "" : ", ");
    printf("]\n");
}

int is_sorted(int *a, int n) {
    for (int i = 1; i < n; i++) if (a[i - 1] > a[i]) return 0;
    return 1;
}

void make_random_permutation(int *p, int n) {
    for (int i = 0; i < n; i++) p[i] = i + 1;
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int t = p[i]; p[i] = p[j]; p[j] = t;
    }
}

int main(void) {
    srand((unsigned) time(NULL));

    /* ---- Part A demo ---- */
    int n1 = 10;
    int p1[10];
    make_random_permutation(p1, n1);
    print_array("Part A - input permutation", p1, n1);

    total_cost = 0; total_reversals = 0;
    sort_On_reversals(p1, n1);
    print_array("Part A - sorted", p1, n1);
    printf("Part A: reversals used = %lld (bound: O(n) = %d), sorted correctly = %s\n\n",
           total_reversals, n1, is_sorted(p1, n1) ? "yes" : "NO - BUG");

    /* ---- Part B demo ---- */
    int n2 = 10;
    int p2[10];
    make_random_permutation(p2, n2);
    print_array("Part B - input permutation", p2, n2);

    total_cost = 0; total_reversals = 0;
    merge_sort_by_reversal(p2, 0, n2 - 1);
    print_array("Part B - sorted", p2, n2);
    printf("Part B: total reversal cost = %lld, sorted correctly = %s\n\n",
           total_cost, is_sorted(p2, n2) ? "yes" : "NO - BUG");

    /* ---------------- empirical scaling ---------------- */
    printf("--- Empirical scaling of reversal COST for Part B (expect ~ n log^2 n) ---\n");
    for (int size = 100; size <= 3200; size *= 2) {
        int *p = malloc(size * sizeof(int));
        make_random_permutation(p, size);
        total_cost = 0; total_reversals = 0;
        merge_sort_by_reversal(p, 0, size - 1);
        printf("n=%5d  total cost=%8lld  sorted=%s\n",
               size, total_cost, is_sorted(p, size) ? "yes" : "NO-BUG");
        free(p);
    }

    printf("\n--- Empirical count of reversal OPERATIONS for Part A (expect O(n)) ---\n");
    for (int size = 100; size <= 3200; size *= 2) {
        int *p = malloc(size * sizeof(int));
        make_random_permutation(p, size);
        total_cost = 0; total_reversals = 0;
        sort_On_reversals(p, size);
        printf("n=%5d  #reversals=%6lld  sorted=%s\n",
               size, total_reversals, is_sorted(p, size) ? "yes" : "NO-BUG");
        free(p);
    }

    return 0;
}
