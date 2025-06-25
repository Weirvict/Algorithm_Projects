#!/usr/bin/python3

from which_pyqt import PYQT_VER

if PYQT_VER == 'PYQT5':
    from PyQt5.QtCore import QLineF, QPointF
elif PYQT_VER == 'PYQT4':
    from PyQt4.QtCore import QLineF, QPointF
else:
    raise Exception('Unsupported Version of PyQt: {}'.format(PYQT_VER))

from TSPClasses import *
import heapq


class TSPSolver:
    def __init__(self, gui_view):
        self._scenario = None

    def setupWithScenario(self, scenario):
        self._scenario = scenario

    ''' <summary>
        This is the entry point for the default solver
        which just finds a valid random tour.  Note this could be used to find your
        initial BSSF.
        </summary>
        <returns>results dictionary for GUI that contains three ints: cost of solution, 
        time spent to find solution, number of permutations tried during search, the 
        solution found, and three null values for fields not used for this 
        algorithm</returns> 
    '''

    """
    This function reduces the cost matrix by subtracting row and column minima.
    
    @:parameter costmatrix: Current cost matrix.
    @:return lowbound: Total reduction in cost (lower bound).
    
    Space complexity is O(n^2) where n is the number of cities.
    Time complexity is O(n^2) where n is the number of cities.
    """
    def reduceCostMatrix(self, costmatrix):
        # Initialize lower bound for the reduced cost matrix
        lowbound = 0

        # Iterate over each row in the cost matrix
        for i in range(len(costmatrix)):
            # Find the minimum cost in the current row
            row_min = min(costmatrix[i])

            # If the minimum cost is not infinity, subtract it from each element in the row
            if row_min != math.inf:
                lowbound += row_min
                costmatrix[i] -= row_min

        # Iterate over each column in the cost matrix
        for j in range(len(costmatrix[0])):
            # Find the minimum cost in the current column
            column_min = min(costmatrix[:, j])

            # If the minimum cost is not infinity, subtract it from each element in the column
            if column_min != math.inf:
                lowbound += column_min
                costmatrix[:, j] -= column_min

        # Return the total reduction in cost (lower bound)
        return lowbound

    def defaultRandomTour(self, time_allowance=60.0):
        results = {}
        cities = self._scenario.getCities()
        ncities = len(cities)
        foundTour = False
        count = 0
        bssf = None
        start_time = time.time()
        while not foundTour and time.time() - start_time < time_allowance:
            # create a random permutation
            perm = np.random.permutation(ncities)
            route = []
            # Now build the route using the random permutation
            for i in range(ncities):
                route.append(cities[perm[i]])
            bssf = TSPSolution(route)
            count += 1
            if bssf.cost < np.inf:
                # Found a valid route
                foundTour = True
        end_time = time.time()
        results['cost'] = bssf.cost if foundTour else math.inf
        results['time'] = end_time - start_time
        results['count'] = count
        results['soln'] = bssf
        results['max'] = None
        results['total'] = None
        results['pruned'] = None
        return results

    ''' <summary>
        This is the entry point for the greedy solver, which you must implement for 
        the group project (but it is probably a good idea to just do it for the branch-and
        bound project as a way to get your feet wet).  Note this could be used to find your
        initial BSSF.
        </summary>
        <returns>results dictionary for GUI that contains three ints: cost of best solution, 
        time spent to find best solution, total number of solutions found, the best
        solution found, and three null values for fields not used for this 
        algorithm</returns> 
    '''

    def greedy(self, time_allowance=60.0):
        pass

    ''' <summary>
        This is the entry point for the branch-and-bound algorithm that you will implement
        </summary>
        <returns>results dictionary for GUI that contains three ints: cost of best solution, 
        time spent to find best solution, total number solutions found during search (does
        not include the initial BSSF), the best solution found, and three more ints: 
        max queue size, total number of states created, and number of pruned states.</returns> 
    '''

    def branchAndBound(self, time_allowance=60.0):

        ''' <summary>
        This is the entry point for the algorithm you'll write for your group project.
        </summary>
        <returns>results dictionary for GUI that contains three ints: cost of best solution,
        time spent to find best solution, total number of solutions found during search, the
        best solution found.  You may use the other three field however you like.
        algorithm</returns>
        '''
        # Start the timer
        start_time = time.time()

        # Get the list of cities from the scenario
        cities = self._scenario.getCities()

        # Initialize the cost matrix
        costmatrix = np.array([[cities[i].costTo(cities[j])
                                for j in range(len(cities))] for i in range(len(cities))])

        # Reduce the initial cost matrix
        lowerbound = self.reduceCostMatrix(costmatrix)

        # Initialize priority queue with root state
        pq = []
        heapq.heappush(pq, (len(cities) - 1, lowerbound, [0], costmatrix))

        # Get an initial solution from default random tour
        initial_bssf = self.defaultRandomTour(time.time())

        # Initialize the best solution so far
        bssf = {
            'cost': initial_bssf['cost'],
            'soln': initial_bssf['soln'],
            'count': 1
        }

        # Initialize counters for states and pruning
        total_states = 1
        pruned_states = 0
        max_queue_size = 0

        # Main loop of the branch-and-bound algorithm
        while pq and time.time() - start_time < time_allowance:
            # Pop state from priority queue
            state = heapq.heappop(pq)

            # Extract information from the popped state
            depth = len(cities) - state[0]  # Depth of the current state in the search tree
            lowerbound = state[1]  # Lower bound of the current state
            visited = state[2]  # List of indices of visited cities in the current state
            costmatrix = state[3]  # Cost matrix associated with the current state

            # Reached leaf node in the search tree
            if depth == len(cities):

                # Update the best solution if a better solution is found
                if lowerbound < bssf['cost']:
                    # Update the solution with the current visited cities
                    bssf['soln'] = [cities[i] for i in visited]

                    # Create a TSPSolution object using the updated solution
                    bssf['soln'] = TSPSolution(bssf['soln'])

                    # Update the cost of the best solution
                    bssf['cost'] = bssf['soln']._costOfRoute()

                    # Increment the count of found solutions
                    bssf['count'] += 1

                # Continue to explore other branches of the search tree
                continue

            # Explore child states
            for i in range(1, len(cities)):
                # Initialize new lower bound for the child state
                new_lowerbound = lowerbound

                # Check if the edge from the last visited city to city 'i' exists
                if costmatrix[visited[-1], i] != math.inf:
                    # Create a copy of the current cost matrix for the child state
                    new_costmatrix = np.copy(costmatrix)

                    # Update the lower bound and cost matrix for the child state
                    # Add the cost of the edge from the last visited city to city 'i' to the lower bound
                    new_lowerbound += new_costmatrix[visited[-1], i]

                    # Set all costs associated with the last visited city to infinity
                    new_costmatrix[visited[-1], :] = math.inf

                    # Set all costs associated with city 'i' to infinity
                    new_costmatrix[:, i] = math.inf

                    # Set the cost of the edge from city 'i' to the last visited city to infinity
                    new_costmatrix[i, visited[-1]] = math.inf

                    # Reduce the cost matrix to update its lower bound
                    new_lowerbound += self.reduceCostMatrix(new_costmatrix)

                    # Increment the total number of states explored
                    total_states += 1

                    # If the new lower bound is better than the current best solution,
                    # add the child state to the priority queue for further exploration
                    if new_lowerbound < bssf['cost']:
                        new_visited = visited + [i]
                        heapq.heappush(pq,
                                       (len(cities) - depth - 1, new_lowerbound, new_visited, new_costmatrix))
                        # Update the maximum queue size
                        max_queue_size = max(max_queue_size, len(pq))
                    else:
                        # Increment the count of pruned states
                        pruned_states += 1

        # Update the results with final metrics
        bssf['time'] = time.time() - start_time
        bssf['max'] = max_queue_size
        bssf['total'] = total_states
        bssf['pruned'] = pruned_states

        # Return the best solution found along with metrics
        return bssf

    def fancy(self, time_allowance=60.0):
        pass




