#include <stdio.h>
#include <stdlib.h>

// Comparator for qsort
int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// Two-pointer for 2-sum
int twoSum(int arr[], int left, int right, int target) {
    while (left < right) {
        int sum = arr[left] + arr[right];
        if (sum == target)
            return 1;
        else if (sum < target)
            left++;
        else
            right--;
    }
    return 0;
}

// k-sum recursive function
int kSum(int arr[], int n, int start, int k, int target) {
    if (k == 2) {
        return twoSum(arr, start, n - 1, target);
    }

    for (int i = start; i < n - k + 1; i++) {
        if (kSum(arr, n, i + 1, k - 1, target - arr[i]))
            return 1;
    }
    return 0;
}

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    int n = 5;
    int k = 3;
    int T = 9;

    // Sort array
    qsort(arr, n, sizeof(int), compare);

    if (kSum(arr, n, 0, k, T))
        printf("Exists\n");
    else
        printf("Does not exist\n");

    return 0;
}
