#include <iostream>
#include <array>
#include <cmath>
#include <iomanip>
#include <chrono>
#include "CS4412Pj3Weir.h"

using namespace std;
/**
 *
 * @author Victoria Weir
 * @class CS4412
 * @date 10-1-23
 * @details Project 3 - Non-Recursive C++ PriorityQueue
 *  This is based mostly off CS4412/5512 Notes 3.3 Heap or Priority Queue
 *
 */

// Define maximum size, probably should change, so it can be dynamic
const unsigned int MAX_SIZE = 100000;

/**
 * @class nonRecursivePriorityQueue: This class is a Non Recursive Priority Queue that is heavily based off of  Note 3.3
 */
class nonRecursivePriorityQueue{
private:
    // Implement as an array
    std::array<unsigned int, MAX_SIZE> nodes{};
    // Keep track of the current size of the priority queue
    int size;

    // Function to get the parent index
    static int getParentIndex(int index) {
        return (index / 2);
    }

    // Function to get the left child index
    static int getLeftChildIndex(int index) {
        return (2 * index);
    }

    // Function to get the right child index
    static int getRightChildIndex(int index) {
        return (2 * index + 1);
    }

    /**
     * This function essentially bubbles down to maintain min-heap property
     * @param index represents the location of the value
     */
    void bubbleDown(int index) {
        int leftChildIndex = getLeftChildIndex(index);
        int rightChildIndex = getRightChildIndex(index);

        int smallestIndex = index;

        // Compare with left child
        if (leftChildIndex < size && nodes[leftChildIndex] < nodes[smallestIndex]) {
            smallestIndex = leftChildIndex;
        }

        // Compare with right child
        if (rightChildIndex < size && nodes[rightChildIndex] < nodes[smallestIndex]) {
            smallestIndex = rightChildIndex;
        }

        // If the smallest element is not the current element, swap them and continue
        if (smallestIndex != index) {
            swapNodes(index, smallestIndex);
            bubbleDown(smallestIndex);
        }
    }

    /**
     * This function is to bubbles up to maintain min-heap property
     * Also places the node to the right-most leaf
     * @param index represent the location
     */
    void bubbleUp(int index) {
        int placement = nodes[index];
        int parentIndex = getParentIndex(index);

        while (index > 0 && placement < nodes[parentIndex]) {
            swapNodes(index, parentIndex);
            index = parentIndex;
            parentIndex = getParentIndex(index);
        }
    }

    /**
     * Function to swap two elements in the array
     * @param index1 The current node we are on
     * @param index2 The node we want to switch positions with
     */
    void swapNodes(int index1, int index2) {
        swap(nodes[index1], nodes[index2]);
    }

public:
    // Initialize and set size to 0
    nonRecursivePriorityQueue() : size(0){}

    // Checks if the queue is empty
    bool checkIfEmpty() const {
        return size == 0;
    }

    /**
     *  Function to extract the minimum element (highest priority)
     * @return Should return the minimum element of the priority
     */
    int extractMin() {
        if (size == 0) {
            throw runtime_error("Priority Queue is empty");
        }

        int min = nodes[0];
        nodes[0] = nodes[size - 1];
        size--;
        bubbleDown(0);
        return min;
    }

    /**
     * Function to insert an element into the priority queue
     * @param value represents the value which the node will be
     */
    void insert(int value) {
        if (size >= MAX_SIZE) {
            throw runtime_error("Priority Queue is full");
        }
        nodes[size] = value;
        bubbleUp(size);
        size++;
    }

    /**
     * Function to print the priority queue
     */
    void toString() {
        cout << "Priority Queue: ";
        for (int i = 0; i < size; i++) {
            cout << nodes[i] << " ";
        }
        cout << endl;
    }

   /**
    * Function to view the tree structure of the priority queue
    */
    void viewTree() {
        if (checkIfEmpty()) {
            cout << "Priority Queue is empty." << endl;
            return;
        }

        int height = static_cast<int>(log2(size)) + 1;
        int maxWidth = (1 << height) - 1; // Maximum width of the tree

        int index = 0;
        int levelWidth = 1;
        for (int i = 0; i < height; i++) {
            int spaceBetweenNodes = maxWidth / levelWidth;

            for (int j = 0; j < levelWidth; j++) {
                // Print the element
                cout << setw(spaceBetweenNodes / 2) << nodes[index + j];
                // Print spaces between nodes
                if (j < levelWidth - 1) {
                    cout << setw(spaceBetweenNodes) << " * ";
                }
            }
            cout << endl;

            index += levelWidth;
            levelWidth *= 2;
        }
    }
};

int main() {
    nonRecursivePriorityQueue priorityQueue;

    // Test 1: Basic Insertion and Extraction
    priorityQueue.toString();
    int size = 31;
    for (int i = 0; i < size; i++) {
        priorityQueue.insert(rand() % 100);
    }
    priorityQueue.toString();

    // Visualize the priority queue as a binary tree
    priorityQueue.viewTree();

    // Test to see if extract the minimum element (highest priority)
    cout << "Extracted Min: " << priorityQueue.extractMin() << endl;
    cout << "Extracted Min: " << priorityQueue.extractMin() << endl;

    //Depending on how much is extracted, size is updated
    size -=2;

    // Visualize the priority queue as a binary tree
    priorityQueue.viewTree();

    // Clear Queue
    for(int p=0; p < size; p++){
        priorityQueue.extractMin();
    }

    // Test 2: Check if empty
    cout << "Is the priority queue empty? " << (priorityQueue.checkIfEmpty() ? "Yes" : "No") << endl;
    priorityQueue.toString();

    // Test 3: Performance Testing
    const unsigned int numElements = 10000;
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < numElements; i++) {
        priorityQueue.insert(rand() % 100);
    }
    priorityQueue.viewTree();
    for (int i = 0; i < numElements; i++) {
        priorityQueue.extractMin();
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Time taken for " << numElements << " insertions and extractions: " << elapsed.count() << " seconds" << endl;


    return 0;
}
