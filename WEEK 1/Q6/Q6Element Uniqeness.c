#include <stdio.h>

#define MAX_SIZE 100

// Returns 1 if duplicate elements exist, otherwise 0
int checkDuplicates(int numbers[], int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (numbers[i] == numbers[j])
            {
                return 1;
            }
        }
    }

    return 0;
}

// Displays the array elements
void displayArray(int numbers[], int size)
{
    printf("\nEntered Array:\n");

    for (int i = 0; i < size; i++)
    {
        printf("%d ", numbers[i]);
    }

    printf("\n");
}

int main()
{
    int numbers[MAX_SIZE];
    int size;

    printf("===== Element Uniqueness Checker =====\n");

    printf("Enter the number of elements: ");
    scanf("%d", &size);

    if (size <= 0 || size > MAX_SIZE)
    {
        printf("Invalid array size.\n");
        return 1;
    }

    printf("Enter %d integers:\n", size);

    for (int i = 0; i < size; i++)
    {
        scanf("%d", &numbers[i]);
    }

    displayArray(numbers, size);

    if (checkDuplicates(numbers, size))
    {
        printf("\nResult: Duplicate elements found.\n");
    }
    else
    {
        printf("\nResult: All elements are unique.\n");
    }

    printf("\nComplexity Analysis\n");
    printf("-------------------\n");
    printf("Time Complexity  : O(n^2)\n");
    printf("Space Complexity : O(1)\n");

    printf("\nObservation:\n");
    printf("The algorithm compares every element with the remaining elements.\n");
    printf("As the input size increases, the number of comparisons increases quadratically.\n");
    printf("Sorting or hashing techniques can provide more efficient solutions for larger datasets.\n");

    return 0;
}
