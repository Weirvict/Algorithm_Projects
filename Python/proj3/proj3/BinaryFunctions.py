import sys

"""
Victoria Weir
CS4412
Project 3 - Network Routing 

"""

'''
These functions provide operations for a binary heap.

Overall time complexity: O(1) for each function.
'''


# Function to get the index of the parent node
def parent(i):
    # Time complexity: O(1)
    return (i - 1) // 2


# Function to get the index of the child node
def child(i, n):
    # Time complexity: O(1)
    return 2 * i + n


# Function to check if a node is the root
def root(i):
    # Time complexity: O(1)
    return i == 0


'''
This class represents a binary heap of nodes and distances.

Overall time complexity:
- Initializing the array with nodes and maximum distances: O(|V|), where |V| is the number of nodes in the network.
- Setting the size of the heap: O(1)
- Initializing the position array: O(|V|)
- Assigning positions to nodes: O(|V|)
- Heapifying the array to satisfy the heap property: O(|V| log |V|)
'''


class BinaryHeap:
    def __init__(self, nodes):
        # Initialize the array with nodes and maximum distances
        # Time complexity: O(|V|), where |V| is the number of nodes in the network.
        self.array = [[node, sys.maxsize] for node in nodes]
        # Set the size of the heap
        # Time complexity: O(1)
        self.size = len(nodes)
        # Initialize the position array to keep track of the position of each node
        # Time complexity: O(|V|)
        self.position = [None] * self.size
        # Assign positions to nodes
        # Time complexity: O(|V|)
        for i, node in enumerate(nodes):
            self.position[node.node_id] = i
        # Heapify the array to satisfy the heap property
        # Time complexity: O(|V| log |V|)
        self.heap()

    # Get the distance of a node at index i
    # Overall space and time complexity: O(1)
    def weight(self, i):
        # Time complexity: O(1)
        return self.array[i][1]

    # Heapify the binary heap
    # Overall time complexity: O(|V|), where |V| is the number of nodes in the network.
    # Overall space complexity: O(1)
    def heap(self):
        # Time complexity: O(|V|)
        for i in reversed(range(self.size)):
            if self.weight(i) < self.weight(parent(i)):
                self.swap(self, i, parent(i))

    '''
    This method swaps two nodes of the heap.

    @param a: The index of the first node.
    @param b: The index of the second node.
    @return: None

    Time complexity: O(1)
    Space complexity: O(1)
    '''

    def swap(self, a, b):
        # Time complexity: O(1)
        self.array[a], self.array[b] = self.array[b], self.array[a]
        c = self.array[a][0].node_id
        d = self.array[b][0].node_id
        self.position[c], self.position[d] = self.position[d], self.position[c]

    '''
    This method checks if a node at index i is a child.

    @param i: The index of the node to check.
    @return: True if the node is a child, False otherwise.

    Time complexity: O(1)
    Space complexity: O(1)
    '''

    def child(self, i):
        # Time complexity: O(1)
        return 0 <= i < self.size

    '''
    This method checks if a node at index i is a leaf.

    @param i: The index of the node to check.
    @return: True if the node is a leaf, False otherwise.

    Time complexity: O(1)
    Space complexity: O(1)
    '''

    def leaf(self, i):
        # Time complexity: O(1)
        return self.child(i) and not self.child(child(i, 1))

    '''
    This method bubbles down a node to satisfy the heap property.

    @param i: The index of the node to start bubbling down.
    @return: None

    Time complexity: O(log N), where N is the number of nodes in the heap.
    Space complexity: O(1)
    '''

    def down(self, i):
        while self.child(i) and not self.leaf(i):
            lowest = i
            if self.weight(child(i, 1)) < self.weight(lowest):
                lowest = child(i, 1)
            if self.child(child(i, 2)) and self.weight(child(i, 2)) < self.weight(lowest):
                lowest = child(i, 2)
            if lowest == i:
                return
            self.swap(i, lowest)
            i = lowest

    '''
    This method bubbles up a node to satisfy the heap property.

    @param i: The index of the node to start bubbling up.
    @return: None

    Time complexity: O(log N), where N is the number of nodes in the heap.
    Space complexity: O(1)
    '''

    def up(self, i):
        while not root(i):
            if self.weight(i) >= self.weight(parent(i)):
                break
            self.swap(i, parent(i))
            i = parent(i)

    '''
    This method gets the minimum node from the heap.

    @return: The minimum node extracted from the heap.
    Time complexity:
    - Checking if the heap is empty: O(1)
    - Swapping the root with the last node: O(1)
    - Extracting the root from the array: O(1)
    - Decreasing the size of the heap: O(1)
    - Bubbling down the root node: O(log N), where N is the number of nodes in the heap.

    Space complexity: O(1)
    '''

    def get_min(self):
        if self.isEmpty():
            return
        self.swap(0, self.size - 1)
        root, _ = self.array.pop()

        self.size -= 1
        self.down(0)

        return root

    '''
    This method checks if the heap is empty.

    @return: True if the heap is empty, False otherwise.
    Time complexity: O(1)
    Space complexity: O(1)
    '''

    def isEmpty(self):
        # Time complexity: O(1)
        return self.size == 0

    '''
    This method decreases the key (distance) of a node in the heap.

    @param v: The node whose key is to be decreased.
    @param dist: The new distance value for the node.
    Time complexity: O(log |V|) with BinaryHeap, O(1) with ArrayQueue, where |V| is the number of nodes in the network.
    Space complexity: O(1)
    '''

    def decreaseKey(self, v, dist):
        i = self.position[v]
        if i >= self.size:
            return
        self.array[i][1] = dist
        self.up(i)
