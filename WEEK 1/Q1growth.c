/* DAA Q1: place 12 functions in increasing order of growth.
 * Produced two ways and cross-checked:
 *   (A) NUMERICAL   merge sort by computed value at n = 1e2..1e8
 *   (B) THEORETICAL merge sort by growth class only (constants/lower
 *                   order terms cannot influence the result)
 * Build: gcc q1_growth_order.c -lm -Wall -Wextra
 */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>
 
 
#define NUM_FUNCS 12
#define NUM_TESTS 7
#define BAR_WIDTH 52
#define OVF_LOG10 305.0   /* past this many decades, report INF */
#define WHY_LEN   96
 
#define CAT_RECIPROCAL   0
#define CAT_LOGARITHMIC  1
#define CAT_POLYNOMIAL   2
#define CAT_LINEARITHMIC 3
#define CAT_SUPERPOLY    4
#define CAT_EXPONENTIAL  5
 
typedef struct {
    char   name[100];
    int    category;
    double exponent;
    double (*evaluate)(double);
    double (*magnitude)(double);
    double value, mag;
} Function;
typedef int (*Comparator)(const Function *, const Function *);
 
static double safe_pow10(double x)  { return (x > OVF_LOG10) ? HUGE_VAL : pow(10.0, x); }
static double log10_floor(double v) { return (v > 0.0) ? log10(v) : -DBL_MAX / 4.0; }
 
static double f_reciprocal(double n) { return 1.0 / n; }
static double m_reciprocal(double n) { return -log10(n); }
static double f_log2v(double n)      { return log2(n); }
static double m_log2v(double n)      { return log10_floor(log2(n)); }
static double f_sqrt12(double n)     { return 12.0 * sqrt(n); }
static double m_sqrt12(double n)     { return log10(12.0) + 0.5 * log10(n); }
static double f_sqrt50(double n)     { return 50.0 * pow(n, 0.5); }
static double m_sqrt50(double n)     { return log10(50.0) + 0.5 * log10(n); }
static double f_pow051(double n)     { return pow(n, 0.51); }
static double m_pow051(double n)     { return 0.51 * log10(n); }
static double f_lin2p32(double n)    { return pow(2.0, 32.0) * n; }
static double m_lin2p32(double n)    { return 32.0 * log10(2.0) + log10(n); }
static double f_nlogn(double n)      { return n * log2(n); }
static double m_nlogn(double n)      { return log10(n) + log10_floor(log2(n)); }
static double f_quad100(double n)    { return 100.0 * n * n + 6.0 * n; }
static double m_quad100(double n)    { return log10(n) + log10(100.0 * n + 6.0); }
static double f_quadm324(double n)   { return n * n - 324.0; }
static double m_quadm324(double n)   { return log10_floor(n * n - 324.0); }
static double f_cubic2(double n)     { return 2.0 * n * n * n; }
static double m_cubic2(double n)     { return log10(2.0) + 3.0 * log10(n); }
static double f_npowlogn(double n)   { return safe_pow10(log2(n) * log10(n)); }
static double m_npowlogn(double n)   { return log2(n) * log10(n); }
static double f_exp3(double n)       { return safe_pow10(n * log10(3.0)); }
static double m_exp3(double n)       { return n * log10(3.0); }
 
static Function funcs[NUM_FUNCS];
static const double test_n[NUM_TESTS] = { 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8 };
 
