#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x;
    int type; // +1 for start, -1 for end
} Event;

// Comparator
int compare(const void *a, const void *b) {
    Event *e1 = (Event*)a;
    Event *e2 = (Event*)b;

    if (e1->x == e2->x)
        return e2->type - e1->type; // start(+1) before end(-1)

    return e1->x - e2->x;
}

void findBestPoint(int l[], int r[], int n) {
    Event *events = (Event*)malloc(2 * n * sizeof(Event));

    // Create events
    for (int i = 0; i < n; i++) {
        events[2*i].x = l[i];
        events[2*i].type = +1;

        events[2*i + 1].x = r[i];
        events[2*i + 1].type = -1;
    }

    // Sort events
    qsort(events, 2*n, sizeof(Event), compare);

    int current = 0, max = 0, best_point = -1;

    // Traverse
    for (int i = 0; i < 2*n; i++) {
        current += events[i].type;

        if (current > max) {
            max = current;
            best_point = events[i].x;
        }
    }

    printf("Maximum overlap = %d at point p = %d\n", max, best_point);

    free(events);
}

int main() {
    int l[] = {10, 20, 50, 15};
    int r[] = {40, 60, 90, 70};
    int n = 4;

    findBestPoint(l, r, n);

    return 0;
}
