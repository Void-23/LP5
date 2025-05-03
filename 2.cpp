#include <iostream>
#include <ctime>
#include <cstdlib>
#include <omp.h>
using namespace std;

void bubbleSort(int arr[], int n) 
{
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n - i - 1; ++j)
        {
            if (arr[j] > arr[j + 1]) swap(arr[j], arr[j + 1]);
        }
        
    }
   
}

void parallelBubbleSort(int arr[], int n) 
{
    for (int i = 0; i < n; ++i) 
    {
        int phase = i % 2;
        #pragma omp parallel for
        for (int j = phase; j < n - 1; j += 2)
        {
            if (arr[j] > arr[j + 1]) swap(arr[j], arr[j + 1]);
        }
        
    }
}


void merge(int arr[], int l, int m, int r) 
{
    int n1 = m - l + 1, n2 = r - m;
    int *L = new int[n1], *R = new int[n2];
    for (int i = 0; i < n1; ++i) L[i] = arr[l + i];
    for (int i = 0; i < n2; ++i) R[i] = arr[m + 1 + i];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    delete[] L; 
    delete[] R;
}

void mergeSort(int arr[], int l, int r) 
{
    if (l < r) 
    {
        int m = (l + r) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}


void parallelMergeSort(int arr[], int l, int r) 
{
    if (l < r) 
    {
        int m = (l + r) / 2;
        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, l, m);
            #pragma omp section
            parallelMergeSort(arr, m + 1, r);
        }
        merge(arr, l, m, r);
    }
}


void copyArray(int* src, int* dest, int n) {
 for (int i = 0; i < n; ++i) dest[i] = src[i];
}


int main() 
{
    int n;
    cout << "Enter array size: ";
    cin >> n;

    int *original = new int[n], *arr = new int[n];
    srand(time(0));

    for (int i = 0; i < n; ++i) original[i] = rand() % 10000;
    clock_t start, end;

    // Sequential Bubble Sort
    copyArray(original, arr, n);
    start = clock();
    bubbleSort(arr, n);
    end = clock();
    cout << "Sequential Bubble Sort Time: " << double(end - start) / CLOCKS_PER_SEC << " sec\n";


    // Parallel Bubble Sort
    copyArray(original, arr, n);
    start = clock();
    parallelBubbleSort(arr, n);
    end = clock();
    cout << "Parallel Bubble Sort Time: " << double(end - start) / CLOCKS_PER_SEC << " sec\n";


    // Sequential Merge Sort
    copyArray(original, arr, n);
    start = clock();
    mergeSort(arr, 0, n - 1);
    end = clock();
    cout << "Sequential Merge Sort Time: " << double(end - start) / CLOCKS_PER_SEC << " sec\n";


    // Parallel Merge Sort
    copyArray(original, arr, n);
    start = clock();
    #pragma omp parallel
    {
        #pragma omp single
        parallelMergeSort(arr, 0, n - 1);
    }
    end = clock();
    cout << "Parallel Merge Sort Time: " << double(end - start) / CLOCKS_PER_SEC << " sec\n";

    
    delete[] original;
    delete[] arr;
    return 0;
}