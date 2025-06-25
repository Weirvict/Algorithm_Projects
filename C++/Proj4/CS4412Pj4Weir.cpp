# include <iostream>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <array>
#include <fstream>

using namespace std;

/**
 *
 * @author Victoria Weir
 * @class CS4412
 * @date 10-19-23
 * @details Project 4 - Graph Search and Shortest Path
 *
 * This is Part 1 of Project 4
 * This project generates a 9x9 integer array that acts as a graph.
 * Depth first search and breath first search are the algorithms coded in this project.
 *
 */

// Vertex class to represent individual vertices in the graph
class Vertex {
public:
    char color;        // Color of the vertex (e.g., 'W' for white, 'G' for gray, 'R' for black)
    int distance;      // Distance from the source vertex (used in BFS)
    int number;        // Vertex number or identifier
    Vertex *parent;    // Parent vertex in the traversal path (used in BFS)

    // Default constructor
    Vertex() : color('W'), distance(-1), number(-1), parent(nullptr) {}
};

const int n = 9; // Size of the array of part 1
const int m = 10; // Size of the array of part 2
const double edgeProbability = 0.3; // Probability of having an edge

int adjacencyMatrix[n][n]; // Adjacency matrix to represent the graph
int adjacencyMatrix2[m][m];
bool visited[n]; // Array to keep track of visited vertices
bool visited2[m];

// Function to initialize the graph by generating random edges
void initializeGraph() {
    // Seed the random number generator
    srand(static_cast<unsigned>(time(0)));

    // Initialize the adjacency matrix with -1, indicating no edges between vertices
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            adjacencyMatrix[i][j] = -1;
        }
    }

    // Randomly generate edges based on the edge probability
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j && (static_cast<double>(rand()) / RAND_MAX) <= edgeProbability) {
                adjacencyMatrix[i][j] = 1;
            }
        }
    }
}

/**
 * Displays the attributes of Vertex objects, including edge weights and total weighted distance, and highlights vertices in the shortest path.
 *
 * @param vertices       An array of Vertex objects representing the vertices in the graph.
 * @param columns        The number of columns in the table (usually the number of vertices).
 * @param shortestPath   A vector of integers representing the sequence of vertices in the shortest path.
 * @param adjacencyMatrix A 2D array representing the edge weights between vertices.
 */
void DisplayVertices2(Vertex vertices[], int columns, const vector<int> &shortestPath, int adjacencyMatrix2[][m]) {
    // Print the table header
    cout << "Color | Distance | Vertex | Parent | Edge Weight\n";
    cout << "--------------------------------------------------\n";

    int totalDistance = 0;

    for (int j = 0; j < columns; j++) {
        // Display the color of the vertex
        cout << vertices[j].color << "     | ";

        // Display the distance of the vertex
        cout << std::setw(8) << vertices[j].distance << " | ";

        // Display the vertex number
        cout << std::setw(6) << vertices[j].number << " | ";

        if (vertices[j].parent) {
            // Display the parent vertex with a 'V' prefix
            cout << "V" << vertices[j].parent->number << " | ";

            // Find the edge weight from parent to current vertex
            int edgeWeight = adjacencyMatrix2[vertices[j].parent->number][vertices[j].number];

            // Add the edge weight to the total distance
            totalDistance += edgeWeight;

            // Display the edge weight
            cout << std::setw(10) << edgeWeight;
        } else {
            // If there's no parent, indicate it with '~'
            cout << "~ | ";
        }

        // Highlight vertices in the shortest path with "(Path)"
        if (find(shortestPath.begin(), shortestPath.end(), vertices[j].number) != shortestPath.end()) {
            cout << " (Path)";
        }
        cout << "\n";
    }

    // Print the table footer
    cout << "--------------------------------------------------\n";

    // Print the total weighted distance
    cout << "Total Weighted Distance: " << totalDistance << endl;
}

// Part 2 for the most part
class CS4412Pj3Weir {
private:
    static const unsigned int MAX_SIZE = m;
    std::array<unsigned int, MAX_SIZE> nodes{};
    int size;

    int getParentIndex(int index);

    int getLeftChildIndex(int index);

    int getRightChildIndex(int index);

    void bubbleDown(int index);

    void bubbleUp(int index);

    void swapNodes(int index1, int index2);

public:
    CS4412Pj3Weir() : size(0) {}

    int extractMin();

    void insert(int value);

    bool checkIfEmpty();

    void toString();

    void viewTree();
};

// Function to get the parent index
int CS4412Pj3Weir::getParentIndex(int index) {
    return (index - 1) / 2;
}