static void setFunction(int i, const char *name, int cat, double expo,
                         double (*ev)(double), double (*mg)(double))
{
    Function *f = &funcs[i];
    snprintf(f->name, sizeof f->name, "%s", name);
    f->category = cat;  f->exponent = expo;
    f->evaluate = ev;   f->magnitude = mg;
    f->value = 0.0;     f->mag = 0.0;
}
static void buildFunctionTable(void)
{
    setFunction(0,  "1/n",         CAT_RECIPROCAL,  -1.0, f_reciprocal, m_reciprocal);
    setFunction(1,  "log2(n)",     CAT_LOGARITHMIC,  0.0, f_log2v,      m_log2v);
    setFunction(2,  "12*sqrt(n)",  CAT_POLYNOMIAL,   0.5, f_sqrt12,     m_sqrt12);
    setFunction(3,  "50*n^0.5",    CAT_POLYNOMIAL,   0.5, f_sqrt50,     m_sqrt50);
    setFunction(4,  "n^0.51",      CAT_POLYNOMIAL,  0.51, f_pow051,     m_pow051);
    setFunction(5,  "(2^32)*n",    CAT_POLYNOMIAL,   1.0, f_lin2p32,    m_lin2p32);
    setFunction(6,  "n*log2(n)",   CAT_LINEARITHMIC, 1.0, f_nlogn,      m_nlogn);
    setFunction(7,  "100*n^2+6*n", CAT_POLYNOMIAL,   2.0, f_quad100,    m_quad100);
    setFunction(8,  "n^2-324",     CAT_POLYNOMIAL,   2.0, f_quadm324,   m_quadm324);
    setFunction(9,  "2*n^3",       CAT_POLYNOMIAL,   3.0, f_cubic2,     m_cubic2);
    setFunction(10, "n^(log2 n)",  CAT_SUPERPOLY,    0.0, f_npowlogn,   m_npowlogn);
    setFunction(11, "3^n",         CAT_EXPONENTIAL,  3.0, f_exp3,       m_exp3);
}
 
static Function mergeBuf[NUM_FUNCS];
static void mergeHalves(Function a[], int lo, int mid, int hi, Comparator cmp)
{
    int i = lo, j = mid + 1, k = lo;
    while (i <= mid && j <= hi)
        mergeBuf[k++] = (cmp(&a[i], &a[j]) <= 0) ? a[i++] : a[j++];
    while (i <= mid) mergeBuf[k++] = a[i++];
    while (j <= hi)  mergeBuf[k++] = a[j++];
    for (k = lo; k <= hi; k++) a[k] = mergeBuf[k];
}
static void mergeSortRange(Function a[], int lo, int hi, Comparator cmp)
{
    if (lo >= hi) return;
    int mid = lo + (hi - lo) / 2;
    mergeSortRange(a, lo, mid, cmp);
    mergeSortRange(a, mid + 1, hi, cmp);
    mergeHalves(a, lo, mid, hi, cmp);
}
static void mergeSort(Function a[], int n, Comparator cmp) { mergeSortRange(a, 0, n - 1, cmp); }
 
static int compareNumeric(const Function *a, const Function *b)
{
    if (a->mag < b->mag) return -1;
    if (a->mag > b->mag) return  1;
    return 0;
}
 
static int tier(int category)
{
    switch (category) {
        case CAT_RECIPROCAL:   return 0;
        case CAT_LOGARITHMIC:  return 1;
        case CAT_POLYNOMIAL:
        case CAT_LINEARITHMIC: return 2;
        case CAT_SUPERPOLY:    return 3;
        default:                return 4;
    }
}
static int compareAsymptotic(const Function *a, const Function *b)
{
    int ta = tier(a->category), tb = tier(b->category);
    if (ta != tb) return (ta < tb) ? -1 : 1;
    if (ta == 2) {
        if (a->exponent < b->exponent) return -1;
        if (a->exponent > b->exponent) return  1;
        if (a->category != b->category)
            return (a->category == CAT_LINEARITHMIC) ? 1 : -1;
    }
    return 0;
}
 
static double values[NUM_FUNCS][NUM_TESTS];
static int    rankAt[NUM_FUNCS][NUM_TESTS];
static int    orderAt[NUM_TESTS][NUM_FUNCS];
 
