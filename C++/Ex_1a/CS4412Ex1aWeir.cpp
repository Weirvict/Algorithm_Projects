#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>
#include <chrono> // For timing

using namespace std;

/**
 *
 * @author Victoria Weir
 * @class CS4412
 * @date 8-28-2023
 * @details This code is based off of mergeSort code in Note 1.6
 *
 */


void doMerge(int *a, int low, int high, int mid);
void mergeSort(int *a, int low, int high);

void mergeSort(int *a, int low, int high) {
    int mid;
    if (low < high) {
        mid=(high+low)/2;
        mergeSort(a,low,mid);
        mergeSort(a,mid+1,high);
        doMerge(a,low,high,mid);
    }
}
//merge already sorted sublists (low-mid, mid+1to high)
// into single list from low to high
void doMerge(int *a, int low, int high, int mid) {
    //Note to myself: low is first index, high is last index, mid is pivot points

    // determining the sizes of the subarrays that need to be merged
    int left_size = mid - low + 1;
    int right_size = high - mid;

    // Creating duplicate arrays
    int left_half[left_size];
    int right_half[right_size];

    for (int i = 0; i < left_size; ++i)
        left_half[i] = a[low + i];
    for (int i = 0; i < right_size; ++i)
        right_half[i] = a[mid + 1 + i];

    //indexes
    int left_idx = 0, right_idx = 0, merged_idx = low;

    //similar to the original codes
    while (left_idx < left_size && right_idx < right_size) {
        if (left_half[left_idx] <= right_half[right_idx]) {
            a[merged_idx] = left_half[left_idx];
            ++left_idx;
        } else{
            a[merged_idx] = right_half[right_idx];
            ++right_idx;
        }
        ++merged_idx;
    }

    while (left_idx < left_size){
        a[merged_idx] = left_half[left_idx];
        ++left_idx;
        ++merged_idx;
    }

    while (right_idx < right_size) {
        a[merged_idx] = right_half[right_idx];
        ++right_idx;
        ++merged_idx;
    }
}

    //Original Code for merge sort
//    int i, j, k, temp[50];
//    k = low;
//    i = low;
//    j = mid + 1;
//    while (i <= mid && j <= high)
//        if (a[i] < a[j])
//            temp[k++] = a[i++];
//        else
//            temp[k++] = a[j++];
//
//    while (i <= mid) temp[k++] = a[i++];
//    while (j <= high) temp[k++] = a[j++];
//    for (i=low;i<k;i++) a[i] = temp[i];

// Alternative 3
int getRandomPivot(int low, int high){
    return low + rand() % (high - low +1);
}

void nonRecursiveQuickSort(int list[], int size){
    const int maxStackSize = 10000; // Maximum size of the stack
    int stack[maxStackSize * 2];  // Stack for storing subarray ranges
    int topStack = -1;                 // Top of the stack

    stack[++topStack] = 0;             // Initial low index
    stack[++topStack] = size - 1;      // Initial high index

    while (topStack >= 0) {
        int high = stack[topStack--];
        int low = stack[topStack--];

        // Alternative 1 (Pivot is first element)
//        int pivot = list[low]; // uses the first element as pivot
//        int i = low;

        //Alternative 3 (Pivot is a random element)
//        int randPivot = getRandomPivot(low,high);
//        int pivot = list[randPivot];
//        int i = low - 1;

       // Original Code
        int pivot = list[high];
        int i = low - 1;

        for (int j = low; j <= high - 1; ++j) {
            if (list[j] < pivot) {
                ++i;
                swap(list[i], list[j]);
            }
        }

        swap(list[i + 1], list[high]);
        int pivotIndex = i + 1;

        if (pivotIndex - 1 > low) {
            stack[++topStack] = low;
            stack[++topStack] = pivotIndex - 1;
        }

        if (pivotIndex + 1 < high) {
            stack[++topStack] = pivotIndex + 1;
            stack[++topStack] = high;
        }
    }
}

