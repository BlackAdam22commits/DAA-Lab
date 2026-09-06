/* ============================================================
   DAA Lab-05, Q3: Quicksort of N random elements stored in a file
   ============================================================
   Input representation: a text file "input.txt" containing N
   space/newline-separated integers (generated randomly by this
   same program). The program reads the whole file into an
   in-memory array, sorts it with Quicksort, and writes the
   sorted result to "output.txt".

   ---------------- Complexity ----------------
   Quicksort (Lomuto/Hoare partitioning), average case: O(n log n)
     - Partitioning an array of size n costs O(n).
     - On average a random pivot splits the array roughly in half,
       giving T(n) = 2T(n/2) + O(n) => O(n log n) by the Master
       Theorem.
   Worst case: O(n^2)
     - If the pivot is consistently the min or max element (e.g.
       adversarial or already-sorted input with a naive
       "always pick first element" pivot), the split is 1 vs n-1:
       T(n) = T(n-1) + O(n) => O(n^2).
     - Using a RANDOM pivot (as done here) makes this worst case
       exponentially unlikely for any fixed input, so expected
       performance is O(n log n) in practice.
   Space: O(log n) auxiliary (recursion stack for in-place
   partitioning), O(n) for the input array itself.
   File I/O: reading/writing N integers is an additional O(n)
   cost on top of the sort.
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int i, int j) { int t = a[i]; a[i] = a[j]; a[j] = t; }

/* Lomuto partition with a random pivot -- O(hi-lo+1) per call */
int partition(int *a, int lo, int hi) {
    int pivot_index = lo + rand() % (hi - lo + 1);
    swap(a, pivot_index, hi);
    int pivot_val = a[hi];
    int store = lo;
    for (int i = lo; i < hi; i++) {
        if (a[i] < pivot_val) { swap(a, i, store); store++; }
    }
    swap(a, store, hi);
    return store;
}

void quicksort(int *a, int lo, int hi) {
    while (lo < hi) {
        int p = partition(a, lo, hi);
        /* recurse into the smaller side, iterate on the larger side,
           to keep the recursion stack at O(log n) in the worst case */
        if (p - lo < hi - p) {
            quicksort(a, lo, p - 1);
            lo = p + 1;
        } else {
            quicksort(a, p + 1, hi);
            hi = p - 1;
        }
    }
}

/* ---------------- file helpers ---------------- */
void generate_input_file(const char *filename, int n, int max_val) {
    FILE *f = fopen(filename, "w");
    if (!f) { perror("fopen (write input)"); exit(1); }
    for (int i = 0; i < n; i++)
        fprintf(f, "%d\n", rand() % max_val);
    fclose(f);
}

int *read_ints_from_file(const char *filename, int *out_n) {
    FILE *f = fopen(filename, "r");
    if (!f) { perror("fopen (read)"); exit(1); }

    int capacity = 1024, n = 0;
    int *a = malloc(capacity * sizeof(int));
    int value;
    while (fscanf(f, "%d", &value) == 1) {
        if (n == capacity) { capacity *= 2; a = realloc(a, capacity * sizeof(int)); }
        a[n++] = value;
    }
    fclose(f);
    *out_n = n;
    return a;
}

void write_ints_to_file(const char *filename, int *a, int n) {
    FILE *f = fopen(filename, "w");
    if (!f) { perror("fopen (write output)"); exit(1); }
    for (int i = 0; i < n; i++) fprintf(f, "%d\n", a[i]);
    fclose(f);
}

int is_sorted(int *a, int n) {
    for (int i = 1; i < n; i++) if (a[i - 1] > a[i]) return 0;
    return 1;
}

int main(int argc, char **argv) {
    srand((unsigned) time(NULL));

    int N = 20; /* default demo size; can pass N as argv[1] */
    if (argc > 1) N = atoi(argv[1]);

    const char *infile = "input.txt";
    const char *outfile = "output.txt";

    printf("Generating %d random integers into '%s' ...\n", N, infile);
    generate_input_file(infile, N, 100000);

    int n;
    int *a = read_ints_from_file(infile, &n);
    printf("Read %d integers from '%s'.\n", n, infile);

    if (n <= 30) {
        printf("Before sort: [");
        for (int i = 0; i < n; i++) printf("%d%s", a[i], i == n - 1 ? "" : ", ");
        printf("]\n");
    }

    clock_t t0 = clock();
    quicksort(a, 0, n - 1);
    clock_t t1 = clock();

    if (n <= 30) {
        printf("After sort:  [");
        for (int i = 0; i < n; i++) printf("%d%s", a[i], i == n - 1 ? "" : ", ");
        printf("]\n");
    }

    printf("Sorted correctly: %s\n", is_sorted(a, n) ? "yes" : "NO - BUG");
    printf("Quicksort time for n=%d: %.6fs\n", n, (double)(t1 - t0) / CLOCKS_PER_SEC);

    write_ints_to_file(outfile, a, n);
    printf("Sorted output written to '%s'.\n", outfile);

    free(a);

    /* ---------------- empirical timing for larger n ---------------- */
    printf("\n--- Empirical timing (expect ~O(n log n)) ---\n");
    for (int size = 50000; size <= 400000; size *= 2) {
        generate_input_file("tmp_input.txt", size, 1000000000);
        int nn;
        int *arr = read_ints_from_file("tmp_input.txt", &nn);

        clock_t s0 = clock();
        quicksort(arr, 0, nn - 1);
        clock_t s1 = clock();

        printf("n=%7d  quicksort time: %.6fs  sorted=%s\n",
               nn, (double)(s1 - s0) / CLOCKS_PER_SEC, is_sorted(arr, nn) ? "yes" : "NO-BUG");
        free(arr);
    }
    remove("tmp_input.txt");

    return 0;
}
