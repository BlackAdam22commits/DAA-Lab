#include <stdio.h>
#include <stdlib.h>

// Linear Search - O(n)
int findPartitionLinear(int arr[], int size, long *count)
{
    *count = 0;

    for (int i = 0; i < size; i++)
    {
        (*count)++;

        if (arr[i] == 1)
            return i;
    }

    return size;
}

// Binary Search - O(log n)
int findPartitionBinary(int arr[], int size, long *count)
{
    *count = 0;

    int left = 0;
    int right = size;

    while (left < right)
    {
        int mid = left + (right - left) / 2;

        (*count)++;

        if (arr[mid] == 1)
            right = mid;
        else
            left = mid + 1;
    }

    return left;
}

// Create an array of 0's followed by 1's
void generateArray(int arr[], int size, int partition)
{
    for (int i = 0; i < size; i++)
    {
        if (i < partition)
            arr[i] = 0;
        else
            arr[i] = 1;
    }
}

int main()
{
    // Small Example
    int sample[] = {0, 0, 0, 0, 1, 1, 1, 1, 1};
    int sampleSize = sizeof(sample) / sizeof(sample[0]);

    long linearComp, binaryComp;

    int linearIndex = findPartitionLinear(sample, sampleSize, &linearComp);
    int binaryIndex = findPartitionBinary(sample, sampleSize, &binaryComp);

    printf("Small Test Case\n");
    printf("-----------------------------\n");
    printf("Linear Search : Index = %d | Comparisons = %ld\n",
           linearIndex, linearComp);

    printf("Binary Search : Index = %d | Comparisons = %ld\n\n",
           binaryIndex, binaryComp);

    // Large Example
    int size = 1000000;
    int partitionPoint = size / 3;

    int *largeArray = (int *)malloc(size * sizeof(int));

    if (largeArray == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    generateArray(largeArray, size, partitionPoint);

    long linearCount, binaryCount;

    int linearResult = findPartitionLinear(largeArray, size, &linearCount);
    int binaryResult = findPartitionBinary(largeArray, size, &binaryCount);

    printf("Large Test Case\n");
    printf("-----------------------------\n");
    printf("Array Size      : %d\n", size);
    printf("Actual Boundary : %d\n\n", partitionPoint);

    printf("Linear Search\n");
    printf("Found Index : %d\n", linearResult);
    printf("Comparisons : %ld\n\n", linearCount);

    printf("Binary Search\n");
    printf("Found Index : %d\n", binaryResult);
    printf("Comparisons : %ld\n\n", binaryCount);

    printf("Conclusion\n");
    printf("----------\n");
    printf("Linear Search requires O(n) comparisons.\n");
    printf("Binary Search utilizes the sorted structure of the array\n");
    printf("to locate the first occurrence of 1 in O(log n) comparisons.\n");
    printf("For an array of %d elements, Binary Search performs\n", size);
    printf("significantly fewer comparisons (%ld vs %ld).\n",
           binaryCount, linearCount);

    free(largeArray);

    return 0;
}
