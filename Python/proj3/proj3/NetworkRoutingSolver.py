#!/usr/bin/python3

from CS4412Graph import *
from ArrayFunction import *
from BinaryFunctions import *
import time

"""
Victoria Weir
CS4412
Project 3 - Network Routing 

"""


class NetworkRoutingSolver:
    def __init__(self):
        pass

    def initializeNetwork(self, network):
        assert (type(network) == CS4412Graph)
        self.network = network

    '''
    This method returns the shortest path from the source node to the given destination node index.
    It uses the prev, dist, and source attributes of the class to backtrack from the destination node to the source node.
    
    @param destIndex: The index of the destination node.
    @return: A dictionary containing the total length and the list of edges representing the shortest path.
    
    Time complexity: O(E), where E is the number of edges in the shortest path.
    Space complexity: O(E), where E is the number of edges in the shortest path.
    '''

    def getShortestPath(self, dest_index):
        # Set the destination index
        # Time complexity: O(1)
        self.dest = dest_index

        # Initialize an empty list to store the edges of the shortest path
        # Time complexity: O(1)
        path_edges = []

        # Initialize the total length of the shortest path
        # Time complexity: O(1)
        total_length = 0

        # Start from the destination node and backtrack to the source node using the prev array
        # Time complexity: O(E), where E is the number of edges in the shortest path.
        node = self.network.nodes[self.source]
        while dest_index != node.node_id:
            # Append the edge to the path_edges list
            # Time complexity: O(1)
            path_edges.append((self.network.nodes[self.prev[dest_index]].loc, self.network.nodes[dest_index].loc,
                               '{:.0f}'.format(self.dist[dest_index])))
            # Update the total length of the shortest path
            # Time complexity: O(1)
            total_length += self.dist[dest_index]
            # Move to the previous node
            # Time complexity: O(1)
            dest_index = self.prev[dest_index]

        # Return the total length and the list of edges representing the shortest path
        # Time complexity: O(1)
        return {'cost': total_length, 'path': path_edges}

    '''
    This method computes the shortest paths from the given source node using Dijkstra's algorithm.

    @param srcIndex: The index of the source node.
    @param use_heap: A boolean flag indicating whether to use BinaryHeap (True) or ArrayQueue (False) for priority queue implementation.
    @return: The time taken for computation in seconds.

    With BinaryHeap:
    Time complexity: O((|V| + |E|) log |V|), where |V| is the number of nodes in the network and |E| is the number of edges.
    With ArrayQueue:
    Time complexity: O(|V|^2), where |V| is the number of nodes in the network.
    
    '''

    def computeShortestPaths(self, srcIndex, use_heap=False):
        # Set the source index
        # Time complexity: O(1)
        self.source = srcIndex

        # Initialize the start time
        # Time complexity: O(1)
        t1 = time.time()

        # Initialize data structures
        # Time complexity: O(|V|), where |V| is the number of nodes in the network.
        vector = self.network.nodes
        self.prev = {}
        self.dist = {}
        self.position = {}

        # Choose the priority queue implementation based on the value of use_heap
        # Time complexity: O(1)
        if use_heap:
            # Use BinaryHeap for priority queue
            priority_queue = BinaryHeap(vector)
        else:
            # Use ArrayQueue for priority queue
            priority_queue = ArrayQueue(vector)

        # Initialize distances and previous nodes
        # Time complexity: O(|V|), where |V| is the number of nodes in the network.
        for i in vector:
            self.dist[i.node_id] = sys.maxsize
            self.prev[i.node_id] = None

        # Initialize the source node
        # Time complexity: O(1)
        priority_queue.position[srcIndex] = srcIndex
        self.dist[srcIndex] = 0
        priority_queue.decreaseKey(srcIndex, self.dist[srcIndex])

        # Dijkstra's algorithm to find shortest paths
        while not priority_queue.isEmpty():
            current_node = priority_queue.get_min()
            for edge in current_node.neighbors:
                if self.dist[edge.dest.node_id] > self.dist[current_node.node_id] + edge.length:
                    self.dist[edge.dest.node_id] = self.dist[current_node.node_id] + edge.length
                    self.prev[edge.dest.node_id] = current_node.node_id
                    priority_queue.decreaseKey(edge.dest.node_id, self.dist[edge.dest.node_id])
                    self.position[edge.dest.node_id] = edge.length

        # Compute the end time
        # Time complexity: O(1)
        t2 = time.time()
        return (t2 - t1)

# Full Time Complexity: O(|V| + (|V| + |E|) log |V|)
# Full Space Complexity: O(|V|)
