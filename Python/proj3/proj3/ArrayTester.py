import unittest

from proj3.proj3.ArrayFunction import ArrayQueue


# Define a test class for the ArrayQueue
class TestArrayQueue(unittest.TestCase):
    # Define a setup method to initialize common test data
    def setUp(self):
        # Create sample nodes for testing
        self.nodes = [Node(0), Node(1), Node(2), Node(3), Node(4)]
        # Initialize an array queue with the sample nodes
        self.array_queue = ArrayQueue(self.nodes)

    # Test the initialization of the ArrayQueue
    def test_initialization(self):
        # Check if the array queue is not empty
        self.assertFalse(self.array_queue.isEmpty())
        # Check if the size of the array queue matches the number of nodes
        self.assertEqual(len(self.nodes), self.array_queue.size)

    # Test the extraction of the minimum node from the ArrayQueue
    def test_extractMin(self):
        # Extract the minimum node from the array queue
        min_node = self.array_queue.get_min()
        # Check if the extracted node is not None
        self.assertIsNotNone(min_node)
        # Check if the size of the array queue is reduced by one after extraction
        self.assertEqual(len(self.nodes) - 1, self.array_queue.size)

    # Test the decreaseKey operation in the ArrayQueue
    def test_decreaseKey(self):
        # Update the distance of a node in the array queue
        updated_distance = 10
        self.array_queue.decreaseKey(0, updated_distance)
        # Check if the distance of the updated node is modified
        self.assertEqual(updated_distance, self.array_queue.array[self.array_queue.position[0]][1])

    # Add more test cases as needed


# Define a sample Node class for testing purposes
class Node:
    def __init__(self, node_id):
        self.node_id = node_id


# Run the tests if this file is executed directly
if __name__ == "__main__":
    unittest.main()
