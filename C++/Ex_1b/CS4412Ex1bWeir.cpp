# include <iostream>
#include <chrono>
#include <vector>


using namespace std;
/**
 *
 * @author Victoria Weir
 * @class CS4412
 * @date 9-2-23
 * @details Homework 2 - Implementing FooSort
 *
 */

void minMax(uint32_t* list, size_t size, int min, int max){
    if(size==0){
        min = 0;
        max = 0; // If array is empty
    }
    // Find the maximum and minimum values in the input array
    min = list[0];
    max = list[0];

    for (size_t  i = 1; i < size; ++i) {
        if (list[i] > max) {
            max = list[i];
        }
        if (list[i] < min) {
            min = list[i];
        }
    }
    //cout << "\n Min: " << min << "\n Max: " << max;
}

void fooSort(uint32_t* list, size_t size){

    // Allocate a freqCount array of length 2^32-1, and initialize the entire array to zero
    const size_t freqCountSize = 1ULL << 32; // 2^32-1,
    uint32_t* freqCount = new uint32_t[freqCountSize]();

    // Traverse the array n and for each element e in a, increment freqCount[e] by one
    for (size_t i = 0; i < size; ++i) {
        uint32_t e = list[i];
        if (e < freqCountSize) {
            freqCount[e]++;
        }
    }

    // Starting an a position 0,  traverse the freqCount array and for each nonzero freqCount[i]=n, add freqCount[i] values to a
    int position = 0;
    uint32_t* a = new uint32_t[size]; // sorted array
    for(size_t i =0; i < freqCountSize; i++){
        while(freqCount[i] > 0){
            a[position] = i;
            position++;
            freqCount[i]--;
        }
    }
    // Print Sorted
    cout << "Sorted: "<< endl;
    for(int p = 0; p<size; ++p){
        cout << a[p] << " ";
    }
    cout <<"\n\n";

    // Deallocate memory
    delete[] freqCount;
    delete[] a;
}

void fooSortAlt(uint32_t* list, size_t size){
    // Find min and max
    int min, max;
    minMax(list, size, min, max);

    // Range
    int range = max - min + 1;

    // Allocate memory for freqCount array
    uint32_t* freqCount = new uint32_t[range]();

    // Traversing
    for (size_t i = 0; i < size; ++i) {
        uint32_t e = list[i] - min;
        if (e < range) {
            freqCount[e]++;
        }
    }

    // Reconstruct the sorted array
    int position = 0;
    uint32_t* a = new uint32_t[range]; // sorted array
    for(size_t i =0; i < range; i++){
        while(freqCount[i] > 0){
            a[position] = i;
            position++;
            freqCount[i]--;
        }
    }
    // Print Sorted
    cout << "Sorted: "<< endl;
    for(int p = 0; p<size; ++p){
        cout << a[p] << " ";
    }
    cout <<"\n\n";

    // Deallocate memory
    delete[] freqCount;
    delete[] a;
}

void fooSortVectors(vector<uint32_t> list){
    // Get max and min
    uint32_t max, min;
    for(uint32_t values : list){
        if(values>max){max = values;}
        if(values<min){min = values;}
    }

    // Allocate and initialize the freqCount vector
    vector<uint32_t> freqCount(max + 1, min);

    // Traverse the input array and increment freqCount for each element
    for (uint32_t position : list) {
        freqCount[position]++;
    }

    // Copied from original fooSort
    vector<uint32_t> sortedArray(list.size());
    int position = 0;
    for (int i = 0; i <= max; ++i) {
        while (freqCount[i] > 0) {
            sortedArray[position] = i;
            position++;
            freqCount[i]--;
        }
    }

    // Copy the sorted array back to the input array
    list = sortedArray;

    //Print
    cout << "Sorted Vector: \n";
    for (uint32_t place : list) {
        cout << place << " ";
    }
    cout << "\n\n";
}




int main(){
    const size_t listSize = 10000;
    uint32_t list[listSize];
    vector<uint32_t> vector1 = {listSize};

    cout << "Unsorted List: \n";
    for(int i=0;i<listSize;i++) list[i]=rand()%100;
    for(int i=0;i<listSize;i++) cout << list[i] << " ";
    cout << endl;
    cout << "\n\n";

//    cout << "Unsorted Vector List: \n";
//    for(int i=0;i<listSize;i++) vector1[i]=rand()%100;
//    for(int i=0;i<listSize;i++) cout << vector1[i] << " ";
//    cout << endl;
//    cout << "\n\n";

    // Measure time using chrono before running it
    auto start_time = chrono::high_resolution_clock::now();

    fooSort(list, listSize);
    //fooSortAlt(list, listSize);
    //fooSortVectors(vector1);

   //test minMax
//    int min, max;
//    minMax(list,listSize,min,max);


    // Measure time using chrono after sorting
    auto end_time = chrono::high_resolution_clock::now();
    // Calculate and display the time taken in milliseconds
    chrono::duration<double, milli> elapsed_time = end_time - start_time;
    cout << "Sorting time for a: " << elapsed_time.count() << " milliseconds\n\n";


//    for(int i=0;i<listSize;i++) cout << list[i] << " ";
//    cout << endl;
//    int x;
//    cin >> x;
//    cout << "\n\n";
}