static int indexOfName(const char *name)
{
    for (int i = 0; i < NUM_FUNCS; i++)
        if (strcmp(funcs[i].name, name) == 0) return i;
    return -1;
}
static void formatValue(double v, char *out, size_t cap)
{
    if (isinf(v) || isnan(v)) snprintf(out, cap, "INF");
    else                      snprintf(out, cap, "%.4e", v);
}
static void evaluateAll(void)
{
    for (int f = 0; f < NUM_FUNCS; f++)
        for (int t = 0; t < NUM_TESTS; t++)
            values[f][t] = funcs[f].evaluate(test_n[t]);
}
static void prepareWork(Function work[], int t)
{
    for (int f = 0; f < NUM_FUNCS; f++) {
        work[f]       = funcs[f];
        work[f].value = values[f][t];
        work[f].mag   = funcs[f].magnitude(test_n[t]);
    }
}
 
static int stableFrom = -1;
static int firstDifference(int t1, int t2)
{
    for (int i = 0; i < NUM_FUNCS; i++)
        if (orderAt[t1][i] != orderAt[t2][i]) return i;
    return -1;
}
static int sameOrder(int t1, int t2) { return firstDifference(t1, t2) < 0; }
 
static void runNumericalRankings(void)
{
    Function work[NUM_FUNCS];
    char buf[32];
    for (int t = 0; t < NUM_TESTS; t++) {
        prepareWork(work, t);
        mergeSort(work, NUM_FUNCS, compareNumeric);
        for (int i = 0; i < NUM_FUNCS; i++) {
            int canonical = indexOfName(work[i].name);
            orderAt[t][i] = canonical;
            rankAt[canonical][t] = i + 1;
        }
        printf("\nn = %.0f\n"
               "Rank  %-16s Value\n"
               "-------------------------------------------\n",
               test_n[t], "Function");
        for (int i = 0; i < NUM_FUNCS; i++) {
            formatValue(work[i].value, buf, sizeof buf);
            printf("%3d   %-16s %s\n", i + 1, work[i].name, buf);
        }
        if (t == 0)
            printf("(first test - no previous ordering to compare)\n");
        else if (sameOrder(t, t - 1))
            printf("Ordering unchanged.\n");
        else {
            int d = firstDifference(t, t - 1);
            printf("Ordering changed at rank %d: %s took the place of %s.\n",
                   d + 1, funcs[orderAt[t][d]].name, funcs[orderAt[t - 1][d]].name);
        }
    }
    for (int t = 0; t < NUM_TESTS; t++) {
        int holds = 1;
        for (int u = t + 1; u < NUM_TESTS; u++)
            if (!sameOrder(t, u)) { holds = 0; break; }
        if (holds) { stableFrom = t; break; }
    }
}
 
static void printRankMovementTable(void)
{
    printf("\nRANK MOVEMENT BETWEEN CONSECUTIVE TESTS\n%-16s", "Function");
    for (int t = 0; t < NUM_TESTS; t++) printf("  1e%-3d", (int)log10(test_n[t]));
    printf("   Moves\n");
    for (int f = 0; f < NUM_FUNCS; f++) {
        int moves = 0;
        printf("%-16s", funcs[f].name);
        for (int t = 0; t < NUM_TESTS; t++) {
            printf("  %-5d", rankAt[f][t]);
            if (t > 0 && rankAt[f][t] != rankAt[f][t - 1]) moves++;
        }
        printf("   %d\n", moves);
    }
}
 
