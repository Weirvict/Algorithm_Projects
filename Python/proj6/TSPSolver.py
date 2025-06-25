#!/usr/bin/python3

from which_pyqt import PYQT_VER

if PYQT_VER == 'PYQT5':
    from PyQt5.QtCore import QLineF, QPointF
elif PYQT_VER == 'PYQT4':
    from PyQt4.QtCore import QLineF, QPointF
else:
    raise Exception('Unsupported Version of PyQt: {}'.format(PYQT_VER))

import time
import numpy as np
from TSPClasses import *
import heapq
import itertools


class TSPSolver:
    def __init__(self, gui_view):
        self._scenario = None
        self.best_fitness = None
        self.curr_fitness = None
        self.fitness_list = None

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

    def greedy(self, time_allowance=60.0):
        results = {}
        start_time = time.time()
        cities = self._scenario.getCities()
        numCities = len(cities)
        tourFound = False
        count = 0
        # print('start greedy')
        while not tourFound:
            p = np.random.permutation(numCities)
            route = []
            for i in range(numCities):
                route.append(cities[p[i]])
            bssf = TSPSolution(route)
            count += 1

            if bssf._costOfRoute() < np.inf:
                tourFound = True
        # print('finishing')
        # end_time = time.time()
        results['cost'] = bssf._costOfRoute()
        results['time'] = time.time() - start_time
        results['count'] = count
        results['soln'] = bssf

        return results

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

    def greed_fancy(self, cities, numCities, best_cost, time_allowance=60.0):
        # Initialize results dictionary
        results = {}

        # Select a random city as the starting point
        curr_city = random.choice(cities)

        # Initialize the route with the current city
        route = [curr_city]

        # Create a set of unvisited cities
        free_cities = set(cities)
        free_cities.remove(curr_city)

        # Start building the tour until all cities are visited
        while free_cities:
            # Find the nearest unvisited city to the current city
            next_city = min(free_cities, key=lambda x: curr_city.costTo(x))

            # Remove the next city from the set of unvisited cities
            free_cities.remove(next_city)

            # Add the next city to the route
            route.append(next_city)

            # Update the current city
            curr_city = next_city

        # Create a TSPSolution object for the constructed route
        bssf = TSPSolution(route)

        # Update the best fitness if the new solution is better
        if bssf._costOfRoute() < best_cost:
            self.best_fitness = bssf

        # Append the current fitness to the fitness list
        self.fitness_list.append(bssf)

        # Return the result dictionary
        return bssf

    def fancy(self, time_allowance=60.0):
        # Initialize results dictionary
        results = {}

        # Start the timer
        start_time = time.time()

        # Get the list of cities from the scenario
        cities = self._scenario.getCities()

        # Get the number of cities
        numCities = len(cities)

        # Initialize the best fitness to infinity
        self.best_fitness = np.inf

        # Initialize the fitness list
        self.fitness_list = []

        # Use greedy algorithm to generate an initial solution
        self.best_fitness = self.greed_fancy(cities, numCities, np.inf)

        # Set the current fitness to the best fitness
        self.curr_fitness = self.best_fitness

        # Perform simulated annealing
        self.simAnnealing(cities, numCities, start_time, 0)

        # Get the current best solution
        curr_bssf = self.best_fitness

        # Fill in results dictionary
        results['cost'] = curr_bssf._costOfRoute()
        results['time'] = time.time() - start_time
        results['max'] = None  # Not used for this algorithm
        results['count'] = len(self.fitness_list)  # Total number of fitness evaluations
        results['total'] = len(self.fitness_list)  # Total number of fitness evaluations
        results['pruned'] = None  # Not used for this algorithm
        results['soln'] = curr_bssf  # Best solution found

        # Return the result dictionary
        return results

    """
        This function performs simulated annealing to explore the solution space.

        @:parameters:
            cities: list
                List of City objects representing the cities.
            numCities: int
                Number of cities in the TSP instance.
            start_time: float
                Start time of the algorithm.
            count: int
                Counter for the number of iterations.
            time_allowance: float, optional
                Maximum time allowed for the algorithm (default is 60.0 seconds).

        @:return:
            None

        Space complexity: O(n^2), where n is the number of cities.
        Time complexity:  O(n^2), where n is the number of cities.
        """
    def simAnnealing(self, cities, numCities, start_time, count, time_allowance=60.0):
        # Set parameters for simulated annealing
        alpha = 0.995
        stoppingTemp = 1e-10
        temperature = math.sqrt(numCities)

        # Perform simulated annealing until stopping criteria are met
        while temperature >= stoppingTemp and time.time() - start_time < time_allowance:
            # Make a copy of the current fitness route
            temp = list(self.curr_fitness.route)

            # Randomly select a segment of the route to reverse
            l = random.randint(2, numCities - 1)
            i = random.randint(0, numCities - l)
            temp[i: (i + l)] = reversed(temp[i: (i + l)])

            # Create a new TSPSolution object for the modified route
            temp_bssf = TSPSolution(temp)

            # Accept or reject the modified solution based on temperature and cost difference
            self.accept(temp_bssf, temperature, count)

            # Update temperature
            temperature *= alpha

            # Append the modified solution to the fitness list
            self.fitness_list.append(temp_bssf)

    """
        This function determines whether to accept a new solution based on the Metropolis criterion.

        @:parameters:
            temp: TSPSolution
                New solution to consider.
            temperature: float
                Current temperature in the simulated annealing process.
            count: int
                Counter for the number of accepted solutions.

        @:return:
            None

        Space complexity: O(1)
        Time complexity: O(1)
    """
    def accept(self, temp, temperature, count):
        # Accept or reject the modified solution based on temperature and cost difference
        if temp._costOfRoute() < self.curr_fitness._costOfRoute():
            self.curr_fitness = temp
            if self.curr_fitness._costOfRoute() < self.best_fitness._costOfRoute():
                self.best_fitness = temp
                count += 1
        else:
            if random.random() < self.probability_accept(temp, temperature):
                self.curr_fitness = temp

    """
        This function calculates the acceptance probability for a new solution in simulated annealing.

        @:parameters:
            temp: TSPSolution
                New solution to consider.
            temperature: float
                Current temperature in the simulated annealing process.

        @:return:
            float
                Acceptance probability.

        Space complexity: O(1)
        Time complexity: O(1)
    """
    def probability_accept(self, temp, temperature):
        # Calculate the acceptance probability based on temperature and cost difference
        return math.exp(-abs(temp._costOfRoute() - self.curr_fitness._costOfRoute()) / temperature)

    """
        This function reduces the cost matrix by subtracting the minimum value of each row from all elements in that row.
        It then returns the sum of all the minimum values.

        @:parameters:
            costArray: np.array
                Cost matrix.
            numCities: int
                Number of cities.

        @:return:
            float
                Total sum of the minimum values after row reduction.

        Space complexity: O(n), where n is the number of cities.
        Time complexity: O(n^2), where n is the number of cities.
    """
    def rowRedux(self, costArray, numCities):
        # Get the minimum value in each row
        temp = costArray.min(1)

        # Subtract the minimum value of each row from all elements in that row
        for i in range(numCities):
            for j in range(numCities):
                if np.isinf(temp[i]):
                    temp[i] = 0
                if not np.isinf(costArray[i, j]):
                    costArray[i, j] -= temp[i]

        # Return the sum of all the minimum values
        return temp.sum()

    """
        This function reduces the cost matrix by subtracting the minimum value of each column from all elements in that column.
        It then returns the sum of all the minimum values.

        @:parameters:
            costArray: np.array
                Cost matrix.
            numCities: int
                Number of cities.

        @:return:
            float
                Total sum of the minimum values after column reduction.

        Space complexity: O(n), where n is the number of cities.
        Time complexity: O(n^2), where n is the number of cities.
    """
    def colRedux(self, costArray, numCities):
        # Get the minimum value in each column
        temp = costArray.min(0)

        # Subtract the minimum value of each column from all elements in that column
        for j in range(numCities):
            for i in range(numCities):
                if np.isinf(temp[i]):
                    temp[j] = 0
                if not np.isinf(costArray[i, j]):
                    costArray[i, j] -= temp[j]

        # Return the sum of all the minimum values
        return temp.sum()

    """
        This function builds an initial node for the branch-and-bound algorithm.
        It initializes a cost matrix based on the distances between cities.
        It then reduces the cost matrix and calculates the lower bound.

        @:parameters:
            cities: list
                List of City objects representing the cities.
            numCities: int
                Number of cities in the TSP instance.
            curr: int
                Index of the current city.

        @:return:
            KeyDict
                Initial node containing the lower bound, node information, and other necessary data.

        Space complexity: O(n^2), where n is the number of cities.
        Time complexity: O(n^2), where n is the number of cities.
    """
    def buildInitialNode(self, cities, numCities, curr):
        # Create a cost matrix based on the distances between cities
        costArray = np.array([[cities[i].costTo(cities[j]) for j in range(numCities)] for i in range(numCities)])

        # Calculate the minimum values for each row and column
        xMin = costArray.min(axis=1)
        yMin = costArray.min(axis=0)

        # Calculate the lower bound by reducing the cost matrix
        LB = self.rowRedux(costArray, numCities) + self.colRedux(costArray, numCities)

        # Create a KeyDict object containing node information and lower bound
        return KeyDict(LB, {"Node": cities[curr], "LB": LB, "Array": costArray, "Route": [curr], "Index": curr})

    """
        This function builds a child node for the branch-and-bound algorithm.
        It updates the cost matrix, route, and lower bound based on the parent node and the edge to the current city.

        @:parameters:
            costArray: np.array
                Cost matrix.
            cities: list
                List of City objects representing the cities.
            numCities: int
                Number of cities in the TSP instance.
            prev: int
                Index of the previous city.
            curr: int
                Index of the current city.
            route: list
                List of indices representing the current route.
            LB: float
                Lower bound of the parent node.

        @:return:
            KeyDict
                Child node containing the updated lower bound, node information, and other necessary data.

        Space complexity: O(n^2), where n is the number of cities.
        Time complexity: O(n^2), where n is the number of cities.
    """
    def buildNode(self, costArray, cities, numCities, prev, curr, route, LB):
        # Update the lower bound with the cost of the edge from the previous city to the current city
        new_LB = LB + costArray[prev, curr]

        # Set the cost of the edge from the previous city to the current city to infinity
        costArray[prev, curr] = np.inf

        # Set all costs associated with the previous city to infinity
        costArray[prev, :] = np.inf

        # Set all costs associated with the current city to infinity
        costArray[:, curr] = np.inf

        # Calculate the new lower bound by reducing the updated cost matrix
        new_LB += self.rowRedux(costArray, numCities)
        new_LB += self.colRedux(costArray, numCities)

        # Create a new route by appending the current city to the existing route
        new_route = route.copy()
        new_route.append(curr)

        # Create a KeyDict object containing the updated lower bound, node information, and other necessary data
        return KeyDict(new_LB - 400 * len(new_route),
                       {"Node": cities[curr], "LB": new_LB, "Array": costArray, "Route": new_route, "Index": curr})


class KeyDict(object):
    def __init__(self, key, dct):
        self.key = key
        self.dct = dct

    def __lt__(self, other):
        return self.key < other.key

    def __eq__(self, other):
        return self.key == other.key

    def __format__(self, format_spec):
        if format_spec == 'r':
            return '{0.__class__.__name__}(key={0.key}, dct={0.dct})'.format(self)
        return str(self.key)
