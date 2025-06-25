import unittest

from BinaryFunctions import BinaryHeap as BinaryFunctions, BinaryHeap


# Define a test class for the BinaryHeap
class TestBinaryHeap(unittest.TestCase):
    # Define a setup method to initialize common test data
    def setUp(self):
        # Create sample nodes for testing
        self.nodes = [Node(0), Node(1), Node(2), Node(3), Node(4)]
        # Initialize a binary heap with the sample nodes
        self.binary_heap = BinaryHeap(self.nodes)

    # Test the initialization of the BinaryHeap
    def test_initialization(self):
        # Check if the binary heap is not empty
        self.assertFalse(self.binary_heap.isEmpty())
        # Check if the size of the binary heap matches the number of nodes
        self.assertEqual(len(self.nodes), self.binary_heap.size)

    # Test the extraction of the minimum node from the BinaryHeap
    def test_extractMin(self):
        # Extract the minimum node from the binary heap
        min_node = self.binary_heap.get_min()
        # Check if the extracted node is not None
        self.assertIsNotNone(min_node)
        # Check if the size of the binary heap is reduced by one after extraction
        self.assertEqual(len(self.nodes) - 1, self.binary_heap.size)

    # Test the decreaseKey operation in the BinaryHeap
    def test_decreaseKey(self):
        # Update the distance of a node in the binary heap
        updated_distance = 10
        self.binary_heap.decreaseKey(0, updated_distance)
        # Check if the distance of the updated node is modified
        self.assertEqual(updated_distance, self.binary_heap.array[self.binary_heap.position[0]][1])

    # Add more test cases as needed

# Define a sample Node class for testing purposes
class Node:
    def __init__(self, node_id):
        self.node_id = node_id

# Run the tests if this file is executed directly
if __name__ == "__main__":
    unittest.main()