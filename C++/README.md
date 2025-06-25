# Advanced Algorithms Projects (C++)

***

This repository contains a collection of projects developed for an advanced algorithms course(CS4412), implemented in C++. Each project explores a different algorithmic concept or data structure, with an emphasis on performance, efficiency, and practical application.

***

## Exercise 1a: Sorting, Induction, and Big O Analysis

This exercise focused on analyzing algorithmic complexity and implementing multiple variations of sorting algorithms in C++. It began with a mathematical induction proof to demonstrate that a fully filled binary tree of height `n` contains `2^n - 1` nodes. The Big O complexities of bubble sort, merge sort, and quick sort were explored through theoretical justification, with merge sort confirmed to run in `O(n log n)` and worst-case quick sort in `O(n^2)`. The merge sort algorithm was modified to replace increment operations with explicit index updates for clarity, and a non-recursive quick sort was adapted from the C language and extended into three variations: pivot at the first element, pivot as the median of three, and a randomly selected pivot. The performance of each variant was measured against both randomly filled and pre-sorted arrays, with results showing that merge sort generally performed fastest, especially on sorted inputs. Timing comparisons and conclusions about practical testing and optimization were discussed, reinforcing the impact of pivot strategy and input data on algorithm performance.

***

## Exercise 1b: FooSort (Counting Sort)

This exercise focused on implementing a simplified version of Counting Sort—referred to as “FooSort”—in C++. The algorithm used a frequency array (`freqCount`) sized for all possible 32-bit unsigned integers (2³²), incrementing frequencies during the first pass and reconstructing the sorted array during the second. Testing revealed that C++11 does not default-initialize local arrays to zero unless explicitly initialized (e.g., `int arr[10] = {}`), making initialization a critical step. Performance comparisons with a non-recursive quick sort showed FooSort to be significantly slower, especially with larger arrays due to its massive memory footprint. Attempts to scale to arrays of size 10⁸ failed due to memory constraints. Optimization using a `minMax` function significantly reduced runtime by limiting the frequency array's size to only the range of values present. The time complexity was analyzed as O(n²) in worst-case due to nested operations, though optimizations could reduce this. Extension to `unsigned long long` would require changing the data types and could lead to memory overflow. FooSort is only practical when the range is known and narrow, making it "better" than merge or quick sort in such cases. A vector-based version was also attempted, but failed due to logic errors and resource constraints, highlighting trade-offs in dynamic versus static storage.


***

## Project 2: Hashing and Dictionary Implementation

This project involved designing and implementing a custom hash-based dictionary system in C++ using object-oriented principles. A PhoneEntry class was created to encapsulate user contact information, including nested classes for name, address, and phone number components—resulting in a well-structured hierarchy of ten interconnected classes. Each name is standardized using a canonicalize function, and display methods were implemented for formatted output. A custom Dictionary class was developed using an array of eight buckets, each handling collisions via a singly linked list. The hash function was designed to convert concatenated names into consistent and well-distributed integer indices using ASCII values mod 8. Core functionality includes insertion, deletion, and fetching based on various name formats, with proper handling of duplicate entries. A reverse phone lookup version of the dictionary was also implemented using phone numbers as hash keys. Performance analysis confirmed that worst-case operations such as insert, delete, and fetch operate in linear time relative to bucket size, while the hash function itself runs in O(k), where k is key length. The quality of distribution and overall performance is shown to be dependent on the balance between the number of entries and the number of available buckets.

***

## Project 3: Non-Recursive Priority Queue

This project involved designing a non-recursive priority queue implemented using a binary heap structure built on top of an array. The implementation strictly followed the module’s specifications, avoiding external algorithms and maintaining adherence to proper heap-based priority queue behavior. Key functions include `insert`, `extractMin`, `bubbleUp`, and `bubbleDown`, each maintaining the min-heap property. Multiple tests were conducted to validate the functionality, including inserting random values, verifying empty states, visualizing the binary tree structure, and performance benchmarking with input sizes ranging from 10 to 10,000. Performance tests showed logarithmic scaling of insertions and extractions, confirming that both average and worst-case time complexity for core operations are O(log n). Additional utility functions like printing the queue and tree structure were implemented for better visualization and debugging. Overall, the project demonstrates foundational understanding of data structures and algorithmic efficiency using C++.

***

## Project 4: Graph Search and Shortest Path

This project focused on implementing graph traversal algorithms and shortest path finding in both unweighted and weighted directed graphs. The first part involved creating a randomly filled 9x9 adjacency matrix representing directed edges with 30% density, visualizing the graph, and performing depth-first and breadth-first search from vertex 0 to vertex 8. Each traversal was analyzed in terms of path length and Big-O complexity, confirming DFS has a worst-case of O(V), and BFS performs at O(V + E) on average. The second part introduced a 10x10 weighted adjacency matrix with positive edge weights and required implementing a shortest path algorithm based on Dijkstra’s algorithm using a priority queue, building upon the non-recursive priority queue from the previous project. The algorithm displayed the final path, edge weights, and total distance, with complexity evaluated as O(E + V log V) due to the priority queue’s heap behavior. Performance and correctness were verified through randomized graph data, graphical output (or debug-friendly cout representations), and step-by-step tracing.

***

## Project 5: Mazes and Graphs

This project focused on designing a maze generation and traversal system using custom data structures and algorithms. The maze is represented as a grid where each cell has four walls, and a randomized algorithm selectively removes walls to ensure a path from the start (top-left) to the end (bottom-right) exists. The system was implemented in C++ using character graphics to visually display the maze. The algorithm ensures full connectivity by continuing to knock down walls until all cells are reachable from one another. Dijkstra’s algorithm was used to determine the shortest path, with key components such as priority queues, cell boundary validation, and movement logic analyzed for time complexity. The overall complexity is O(R×C log N), where R and C are the maze dimensions and N is the number of cells. Although a bug in path reconstruction prevented full visualization of the shortest path, the program demonstrates advanced control over maze generation, traversal, and complexity management.


***

## Project 6: B+ Tree

This project involved implementing a B+ Tree data structure in C# to manage and query instances of a `Person` class, with each node keyed by a unique ID. The B+ Tree maintained references in leaf nodes and supported operations such as insertion, deletion, key-based searching, and range queries. An interface was developed to test and visualize the structure and operations. A large-scale performance test was attempted using 10 million unsigned integers to assess tree balance over repeated insertions and deletions. Despite completing the insert functionality, the implementation encountered significant issues with node splitting logic, which prevented full tree balancing and traversal consistency. While the graphical interface and some search functionality were partially functional, the project became a valuable learning experience in pointer management, class design, and debugging complex data structures under real constraints.

***

## Project 7: Traveling Salesperson Problem (Branch and Bound)

This project uses a branch and bound algorithm to explore all permutations of the TSP problem efficiently. Cost matrices, lower bounds, and pruning are used to improve performance and reduce the number of states. The program demonstrates how combinatorial explosion is controlled by smart bounding, with results printed step-by-step to show the algorithm’s progression and effectiveness.

***

## Notes

- Language: **C++**
- Style: Command-line based output and logic visualization (no GUI).
- Purpose: Demonstrate knowledge in algorithmic design, time/space complexity, and data structure implementation.
- This portfolio reflects independent design, debugging, and testing across all projects.