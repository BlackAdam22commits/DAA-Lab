#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 1000
#define STEP_SIZE 100
#define MAX_VALUE 10000

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Optimized Bubble Sort
int bubbleSortOptimized(int arr[], int size)
{
    int comparisons = 0;
    int pass = 0;

    while (pass < size - 1)
    {
        int swapped = 0;

        for (int j = 0; j < size - pass - 1; j++)
        {
            comparisons++;

            if (arr[j] > arr[j + 1])
            {
                swap(&arr[j], &arr[j + 1]);
                swapped = 1;
            }
        }

        if (!swapped)
            break;

        pass++;
    }

    return comparisons;
}

// Standard Bubble Sort
int bubbleSortStandard(int arr[], int size)
{
    int comparisons = 0;

    for (int pass = 0; pass < size - 1; pass++)
    {
        for (int j = 0; j < size - pass - 1; j++)
        {
            comparisons++;

            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
        }
    }

    return comparisons;
}

int main()
{
    srand((unsigned)time(NULL));

    FILE *fp = fopen("comparisons.csv", "w");

    if (fp == NULL)
    {
        printf("Unable to create comparisons.csv\n");
        return 1;
    }

    fprintf(fp, "Size,Optimized,Standard\n");

    int optimizedArray[MAX_SIZE];
    int standardArray[MAX_SIZE];

    for (int size = STEP_SIZE; size <= MAX_SIZE; size += STEP_SIZE)
    {
        for (int i = 0; i < size; i++)
        {
            optimizedArray[i] = rand() % MAX_VALUE;
            standardArray[i] = optimizedArray[i];
        }

        int optimizedComparisons = bubbleSortOptimized(optimizedArray, size);
        int standardComparisons = bubbleSortStandard(standardArray, size);

        fprintf(fp, "%d,%d,%d\n",
                size,
                optimizedComparisons,
                standardComparisons);

        printf("Array Size : %4d | Optimized : %6d | Standard : %6d\n",
               size,
               optimizedComparisons,
               standardComparisons);
    }

    fclose(fp);

    printf("\nCSV file generated successfully: comparisons.csv\n");

    return 0;
}
