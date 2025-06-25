import operator

from which_pyqt import PYQT_VER
import random

if PYQT_VER == 'PYQT5':
    from PyQt5.QtCore import QLineF, QPointF, QObject
# elif PYQT_VER == 'PYQT4':
#     from PyQt4.QtCore import QLineF, QPointF, QObject
else:
    raise Exception('Unsupported Version of PyQt: {}'.format(PYQT_VER))

import time

"""
Victoria Weir
CS4412
Project 2 - Convex Hull

"""

# Some global color constants that might be useful
RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)

# Global variable that controls the speed of the recursion automation, in seconds
#
PAUSE = 0.25


#
# This is the class you have to complete.
#
class ConvexHullSolver(QObject):

    # Class constructor
    def __init__(self):
        super().__init__()
        self.pause = False

    # Some helper methods that make calls to the GUI, allowing us to send updates
    # to be displayed.

    def showTangent(self, line, color):
        self.view.addLines(line, color)
        if self.pause:
            time.sleep(PAUSE)

    def eraseTangent(self, line):
        self.view.clearLines(line)

    def blinkTangent(self, line, color):
        self.showTangent(line, color)
        self.eraseTangent(line)

    def showHull(self, polygon, color):
        self.view.addLines(polygon, color)
        if self.pause:
            time.sleep(PAUSE)

    def eraseHull(self, polygon):
        self.view.clearLines(polygon)

    def showText(self, text):
        self.view.displayStatusText(text)

    # ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    """
    Calculates the slope of two points

    :Param point1: The first point
    :Param point2: The second point
    :Return: The slope of the two points

    :Complexity: O(1)
    :Space complexity: O(1) - Simply calculating
    """

    def slope(self, point1, point2):
        slope = (point2.y() - point1.y()) / (point2.x() - point1.x())
        return slope

    """
    Sorts a list of points by their x-coordinates

    :Param points: A list of points
    :Return: A sorted list of points

    :Complexity: O(nlogn), because the Python's sort function is typically O(nlogn)
    :Space Complexity: O(n), where n is the number of points in the list
    """

    def sortPoints(self, points):
        sorted_points = sorted(points, key=lambda point: point.x())
        return sorted_points

    """
    Finds the lower leftmost point in a list of points

    :Param points: A list of points
    :Return: The leftmost point in the list

    :Complexity: O(n) because min function is typically O(n)
    :Space Complexity: O(1) - Only stores few variables
    """

    def get_lower_left(self, left_hull):
        lower_left_index = min(left_hull, key=lambda point: point.x())
        return lower_left_index

    """
    Finds the lower rightmost point in a list of points

    :Param points: A list of points
    :Return: The rightmost point in the list

    :Complexity: O(n) because max function is typically O(n)
    :Space Complexity: O(1) - Only stores few variables
    """

    def get_lower_right(self, right_hull):
        lower_right_index = max(right_hull, key=lambda point: point.x())
        return lower_right_index

    """
    Finds the upper rightmost point in a list of points

    :Param points: A list of points
    :Return: The rightmost point in the list

    :Complexity: O(n) because min function is typically O(n)
    :Space Complexity: O(1) - Only stores few variables
    """

    def get_upper_right(self, right_hull):
        right_upper_index = min(right_hull, key=lambda point: point.x())
        return right_upper_index

    """
    Finds the upper leftmost point in a list of points

    :Param points: A list of points
    :Return: The leftmost point in the list

    :Complexity: O(n) because max function is typically O(n)
    :Space Complexity: O(1) - Only stores few variables
    """

    def get_upper_left(self, left_hull):
        left_upper_index = max(left_hull, key=lambda point: point.x())
        return left_upper_index

    """
    Finds the upper tangent point in a list of points

    :Param left_hull: A list of points
    :Param right_hull: A list of points
    :Return: The upper tangent point in the list

    :Complexity: O(n) because it involves iterating through the points in the hulls, calculating slopes, and updating 
    the indexes until the tangent is found. This is the worst case scenario if it has to iterate through all the 
    points. 
    :Space Complexity: O(1) because they track the indices and slopes but not scale them with input sizes. 
    Therefore, constant space complexity, O(1).
    """

    def get_upper_tangent(self, left_hull, right_hull):
        # Sizes
        l_size = len(left_hull)
        r_size = len(right_hull)

        # Get leftmost and rightmost points from the respective hulls
        left_point = self.get_upper_left(left_hull)
        right_point = self.get_upper_right(right_hull)

        # Get indexes of left and right points
        left_index = left_hull.index(left_point)
        right_index = right_hull.index(right_point)

        # While the edge is not upper tangent to both points
        while True:
            # Slopes for both hulls
            left_slope = self.slope(left_hull[left_index], right_hull[right_index])
            right_slope = self.slope(left_hull[left_index], right_hull[right_index])

            # Get the points in left and right hulls
            next_left_point = (left_index - 1) % l_size
            next_left_slope = self.slope(left_hull[next_left_point], right_hull[right_index])
            next_right_point = (right_index + 1) % r_size
            next_right_slope = self.slope(left_hull[left_index], right_hull[next_right_point])

            # Edge found so we break
            if next_left_slope >= left_slope and next_right_slope <= right_slope:
                break

            # If edges are not found move counter-clockwise
            while next_left_slope < left_slope:  # While the edge is not upper tangent to the left
                left_index = next_left_point
                left_slope = next_left_slope
                next_left_point = (left_index - 1) % l_size
                next_left_slope = self.slope(left_hull[next_left_point], right_hull[right_index])

            # Clockwise move
            while next_right_slope > right_slope:  # While the edge is not upper tangent to the right
                right_index = next_right_point
                right_slope = next_right_slope
                next_right_point = (right_index + 1) % r_size
                next_right_slope = self.slope(left_hull[left_index], right_hull[next_right_point])
        return left_hull[left_index], right_hull[right_index]

    """
    Finds the lower tangent point in a list of points

    :Param left_hull: A list of points
    :Param right_hull: A list of points
        

    :Complexity: O(n) same reason as get_upper_tangent function
    :Space Complexity: O(1) same reason as get_upper_tangent function
    """

    def get_lower_tangent(self, left_hull, right_hull):
        # Sizes
        l_size = len(left_hull)
        r_size = len(right_hull)

        # Leftmost and rightmost points from the respective hulls
        left_point = self.get_lower_left(left_hull)
        right_point = self.get_lower_right(right_hull)

        # Get indexes of left and right points
        left_index = left_hull.index(left_point)
        right_index = right_hull.index(right_point)

        # While the edge is not lower tangent to both points
        while True:
            # Slopes for both hulls
            left_slope = self.slope(left_hull[left_index], right_hull[right_index])
            right_slope = self.slope(left_hull[left_index], right_hull[right_index])

            # Get the points in left and right hulls
            next_left_point = (left_index - 1) % l_size
            next_left_slope = self.slope(left_hull[next_left_point], right_hull[right_index])
            next_right_point = (right_index + 1) % r_size
            next_right_slope = self.slope(left_hull[left_index], right_hull[next_right_point])

            # Edge found so we break
            if next_left_slope <= left_slope and next_right_slope >= right_slope:
                break

            # If edges are not found move counter-clockwise
            while next_left_slope > left_slope:  # While the edge is not lower tangent to the left
                left_index = next_left_point
                left_slope = next_left_slope
                next_left_point = (left_index - 1) % l_size
                next_left_slope = self.slope(left_hull[next_left_point], right_hull[right_index])

            # Clockwise move
            while next_right_slope < right_slope:  # While the edge is not lower tangent to the right
                right_index = next_right_point
                right_slope = next_right_slope
                next_right_point = (right_index + 1) % r_size
                next_right_slope = self.slope(left_hull[left_index], right_hull[next_right_point])
        return left_hull[left_index], right_hull[right_index]

    """
    Divides a list of points into two halves and calls the divide_and_conquer algorithm

    :Param points: A list of points
    :Return: A combined hull

    :Complexity: O(n log n) because the function recursively divides the list of points into halves until there are 
    two or fewer points left, and then merges the convex hulls of the sub-problems. At each recursion level, 
    the size of the problem is halved, resulting in a binary tree of recursive calls. Each level of recursion 
    involves sorting the points (O(n log n) time complexity) and merging the hulls (potentially O(n) time 
    complexity).
    :Space Complexity: O(log n) because recursion depth in this algorithm is O(log n), where n is the number of points.
    """

    def divide_and_conquer(self, points):
        point_length = len(points)

        if point_length == 1:
            # Base case: return the points when there are 1
            return points

        mid = point_length // 2
        left_side = points[:mid]
        right_side = points[mid:]

        # Recursive case: divide the points and get left and right hulls
        left_hull = self.divide_and_conquer(left_side)
        right_hull = self.divide_and_conquer(right_side)

        self.pause
        if self.pause:
            # Display left hulls
            l_hull = [QLineF(left_hull[i], left_hull[(i + 1) % len(left_hull)]) for i in range(len(left_hull))]
            self.showHull(l_hull, GREEN)

        if self.pause:
            r_hull = [QLineF(right_hull[i], right_hull[(i + 1) % len(right_hull)]) for i in range(len(right_hull))]
            self.showHull(r_hull, BLUE)

        # Find tangents
        hull = self.merge(left_hull, right_hull)
        if self.pause:
            self.eraseHull(l_hull)
            self.eraseHull(r_hull)

        return hull  # Return the computed hull

    """
    Merges two hulls into one

    :Param left_hull: A list of left hull points
    :Param right_hull: A list of right hull points
    :Return: A merged hull

    :Complexity: O(n log n), because the function merges two convex hulls.  It involves iterating through the points 
    of the hulls once, determining the upper and lower tangent points (O(n) time complexity), and then merging the 
    hulls between these tangent points. The  merging process involves appending the points to the combined hull, 
    resulting in a a linear operation in the size of the hulls. 
    :Space Complexity: O(n) - simply creates a new list 
    and stores the combined hull. The size of the list is at most the sum of the sizes of the hulls.
    """

    def merge(self, left_hull, right_hull):
        combined_hull = []
        l_size = len(left_hull)
        r_size = len(right_hull)

        # Get edges
        upper_left, upper_right = self.get_upper_tangent(left_hull, right_hull)
        lower_left, lower_right = self.get_lower_tangent(left_hull, right_hull)

        # Handle cases where size is less than 3
        for i in range(l_size):
            if l_size < 3:
                combined_hull.append(left_hull[i])
        for i in range(r_size):
            if r_size < 3:
                combined_hull.append(right_hull[i])

        # Merge from upper right tangent to lower right tangent
        i = right_hull.index(upper_right)
        while right_hull[i] != lower_right:
            combined_hull.append(right_hull[i])
            i = (i + 1) % r_size
        combined_hull.append(lower_right)

        # Merge from lower left tangent to upper left tangent
        i = left_hull.index(lower_left)
        while left_hull[i] != upper_left:
            combined_hull.append(left_hull[i])
            i = (i + 1) % l_size
        combined_hull.append(upper_left)

        return combined_hull

    """
    Computes the convex hull of a list of points

    :Param points: A list of points
    :Return: A computed convex hull 

    :Complexity: O(n log n), because it computes the convex hull. It first sorts the points and then calls the 
    divide_and_conquer function. Resulting into O(n log n) time complexity. 
    :Space Complexity: O(n) - this function does not use any additional memory proportional to the input size from 
    storing the points and the hull.
    """

    def compute_hull(self, points, pause, view):
        self.pause = pause
        self.view = view
        assert (type(points) == list and type(points[0]) == QPointF)
        t1 = time.time()

        # Sort the points by increasing x-coordinate
        points = self.sortPoints(points)
        t2 = time.time()

        t3 = time.time()
        divider = self.divide_and_conquer(points)
        polygon = [QLineF(divider[i], divider[(i + 1) % len(divider)]) for i in range(len(divider))]

        t4 = time.time()

        # Display the convex hull using QLineF objects
        self.showHull(polygon, RED)
        self.showText('Time Elapsed (Convex Hull): {:3.3f} sec'.format(t4 - t3))

    """
    Main function for testing

    :Complexity: O(n log n), same reason as compute_hull function. 
    Space Complexity: O(n) - same reason as compute_hull function.
    """

    def main(self):
        # Generate random points for testing
        num_points = 20
        points = [QPointF(random.uniform(0, 10), random.uniform(0, 10)) for _ in range(num_points)]

        # Sort points by increasing x-coordinate
        points = self.sortPoints(points)

        # Display the sorted points
        print("Sorted Points:")
        for point in points:
            print(point.x(), point.y())

        # Call the divide-and-conquer convex hull algorithm
        convex_hull = self.divide_and_conquer(points)
        print("Number of points in Convex Hull: ", len(convex_hull))

        # Display the convex hull
        print("\nConvex Hull:")
        for point in convex_hull:
            print(point.x(), point.y())


# Instantiate the ConvexHullSolver
solver = ConvexHullSolver()

# Call the main function for testing
solver.main()
