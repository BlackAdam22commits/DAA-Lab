/* ============================================================
   Problem 6: The Best Time to Be Alive
   ============================================================
   INPUT REPRESENTATION
   ---------------------
   The book's index gives, for each of n scientists, a pair
   (birth[i], death[i]). We read these n pairs into two arrays.
   Each scientist "covers" the closed year-interval [birth, death].
   We want the year (or year-range) covered by the maximum number
   of overlapping intervals -- this is the classical "maximum
   point/interval overlap" (activity-on-timeline) problem.

   ALGORITHM (event / sweep-line technique)
   -------------------------------------------
   1. Build 2n "events": a (+1) event at year birth[i] (a scientist
      becomes alive) and a (-1) event at year death[i]+1... BUT the
      problem statement adds a subtlety: "if person A died the same
      year person B was born, assume the death happened BEFORE the
      birth" -- i.e. deaths are processed before births that occur
      in the same year. We encode this directly:
         - death events get a small ordering key so that at an
           equal year they are processed first (before births).
   2. Sort all 2n events by (year, type) where type(death) < type(birth).
   3. Sweep through the sorted events, keeping a running counter
      `alive`: increment on a birth event, decrement on a death
      event. Track the maximum value reached and the year(s) at
      which it occurs.
   This single left-to-right sweep finds the true maximum, because
   the count of people alive can only change at a birth or death
   year, and processing deaths-before-births at tied years enforces
   exactly the tie-breaking rule stated in the problem.

   COMPLEXITY
   ----------
   Building events: O(n). Sorting: O(n log n). Sweeping: O(n).
   Overall: O(n log n) time, O(n) space -- optimal, since the input
   itself must be read in O(n) and any correct algorithm must at
   least look at every birth/death pair once; sorting is the
   dominant cost.
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int year;
    int type;   /* 0 = death (processed first), 1 = birth */
} Event;

int cmpEvent(const void *a, const void *b) {
    const Event *e1 = a, *e2 = b;
    if (e1->year != e2->year) return e1->year - e2->year;
    return e1->type - e2->type;   /* death(0) before birth(1) on the same year */
}

int main(void) {
    int n;
    printf("Enter number of scientists (n): ");
    if (scanf("%d", &n) != 1 || n <= 0) { printf("Invalid input\n"); return 1; }

    Event *ev = malloc(2 * n * sizeof(Event));
    printf("Enter birth and death year for each scientist:\n");
    for (int i = 0; i < n; i++) {
        int b, d;
        printf("  Scientist %d (birth death): ", i + 1);
        if (scanf("%d %d", &b, &d) != 2 || b > d) { printf("Invalid input\n"); return 1; }
        ev[2 * i]     = (Event){ b, 1 };  /* birth */
        ev[2 * i + 1] = (Event){ d, 0 };  /* death */
    }

    qsort(ev, 2 * n, sizeof(Event), cmpEvent);

    int alive = 0, best = 0, bestYear = 0;
    for (int i = 0; i < 2 * n; i++) {
        if (ev[i].type == 1) alive++;   /* birth: someone becomes alive */
        else alive--;                   /* death: someone stops being alive */
        if (ev[i].type == 1 && alive > best) {   /* record maxima right after a birth */
            best = alive;
            bestYear = ev[i].year;
        }
    }

    printf("\nMaximum number of scientists alive simultaneously = %d\n", best);
    printf("This maximum is first achieved in the year %d\n", bestYear);

    free(ev);
    return 0;
}