//Alternate 2 (Pivot is first, middle, and last elements)
void triPivNonRecursQuickSort(int list[], int size) {
    int maxStackSize = 10000; // Maximum size of the stack

    // Arrays that store the lower and upper sub arrays that need to be sorted
    int lowerStack[maxStackSize];
    int higherStack[maxStackSize];

    int topStack = -1;                 // Top of the stack

    lowerStack[++topStack] = 0;             // Initial low index
    higherStack[++topStack] = size - 1;      // Initial high index

    while (topStack >= 0) {
        int high = higherStack[topStack--];
        int low = lowerStack[topStack--];


        int pivot1 = list[low];
        int pivot2 = list[(low + high) / 2];  // Pivot point between low and high
        int pivot3 = list[high];

        if (pivot1 > pivot2) swap(pivot1, pivot2);
        if (pivot1 > pivot3) swap(pivot1, pivot3);
        if (pivot2 > pivot3) swap(pivot2, pivot3);

        int firstP = low + 1, secondP = low + 1, thirdP = high - 1;

        // Similar to original code, partitioning the code
        while (secondP <= thirdP) {
            if (list[secondP] < pivot2) {
                std::swap(list[firstP], list[secondP]);
                ++firstP;
                ++secondP;
            } else if (list[secondP] >= pivot2 && list[secondP] <= pivot3) {
                ++secondP;
            } else {
                std::swap(list[secondP], list[thirdP]);
                --thirdP;
            }
        }

        swap(list[low], list[firstP - 1]);
        swap(list[low + 1], list[secondP - 1]);
        swap(list[high], list[thirdP + 1]);

        // Determining the partitioned segments and pushed onto stacks
        if (firstP - 2 > low) {
            lowerStack[++topStack] = low;
            higherStack[topStack] = firstP - 2;
        }

        if (firstP < secondP - 2) {
            lowerStack[++topStack] = firstP;
            higherStack[topStack] = secondP - 2;
        }

        if (secondP < thirdP) {
            lowerStack[++topStack] = secondP;
            higherStack[topStack] = thirdP;
        }

        if (thirdP + 2 < high) {
            lowerStack[++topStack] = thirdP + 2;
            higherStack[topStack] = high;
        }
    }
}

int main(){
    const int listSize = 1000;
    int list[listSize];

    // Problem 4 code
    unsigned long long int foo1[listSize];
    unsigned long long int foo2[listSize];

    // Generate random values for the arrays
    for (int i = 0; i < listSize; ++i) {
        foo1[i] = rand() % 101; // Generate random values between 0 and 100
        foo2[i] = foo1[i];}

    // Check if arrays are identical
//    bool check = equal(begin(foo1), end(foo1), begin(foo2));
//    if(check){cout << "Equal \n";}
//    else{cout << "Bug \n";}

    // Sort one of the foos
    //sort(begin(foo2), end(foo2));
    // Check if foo2 is sorted
//    for (int i = 0; i < listSize; ++i) {
//        cout << foo2[i] << " ";} cout << "\n\n";


    for(int i=0;i<listSize;i++) list[i]=rand()%100;
    for(int i=0;i<listSize;i++) cout << list[i] << " ";
    cout << endl;
    cout << "\n\n";

    // Measure time using chrono before running it
    auto start_time = chrono::high_resolution_clock::now();    //nonRecursiveQuickSort(reinterpret_cast<int *>(foo1), listSize); //Alt 1

    //mergeSort(list, 0, listSize-1); // Runs mergeSort (works)

    nonRecursiveQuickSort(list,listSize); // Runs rewritten, alternative 1, and alternative 3 (works)

    // I am aware there is a bug in this code but did not have enough time to find it and fix it
    //triPivNonRecursQuickSort(list,listSize); // Runs alternative 2

    // Continuation of problem 4

    //nonRecursiveQuickSort(reinterpret_cast<int *>(foo1), listSize);
    //triPivNonRecursQuickSort(reinterpret_cast<int *>(foo1), listSize);
    //mergeSort(reinterpret_cast<int *>(foo1), 0, listSize - 1);

    //nonRecursiveQuickSort(reinterpret_cast<int *>(foo2), listSize);
    //triPivNonRecursQuickSort(reinterpret_cast<int *>(foo2), listSize);
    //mergeSort(reinterpret_cast<int *>(foo2), 0, listSize - 1);

    // Measure time using chrono after sorting
    auto end_time = chrono::high_resolution_clock::now();
    // Calculate and display the time taken in milliseconds
    chrono::duration<double, milli> elapsed_time = end_time - start_time;
    cout << "Sorting time for foos: " << elapsed_time.count() << " milliseconds\n\n";


    for(int i=0;i<listSize;i++) cout << list[i] << " ";
    cout << endl;
    int x;
    cin >> x;
    cout << "\n\n";

}