static Function finalOrder[NUM_FUNCS];
static void explainFunction(const Function *f, char *out, size_t cap)
{
    switch (f->category) {
    case CAT_RECIPROCAL:   snprintf(out, cap, "decays towards 0, below everything else"); break;
    case CAT_LOGARITHMIC:  snprintf(out, cap, "grows slower than n^c for every c > 0"); break;
    case CAT_LINEARITHMIC: snprintf(out, cap, "n^1 with an extra log2 n factor"); break;
    case CAT_SUPERPOLY:    snprintf(out, cap, "exponent log2 n grows, beats every fixed power"); break;
    case CAT_EXPONENTIAL:  snprintf(out, cap, "base %.0f^n, log of it linear in n: steepest here", f->exponent); break;
    default:               snprintf(out, cap, "polynomial of degree %g, constants dropped", f->exponent);
    }
}
static void runTheoreticalSort(void)
{
    for (int i = 0; i < NUM_FUNCS; i++) finalOrder[i] = funcs[i];
    mergeSort(finalOrder, NUM_FUNCS, compareAsymptotic);
}
static void printFinalOrder(void)
{
    char why[WHY_LEN];
    printf("\nFINAL ASYMPTOTIC ORDER (merge sort on growth class only)\n\n");
    for (int i = 0; i < NUM_FUNCS; i++) {
        explainFunction(&finalOrder[i], why, sizeof why);
        printf("   %-16s   [%s]\n", finalOrder[i].name, why);
        if (i + 1 < NUM_FUNCS)
            printf("%s", compareAsymptotic(&finalOrder[i], &finalOrder[i + 1]) == 0
                   ? "   =  (same growth class, neither dominates)\n" : "   |\n   v\n");
    }
}
 
static void printAsciiGraph(void)
{
    double score[NUM_FUNCS], lo = DBL_MAX, hi = -DBL_MAX;
    double nbig = test_n[NUM_TESTS - 1];
    printf("\nASCII GROWTH VISUALISATION at n = %.0e\n"
           "(bar ~ log10 of number of decades in f(n))\n\n", nbig);
    for (int f = 0; f < NUM_FUNCS; f++) {
        score[f] = log10(funcs[f].magnitude(nbig) + 10.0);
        if (score[f] < lo) lo = score[f];
        if (score[f] > hi) hi = score[f];
    }
    for (int i = 0; i < NUM_FUNCS; i++) {
        int f = indexOfName(finalOrder[i].name);
        int len = 1 + (int)((score[f] - lo) / (hi - lo) * (BAR_WIDTH - 1));
        printf("%-16s ", funcs[f].name);
        while (len-- > 0) printf("\u2588");
        putchar('\n');
    }
    printf("\nDecades of f(%.0e): from %.1f (1/n) up to %.3e (3^n).\n",
           nbig, funcs[indexOfName("1/n")].magnitude(nbig),
           funcs[indexOfName("3^n")].magnitude(nbig));
}
 
static void printConclusion(void)
{
    int mismatch = 0, lastTest = NUM_TESTS - 1;
    printf("\nCONCLUSION\n\nNumerical ordering at n = %.0e:\n   ", test_n[lastTest]);
    for (int i = 0; i < NUM_FUNCS; i++)
        printf("%s%s", funcs[orderAt[lastTest][i]].name, i + 1 < NUM_FUNCS ? " < " : "\n");
 
    if (stableFrom < 0)
        printf("\nStable ordering: still moving at n = %.0e, not settled in tested range.\n",
               test_n[lastTest]);
    else if (stableFrom == 0)
        printf("\nStable ordering: unchanged from n = %.0e onwards.\n", test_n[0]);
    else
        printf("\nOrdering stabilized after n = %.0f\n", test_n[stableFrom]);
 
    printf("\nTheoretical ordering:\n   ");
    for (int i = 0; i < NUM_FUNCS; i++) {
        printf("%s", finalOrder[i].name);
        if (i + 1 < NUM_FUNCS)
            printf("%s", compareAsymptotic(&finalOrder[i], &finalOrder[i + 1]) == 0 ? " = " : " < ");
    }
    printf("\n");
 
    for (int i = 0; i < NUM_FUNCS; i++)
        if (indexOfName(finalOrder[i].name) != orderAt[lastTest][i]) mismatch++;
    printf("\nAgreement: %d of %d positions match at n = %.0e.%s\n",
           NUM_FUNCS - mismatch, NUM_FUNCS, test_n[lastTest],
           mismatch ? "  Gaps are pairs whose crossover lies beyond 1e8." : "");
 
    printf("\nWhy constants disappear:\n"
           "  12*sqrt(n) and 50*n^0.5 keep a fixed ratio of 50/12 = %.4f forever,\n"
           "  so they share a Theta class. (2^32)*n is still only Theta(n); n*log2(n)\n"
           "  overtakes it once log2(n) > 2^32.\n"
           "\nWhy lower order terms disappear:\n"
           "  In 100*n^2+6*n, the 6*n share is %.3e of the total at n = 1e8, already\n"
           "  negligible. n^2-324's -324 matters only while n is small. Both are\n"
           "  Theta(n^2).\n"
           "\nWhy n^(log2 n) beats any polynomial:\n"
           "  log2(n^log2 n) = (log2 n)^2 is quadratic in log2 n, while log2(n^k) = k*log2 n\n"
           "  is linear; the crossing happens once log2(n) > k, for any fixed k.\n"
           "\nWhy 3^n dominates everything:\n"
           "  log10(3^n) = %.4f*n grows linearly in n; the runner-up n^(log2 n) only\n"
           "  grows as log2(n)*log10(n). At n = 1e8 that is %.3e decades vs %.3e.\n",
           50.0 / 12.0, 6.0 / (100.0 * 1e8 + 6.0),
           log10(3.0), 1e8 * log10(3.0), m_npowlogn(1e8));
}
 
