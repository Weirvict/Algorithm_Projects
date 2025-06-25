# This class is to deal with nodes for the array queue as well as making my project more organized. I also thought it
# would be smart since it'll be easier to see and find. This class is also based off of the slides in class,
# title: "Using and Array to Implement a Priority Queue(PQ)" of an unsorted array.
import sys

"""
Victoria Weir
CS4412
Project 3 - Network Routing 

"""

'''
These functions provide utility operations for navigating an array-based priority queue.

Overall time complexity: 
- Initializing the array with nodes and maximum distances: O(|V|), where |V| is the number of nodes in the network.
- Setting the size of the array: O(1)
- Initializing the position array: O(|V|)
- Assigning positions to nodes: O(|V|)
'''


# Define a class for the Array Queue
class ArrayQueue:
    # Constructor to initialize the ArrayQueue with nodes
    def __init__(self, nodes):
        # Initialize the array with nodes and maximum distances
        # Time complexity: O(|V|), where |V| is the number of nodes in the network.
        self.array = [[node, sys.maxsize] for node in nodes]
        # Set the size of the array
        # Time complexity: O(1)
        self.size = len(nodes)
        # Initialize the position array to keep track of the position of each node
        # Time complexity: O(|V|)
        self.position = [None] * self.size
        # Assign positions to nodes
        # Time complexity: O(|V|)
        for i, node in enumerate(nodes):
            self.position[node.node_id] = i

    # Get the distance of a node at index i
    # Overall space and time complexity: O(1)
    def weight(self, i):
        return self.array[i][1]

    # Swap two nodes of the array
    # Overall time complexity: O(1)
    # Overall space complexity: O(1)
    def swap(self, a, b):
        self.array[a], self.array[b] = self.array[b], self.array[a]
        c = self.array[a][0].node_id
        d = self.array[b][0].node_id
        self.position[c], self.position[d] = self.position[d], self.position[c]

    '''
    This method decreases the key (distance) of a node in the array.

    @param v: The node whose key is to be decreased.
    @param dist: The new distance value for the node.
    Time complexity: O(1)
    Space complexity: O(1)
    '''

    def decreaseKey(self, v, dist):
        i = self.position[v]
        if i >= self.size:
            return
        self.array[i][1] = dist

    '''
    This method checks if the array is empty.

    @return: True if the array is empty, False otherwise.
    Time complexity: O(1)
    Space complexity: O(1)
    '''

    def isEmpty(self):
        # Time complexity: O(1)
        return self.size == 0

    '''
    This method gets the minimum node from the array.

    @return: The minimum node extracted from the array.
    Time complexity:
    - Checking if the array is empty: O(1)
    - Finding the index of the minimum node: O(N), where N is the number of nodes in the array.
    - Swapping the minimum node with the last node: O(1)
    - Removing the last node from the array: O(1)
    - Decrementing the size of the array: O(1)

    Space complexity: O(1)
    '''

    def get_min(self):
        if self.size == 0:
            return
        # Find the index of the minimum node based on the weight function
        i = min(range(self.size), key=self.weight)
        # Swap the minimum node with the last node
        self.swap(i, self.size - 1)
        # Remove the last node from the array and get its node object
        node, _ = self.array.pop()
        # Decrement the size of the array
        self.size -= 1
        # Return the minimum node
        return node
