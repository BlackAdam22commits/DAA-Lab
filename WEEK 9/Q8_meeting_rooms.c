/*
 * Q8 - Minimum number of meeting rooms
 *
 * Intervals are half-open [s,e): a meeting ending at t and one starting at t may
 * share a room.
 *
 * INPUT REPRESENTATION: two separate arrays S[] (starts) and E[] (ends), each
 *   sorted independently.  The pairing of start to end is irrelevant for the
 *   count of simultaneously active meetings.
 *
 * ALGORITHM (sweep line): walk through starts in order with a pointer into ends.
 *   For each start s: if the earliest unfinished end e <= s, that room is reused
 *   (advance end pointer); otherwise a new room is needed (rooms++).
 *   Answer = maximum number of overlapping intervals = interval-graph chromatic
 *   number (greedy colouring by start time is optimal).
 *
 * COMPLEXITY: two sorts O(n log n) + one linear merge O(n)  => O(n log n) time,
 *   O(n) space.
 *
 * VALIDATION: brute force - maximum, over all start points p, of the number of
 *   intervals with s <= p < e  (O(n^2)).
 * Build: gcc -O2 -Wall -o q8 Q8_meeting_rooms.c
 * Run: ./q8 | ./q8 stdin  (input: n then n lines "s e")
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXN 100005
static int cmp(const void *a, const void *b) { int x = *(const int *)a, y = *(const int *)b; return (x > y) - (x < y); }
static int min_rooms(const int *s, const int *e, int n) {
    int *S = malloc(n * sizeof(int)), *E = malloc(n * sizeof(int)); memcpy(S, s, n * sizeof(int)); memcpy(E, e, n * sizeof(int));
    qsort(S, n, sizeof(int), cmp); qsort(E, n, sizeof(int), cmp);
    int rooms = 0, j = 0;
    for (int i = 0; i < n; i++) { if (E[j] <= S[i]) j++; else rooms++; }
    free(S); free(E); return rooms;
}
static int brute(const int *s, const int *e, int n) {
    int best = 0; for (int i = 0; i < n; i++) { int c = 0; for (int k = 0; k < n; k++) if (s[k] <= s[i] && s[i] < e[k]) c++; if (c > best) best = c; } return best;
}
int main(int argc, char **argv) {
    static int s[MAXN], e[MAXN]; int n;
    if (argc > 1 && !strcmp(argv[1], "stdin")) { if (scanf("%d", &n) != 1) return 1; for (int i = 0; i < n; i++) if (scanf("%d %d", &s[i], &e[i]) != 2) return 1; }
    else { n = 3; int a[] = {0, 5, 15}, b[] = {30, 10, 20}; memcpy(s, a, sizeof a); memcpy(e, b, sizeof b); }
    printf("Minimum rooms = %d\n", min_rooms(s, e, n));
    if (argc > 1) return 0;
    srand(6); int bad = 0, T = 20000;
    for (int tc = 0; tc < T; tc++) { int m = 1 + rand() % 12, a[16], b[16]; for (int i = 0; i < m; i++) { a[i] = rand() % 20; b[i] = a[i] + 1 + rand() % 10; }
        if (min_rooms(a, b, m) != brute(a, b, m)) bad++; }
    printf("Validation vs max-overlap brute force: %d/%d mismatches\n", bad, T);
    return 0;
}