/* Dumps the full (function x test_n) magnitude matrix to CSV so the growth
 * curves can be plotted properly (gnuplot/Excel/Python) instead of relying
 * on the terminal ASCII bar chart. One row per function, one column per n. */
static void exportGraphCSV(const char *path)
{
    FILE *fp = fopen(path, "w");
    if (!fp) { perror("exportGraphCSV"); return; }
 
    fprintf(fp, "function,category");
    for (int t = 0; t < NUM_TESTS; t++) fprintf(fp, ",n=%.0f", test_n[t]);
    fprintf(fp, "\n");
 
    for (int f = 0; f < NUM_FUNCS; f++) {
        fprintf(fp, "%s,%d", funcs[f].name, funcs[f].category);
        for (int t = 0; t < NUM_TESTS; t++)
            fprintf(fp, ",%.6f", funcs[f].magnitude(test_n[t])); /* log10(value) */
        fprintf(fp, "\n");
    }
    fclose(fp);
    printf("\nGraph data written to %s (log10 magnitude per function per n).\n", path);
}
 
static void printComplexity(void)
{
    printf("\nTIME AND SPACE COMPLEXITY\n"
           "F = %d functions, T = %d test values.\n"
           "Evaluation : O(F*T) = O(%d) calls.\n"
           "Merge sort : O(F log F) per sort, run T times (numeric) + once (theoretical)\n"
           "             => O((T+1) * F log F).\n"
           "Overall time  : O(T * F log F).\n"
           "Space         : O(F*T) for the cached matrix + O(F) merge buffer/working copy.\n",
           NUM_FUNCS, NUM_TESTS, NUM_FUNCS * NUM_TESTS);
}
 
int main(void)
{
    printf("DAA Q1 : ORDERING FUNCTIONS BY RATE OF GROWTH\n"
           "Two independent rankings of the same %d functions:\n"
           "  1. merge sort on values computed at n = 1e2..1e8\n"
           "  2. merge sort on growth class only\n", NUM_FUNCS);
 
    buildFunctionTable();
    evaluateAll();
 
    printf("\nPASS 1 : NUMERICAL RANKING PER TEST\n");
    runNumericalRankings();
    printRankMovementTable();
 
    printf("\nPASS 2 : THEORETICAL RANKING\n");
    runTheoreticalSort();
    printFinalOrder();
    printAsciiGraph();
    exportGraphCSV("growth_data.csv");
    printConclusion();
    printComplexity();
    return 0;
}