// Function to get the left child index
int CS4412Pj3Weir::getLeftChildIndex(int index) {
    return (2 * index + 1);
}

// Function to get the right child index
int CS4412Pj3Weir::getRightChildIndex(int index) {
    return (2 * index + 2);
}

/**
	 * This function essentially bubbles down to maintain min-heap property
	 * @param index represents the location of the value
	 */
void CS4412Pj3Weir::bubbleDown(int index) {
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
void CS4412Pj3Weir::bubbleUp(int index) {
    int parentIndex = getParentIndex(index);

    while (index > 0 && nodes[index] < nodes[parentIndex]) {
        swapNodes(index, parentIndex);
        index = parentIndex;
        parentIndex = getParentIndex(index);
    }
}

/**
	 * Function to insert an element into the priority queue
	 * @param value represents the value which the node will be
	 */
void CS4412Pj3Weir::insert(int value) {
    if (size >= MAX_SIZE) {
        throw runtime_error("Priority Queue is full");
    }
    nodes[size] = value;
    bubbleUp(size);
    size++;
}

/**
	 *  Function to extract the minimum element (highest priority)
	 * @return Should return the minimum element of the priority
	 */
int CS4412Pj3Weir::extractMin() {
    if (size == 0) {
        throw runtime_error("Priority Queue is empty");
    }

    int min = nodes[0];
    nodes[0] = nodes[size - 1];
    size--;
    bubbleDown(0);
    return min;
}

// Checks if the queue is empty
bool CS4412Pj3Weir::checkIfEmpty() {
    return size == 0;
}

/**
	 * Function to swap two elements in the array
	 * @param index1 The current node we are on
	 * @param index2 The node we want to switch positions with
	 */
void CS4412Pj3Weir::swapNodes(int index1, int index2) {
    int temp = nodes[index1];
    nodes[index1] = nodes[index2];
    nodes[index2] = temp;
}

/**
 * Calculates and displays the shortest path from a starting vertex to a target vertex in a graph
 * and uses an adjacency matrix to represent edge weights.
 *
 * @param start              The vertex number from which the shortest path should start.
 * @param target             The vertex number to which the shortest path should be found.
 * @param vertices           An array of Vertex objects representing the vertices in the graph.
 * @param adjacencyMatrix2   A 2D array representing the adjacency matrix with edge weights.
 *
 */
void shortestPath(int start, int target, Vertex vertices[], int adjacencyMatrix2[][m]) {
    int dist[m]; // An array to store the shortest distance to each vertex
    bool visited2[m]; // An array to keep track of visited vertices

    // Initialize the distance array with "infinite" distances and the visited array to false
    for (int i = 0; i < m; i++) {
        dist[i] = INT_MAX;
        visited2[i] = false;
    }

    // Initialize priority queue
    CS4412Pj3Weir priorityQueue;

    // The distance to the starting vertex is 0
    dist[start] = 0;

    cout << start << vertices->number << "\n";

    // Insert the starting distance into priority queue
    priorityQueue.insert(start);

    while (!priorityQueue.checkIfEmpty()) {
        // Extract the vertex with the smallest distance
        int u = priorityQueue.extractMin();

        // If the extracted vertex is the target, break the loop
        if (u == target) {
            break;
        }

        // Update distance to adjacent vertices
        for (int v = 0; v < m; v++) {
            if (!visited2[v] && adjacencyMatrix2[u][v] > 0 &&
                dist[u] != INT_MAX && dist[u] + adjacencyMatrix2[u][v] < dist[v]) {
                // If a shorter path is found, update the distance and set the parent
                dist[v] = dist[u] + adjacencyMatrix2[u][v];
                vertices[v].parent = &vertices[u];
                // Insert the vertex with updated distance into the priority queue
                priorityQueue.insert(v);
            }
        }

        // Mark the selected vertex as visited
        visited2[u] = true;
    }

    // Construct and display the shortest path
    int currentVertex = target;
    int shortestDistance = dist[target];
    vector<int> path;
    while (currentVertex != -1) {
        path.push_back(currentVertex);
        // Change the color of the vertex to 'R'
        vertices[currentVertex].color = 'R';
        currentVertex = vertices[currentVertex].parent ? vertices[currentVertex].parent->number : -1;
    }

    reverse(path.begin(), path.end());

    // Change the color of visited vertices to 'G'
    for (int i = 0; i < m; i++) {
        if (visited2[i]) {
            vertices[i].color = 'G';
        }
    }

    cout << "Shortest Path from " << start << " to " << target << ":\n";
    for (int vertex: path) {
        cout << vertex << " -> ";
    }
    cout << "Distance: " << shortestDistance << endl;

    // Construct and display the shortest path, passing adjacencyMatrix2
    DisplayVertices2(vertices, m, path, adjacencyMatrix2);
}


// Part 1 for the most part
/**
 *
 * DFS (Depth-First Search) Algorithm
 * @param current represents the current vertex being explored during the DFS.
 * @param target represents the vertex we want to reach or find a path to in the graph
 * @param vertices is an array of Vertex objects to represent the graph vertices
 * @param path  is an array used to store the sequence of vertices visited during the DFS.
 * @param pathIndex keeps track of the current index or position in the path array.
 * @return true if current to target is found, false if no path exits.
 *
 * This algorithm recursively explores a graph, starting from the "current" vertex to the "target" vertex.
 * It keeps track of visited vertices, has a path array to record the order, and returns true when a path
 * is found or false if no path exists.
 *
 */
bool dfs(int current, int target, Vertex vertices[], int path[], int &pathIndex) {
    // If the current vertex is the target, return true and add it to the path
    if (current == target) {
        path[pathIndex++] = current;
        vertices[current].color = 'R'; // Change the color of the target vertex to 'R'
        return true;
    }

    // Mark the vertex as in-progress (gray)
    vertices[current].color = 'G';

    // Iterate through neighbors to explore unvisited ones
    for (int neighbor = 0; neighbor < n; neighbor++) {
        if (adjacencyMatrix[current][neighbor] == 1) {
            if (vertices[neighbor].color == 'W') {
                // Set the parent to the current vertex
                vertices[neighbor].parent = &vertices[current];
                if (dfs(neighbor, target, vertices, path, pathIndex)) {
                    return true;
                }
            }
        }
    }

    // Mark the vertex as visited (Gray) if no path is found
    vertices[current].color = 'G';
    pathIndex--;
    return false;
}

/**
 * BFS (Breadth-First Search) Algorithm
 * @param start represents the starting vertex from which is the shortest path
 * @param target is the destination vertex to which the shortest path from the start vertex.
 * @param vertices is an array of Vertex objects to represent the graph vertices
 * @return  the shortest distance if a path is found, and it returns -1 if there is no path between 'start' to 'target' vertex.
 *
 * This algorithm explores the graph, searching for a path from the start vertex to the target vertex
 * while keeping track of the shortest distance found during the run.
 *
 */
int bfs(int start, int target, Vertex vertices[]) {
    queue<int> queuing;

    // Initialize the distance of the start vertex
    vertices[start].distance = 0;

    // Start vertex is added to begin the search
    queuing.push(start);

    // Enters a loop until empty, this does all the work in the algorithm
    while (!queuing.empty()) {
        int current = queuing.front();
        queuing.pop();

        // If the target vertex is reached, return the distance to it
        if (current == target) {
            vertices[current].color = 'R'; // Change the color of the target vertex to 'R'
            return vertices[current].distance;
        }

        // Iterate through neighbors to explore unvisited ones
        for (int neighbor = 0; neighbor < n; neighbor++) {
            if (adjacencyMatrix[current][neighbor] == 1) {
                if (vertices[neighbor].color == 'W') {
                    // Set the parent to the current vertex
                    vertices[neighbor].parent = &vertices[current];
                    vertices[neighbor].color = 'G'; // Mark as in-progress
                    vertices[neighbor].distance = vertices[current].distance + 1;
                    queuing.push(neighbor);
                }
            }
        }
        //Mark as visited
        vertices[current].color = 'G';
    }
    // If the target is not reachable, return -1
    return -1;
}

/**
 * Function to display the color, distance, vertex number, and parent vertex in a table.
 *
 * @param vertices An array of Vertex objects representing the vertices in the graph.
 * @param columns The number of columns in the table (usually the number of vertices).
 * @param path An array representing the sequence of vertices visited during traversal (e.g., DFS or BFS).
 * @param pathLength The length of the path array.
 *
 * This function displays various attributes of the Vertex objects in a table format, to visualize
 * the state of the vertices during the run. As well as also displaying the path that was taken.
 *
 */
void DisplayVertices(Vertex vertices[], int columns, const int path[], int pathLength) {
    // Print the table header
    cout << "Color | Distance | Vertex | Parent\n";
    cout << "----------------------------------\n";

    // Loop through the vertices to display their information
    for (int j = 0; j < columns; j++) {
        // Display the color of the vertex
        cout << vertices[j].color << "     | ";

        // Display the distance if it's not equal to 'm' (to handle the case where distance is not available)
        if (vertices[j].distance < m) {
            cout << std::setw(8) << vertices[j].distance << " | ";
        } else {
            // If distance is 'm', indicate it's not available
            cout << "        - | ";
        }

        // Display the vertex number
        cout << std::setw(6) << vertices[j].number << " | ";

        // Display the parent of the vertex
        if (vertices[j].parent) {
            cout << "V" << vertices[j].parent->number;
        } else {
            // If there's no parent, indicate it with '~'
            cout << "~";
        }
        cout << "\n";
    }

    // Print the table footer
    cout << "----------------------------------\n";

    // Display the path taken during the traversal
    cout << "Path: ";
    for (int i = 0; i < pathLength; i++) {
        cout << path[i] << " ";
    }
    cout << "\n";
}

int main() {
    initializeGraph(); // Generate a random graph and initialize the adjacency matrix

    // Initialize the visited array to mark vertices as unvisited
    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }

    // Initialize an array of Vertex objects
    Vertex vertices[n];

    // Explicitly create Vertex objects for each vertex
    for (int i = 0; i < n; i++) {
        vertices[i] = Vertex(); // Create a Vertex object
        vertices[i].number = i; // Set the number attribute
    }

    int dfsPath[n];
    int dfsPathIndex = 0;

    if (dfs(0, 8, vertices, dfsPath, dfsPathIndex)) {
        cout << "DFS Path from vertex 0 to vertex 8: ";
        for (int i = 0; i < dfsPathIndex; i++) {
            cout << dfsPath[i] << " ";
        }
        cout << endl;

        // Call DisplayVertices to display the table along with the path
        DisplayVertices(vertices, n, dfsPath, dfsPathIndex);
    } else {
        cout << "Vertex 8 is not reachable from vertex 0 using DFS." << endl;
    }

    // Reset visited array for BFS
    for (int i = 0; i < n; i++) {
        visited[i] = false;
        vertices[i].color = 'W';
        vertices[i].distance = -1;
        vertices[i].parent = nullptr;
    }

    int bfsPath[n];
    int bfsPathIndex = 0;

    int shortestDistance = bfs(0, 8, vertices);

    // Store the BFS path and its length
    if (shortestDistance != -1) {
        cout << "\n BFS Path distance from vertex 0 to vertex 8 using BFS: " << shortestDistance << endl;
        bfsPathIndex = 0;
        int currentVertex = 8; // Start from the target vertex
        while (currentVertex != -1) {
            bfsPath[bfsPathIndex++] = currentVertex;
            currentVertex = vertices[currentVertex].parent ? vertices[currentVertex].parent->number : -1;
        }
    } else {
        cout << "Vertex 8 is not reachable from vertex 0 using BFS." << endl;
    }

    // Call DisplayVertices to display the table along with the BFS result
    DisplayVertices(vertices, n, bfsPath, bfsPathIndex);

    // Part 2:
    cout << "\n Start of Part 2: \n";

    // Initialize a 10x10 integer array with -1 (absence of an edge)
    int adjacencyMatrix2[m][m];
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            adjacencyMatrix2[i][j] = -1;
        }
    }
    // Read data from the file
    ifstream inputFile(R"(C:\Users\torib\CS\Classes\CS4412\HW\CS4412_HWs\Proj4\project4DataFile.txt)");
    if (!inputFile) {
        cerr << "Failed to open the data file." << endl;
        return 1;
    } else {
        cout << " File Read: \n";
    }

    int value;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            inputFile >> value;
            adjacencyMatrix2[i][j] = value;
        }
    }

    // Close the file
    inputFile.close();

    // Print the generated adjacency matrix
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            cout << adjacencyMatrix2[i][j] << "\t";
        }
        cout << endl;
    }
    cout << "\n";

    int start = 0;
    int target = 8;
    Vertex vertices2[m];

    // Process the array, since I kept getting memory failure
//    int i = 5;
//    int j = 7;
//    if (i >= 0 && i < m && j >= 0 && j < n) {
//        int element = adjacencyMatrix2[i][j];
//        // Use 'element' safely
//        cout << "Element at " << i << ", " << j << " is " << element << endl;
//    } else {
//        cout << "Out of bounds: " << i << ", " << j << endl;
//        // Handle the out-of-bounds situation, e.g., log an error or take appropriate action.
//    }


    shortestPath(start, target, vertices2, adjacencyMatrix2);

    vector<int> shortestPath;
    DisplayVertices2(vertices2, m, shortestPath, adjacencyMatrix2);


    cout << "\n Color Legend: \n"
            "W-Not Visited \n"
            "G-Visited \n"
            "R-Visited and part of the path/target \n";


    return 0;
}