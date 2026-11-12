/*
 * Q2 - Dynamic Connected Components in a Grid (Number of Islands II)  (DAA Lab-10)
 *
 * Representation : R x C grid flattened to ids  id = r*C + c  (coordinate
 *                  flattening).  No explicit grid is stored: parent[id] == -1
 *                  means "water", otherwise the cell is land inside a DSU.
 * Idea           : count = current #islands.
 *                    add land at id  -> count++
 *                    for each of its 4 neighbours that is land: if union()
 *                    really merges two different sets -> count--
 *                  Re-adding an existing land cell changes nothing.
 *
 * Complexity     : k operations, each <= 4 unions + 1 make-set
 *                  => O(k * alpha(R*C)) time  (~O(k)),  O(R*C) space.
 *                  (Re-running BFS/DFS after every operation: O(k*R*C).)
 *
 * Usage          : ./q2           -> built-in tests + random brute-force check
 *                  ./q2 -i < in   -> input: R C K, then K lines "r c" (0-indexed)
 *                                    prints the island count after each operation
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { int *p, *sz; int R, C, count; } Grid;

static void g_init(Grid *g, int R, int C) {
    g->R = R; g->C = C; g->count = 0;
    g->p = malloc((size_t)R * C * sizeof(int));
    g->sz = calloc((size_t)R * C, sizeof(int));
    for (int i = 0; i < R * C; i++) g->p[i] = -1;
}
static void g_free(Grid *g) { free(g->p); free(g->sz); }

static int find(Grid *g, int x) {                 /* path halving */
    while (g->p[x] != x) { g->p[x] = g->p[g->p[x]]; x = g->p[x]; }
    return x;
}
static int unite(Grid *g, int a, int b) {         /* 1 if merged */
    a = find(g, a); b = find(g, b);
    if (a == b) return 0;
    if (g->sz[a] < g->sz[b]) { int t = a; a = b; b = t; }
    g->p[b] = a; g->sz[a] += g->sz[b];
    return 1;
}

/* returns island count after adding (r,c); out-of-range cells are ignored */
static int add_land(Grid *g, int r, int c) {
    if (r < 0 || r >= g->R || c < 0 || c >= g->C) return g->count;
    int id = r * g->C + c;
    if (g->p[id] != -1) return g->count;          /* already land */
    g->p[id] = id; g->sz[id] = 1; g->count++;
    static const int dr[4] = {-1, 1, 0, 0}, dc[4] = {0, 0, -1, 1};
    for (int k = 0; k < 4; k++) {
        int nr = r + dr[k], nc = c + dc[k];
        if (nr < 0 || nr >= g->R || nc < 0 || nc >= g->C) continue;
        int nid = nr * g->C + nc;
        if (g->p[nid] != -1 && unite(g, id, nid)) g->count--;
    }
    return g->count;
}

/* ---------------- brute force: flood-fill the whole grid ------------------ */
static int brute_count(int R, int C, char land[][12]) {
    char seen[12][12]; memset(seen, 0, sizeof seen);
    int cnt = 0, stk[200];
    for (int i = 0; i < R; i++) for (int j = 0; j < C; j++)
        if (land[i][j] && !seen[i][j]) {
            cnt++; int top = 0; stk[top++] = i * 12 + j; seen[i][j] = 1;
            while (top) { int x = stk[--top], r = x / 12, c = x % 12;
                int dr[4] = {-1,1,0,0}, dc[4] = {0,0,-1,1};
                for (int k = 0; k < 4; k++) { int nr = r+dr[k], nc = c+dc[k];
                    if (nr>=0&&nr<R&&nc>=0&&nc<C&&land[nr][nc]&&!seen[nr][nc]) {
                        seen[nr][nc] = 1; stk[top++] = nr*12+nc; } } }
        }
    return cnt;
}

static int tests = 0, fails = 0;
#define CHECK(c) do { tests++; if (!(c)) { fails++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)

static void self_tests(void) {
    /* classic 3x3 example: 1 1 2 3 1 */
    int ops[][2] = {{0,0},{0,1},{1,2},{2,1},{1,1}}, exp[] = {1,1,2,3,1};
    Grid g; g_init(&g, 3, 3);
    for (int i = 0; i < 5; i++) CHECK(add_land(&g, ops[i][0], ops[i][1]) == exp[i]);
    CHECK(add_land(&g, 1, 1) == 1);               /* duplicate */
    CHECK(add_land(&g, 9, 9) == 1);               /* out of range */
    g_free(&g);
    /* diagonal cells are NOT 4-connected */
    g_init(&g, 2, 2);
    CHECK(add_land(&g, 0, 0) == 1); CHECK(add_land(&g, 1, 1) == 2);
    CHECK(add_land(&g, 0, 1) == 1); g_free(&g);
    /* 1x1 */
    g_init(&g, 1, 1); CHECK(add_land(&g, 0, 0) == 1); g_free(&g);

    srand(777);
    for (int it = 0; it < 3000; it++) {
        int R = 1 + rand() % 8, C = 1 + rand() % 8, K = rand() % 40;
        char land[12][12]; memset(land, 0, sizeof land);
        g_init(&g, R, C);
        for (int i = 0; i < K; i++) {
            int r = rand() % R, c = rand() % C;
            land[r][c] = 1;
            CHECK(add_land(&g, r, c) == brute_count(R, C, land));
        }
        g_free(&g);
    }
}

int main(int argc, char **argv) {
    if (argc > 1 && !strcmp(argv[1], "-i")) {
        int R, C, K;
        if (scanf("%d %d %d", &R, &C, &K) != 3) return 1;
        Grid g; g_init(&g, R, C);
        for (int i = 0; i < K; i++) {
            int r, c; if (scanf("%d %d", &r, &c) != 2) break;
            printf("%d%c", add_land(&g, r, c), i + 1 < K ? ' ' : '\n');
        }
        g_free(&g);
        return 0;
    }
    self_tests();
    printf("Q2: %d checks, %d failed -> %s\n", tests, fails, fails ? "FAIL" : "ALL PASS");
    return fails != 0;
}
