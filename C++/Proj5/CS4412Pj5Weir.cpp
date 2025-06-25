#include <iostream>
#include <random>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <algorithm>
#include <queue>
#include <thread>
#include <stack>

using namespace std;

/**
 * Author: Victoria Weir
 * Class: CS4412
 * Date: 11-12-23
 * Details: Project 5 - Maze and Graphs
 *
 */

// Define color codes for console text
const int DEFAULT_COLOR = 7; // White
const int RED_COLOR = 12;     // Red

/**
 * Sets the console text color using the specified color code.
 *
 * This function changes the text color in the console to the specified color code.
 * The color codes are specific to the Windows console.
 *
 * @param color An integer representing the color code to set.
 */
void setColor(int color) {
    // Set console text color (specific to Windows console)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

/**
 * Resets the console text color to the default color.
 *
 * This function resets the console text color to the default color.
 */
void resetColor() {
    // Reset console text color to default
    setColor(DEFAULT_COLOR);
}

// Cell class represents an individual cell in the maze.
class Cell {
public:
    bool topWall;
    bool rightWall;
    bool bottomWall;
    bool leftWall;
    bool visited;
    bool path;


    //The constructor initializes cell properties, such as walls and visits status.
    Cell() {
        // Initialize cell properties
        topWall = true;
        rightWall = true;
        bottomWall = true;
        leftWall = true;
        visited = false;
        path = false;
    }
};

// Maze class represents a 2D maze with cells and provides methods for maze generation and pathfinding.
class Maze {
public:
    int rows;
    int cols;
    vector<vector<Cell>> cells;
    // Vector to store the shortest path
    vector<pair<int, int>> shortestPath;
    vector<vector<pair<int, int>>> prevCell;
    // Stack to store previous positions for backtracking
    stack<vector<int>> previousPositions;
    // Member variable to store cumulative costs
    vector<vector<int>> cumulativeCost;

    /**
     * Maze class constructor initializes a maze with the specified number of rows and columns.
     *
     * @param numRows The number of rows in the maze.
     * @param numCols The number of columns in the maze.
     */
    Maze(int numRows, int numCols) : rows(numRows), cols(numCols) {
        // Initialize the maze with cells
        cells.resize(rows, vector<Cell>(cols));

        // Initialize the prevCell vector for backtracking during pathfinding
        prevCell.resize(rows, vector<pair<int, int>>(cols, {-1, -1}));
    }

    // Reset the visited cells
    void resetVisited() {
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                cells[row][col].visited = false;
            }
        }
    }

    /**
     * Generates a maze using a randomized depth-first search algorithm.
     *
     * The maze is created by establishing a path from the top-left corner (start)
     * to the bottom-right corner (end). The algorithm randomly explores neighboring
     * cells, creating paths and removing walls between them.
     */
    void generateMaze() {
        // Create a path from the top-left corner (start) to the bottom-right corner (end)
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                if (row == 0) {
                    cells[row][col].topWall = true;
                    if (col > 0) cells[row][col - 1].rightWall = false;
                }
                if (col == 0) {
                    cells[row][col].leftWall = true;
                    if (row > 0) cells[row - 1][col].bottomWall = false;
                }
            }
        }
        // Initialize random number generator
        srand(time(nullptr));
        // Start the generation process from the bottom-right cell
        visitCell(rows - 1, cols - 1);
    }

    /**
     * Recursively visits the specified cell during maze generation.
     *
     * This function marks the cell as visited and randomly explores neighboring
     * cells to create paths. It uses a randomized depth-first search algorithm.
     *
     * @param row The row index of the cell to visit.
     * @param col The column index of the cell to visit.
     */
    void visitCell(int row, int col) {
        cells[row][col].visited = true;

        // Order in which to check neighboring cells
        int directions[] = {1, 2, 3, 4};
        shuffle(directions, directions + 4, mt19937(random_device()()));

        // Iterate over the shuffled directions to explore neighboring cells
        for (int direction: directions) {
            // Check if moving upward is a valid option and the neighbor cell is not visited
            if (direction == 1 && row > 0 && !cells[row - 1][col].visited) {
                // Knock down the top wall and the corresponding bottom wall of the neighbor
                cells[row][col].topWall = false;
                cells[row - 1][col].bottomWall = false;
                // Recursively visit the neighbor cell
                visitCell(row - 1, col);
            }
            // Check if moving rightward is a valid option and the neighbor cell is not visited
            else if (direction == 2 && col < cols - 1 && !cells[row][col + 1].visited) {
                // Knock down the right wall and the corresponding left wall of the neighbor
                cells[row][col].rightWall = false;
                cells[row][col + 1].leftWall = false;
                // Recursively visit the neighbor cell
                visitCell(row, col + 1);
            }
            // Check if moving downward is a valid option and the neighbor cell is not visited
            else if (direction == 3 && row < rows - 1 && !cells[row + 1][col].visited) {
                // Knock down the bottom wall and the corresponding top wall of the neighbor
                cells[row][col].bottomWall = false;
                cells[row + 1][col].topWall = false;
                // Recursively visit the neighbor cell
                visitCell(row + 1, col);
            }
            // Check if moving leftward is a valid option and the neighbor cell is not visited
            else if (direction == 4 && col > 0 && !cells[row][col - 1].visited) {
                // Knock down the left wall and the corresponding right wall of the neighbor
                cells[row][col].leftWall = false;
                cells[row][col - 1].rightWall = false;
                // Recursively visit the neighbor cell
                visitCell(row, col - 1);
            }
        }
    }

    /**
     * Checks if movement is allowed to the specified coordinates.
     *
     * Movement is allowed if the position is within the maze bounds, is not part
     * of the current path, and is not blocked by walls.
     *
     * @param x The x-coordinate of the position.
     * @param y The y-coordinate of the position.
     * @return True if movement is allowed; otherwise, false.
     */
    [[nodiscard]] bool isInBounds(int x, int y) const {
        if (x >= rows || x < 0)
            return false;
        if (y >= cols || y < 0)
            return false;
        return true;
    }

    /**
     * Checks if movement is allowed to the specified coordinates.
     *
     * Movement is allowed if the position is within the maze bounds, is not part
     * of the current path, and is not blocked by walls.
     *
     * @param x The x-coordinate of the position.
     * @param y The y-coordinate of the position.
     * @return True if movement is allowed; otherwise, false.
     */
    bool isMovementAllowed(int x, int y) {
        if (!isInBounds(x, y)) {
            return false; // Out of bounds
        }

        // Check if the current coordinates are part of valid spaces
        vector<vector<int>> validSpaces;
        getValidSpaces(validSpaces);

        for (const auto& coords : validSpaces) {
            if (coords[0] == x && coords[1] == y) {
                return true; // Current coordinates are part of valid spaces
            }
        }

        // Check for walls
        if (cells[y][x].topWall || cells[y][x].rightWall || cells[y][x].bottomWall || cells[y][x].leftWall) {
            return false; // Wall encountered
        }
        return true;
    }

    /**
     * Retrieves all valid spaces in the maze.
     *
     * A valid space is a cell that is either part of the current path or an open
     * cell (not blocked by walls).
     *
     * @param validSpaces A 2D vector to store the coordinates of valid spaces.
     */
    void getValidSpaces(vector<vector<int>> &validSpaces) {
        for (int y = 0; y < cols; y++) {
            for (int x = 0; x < rows; x++) {
                if (cells[y][x].path || (!cells[y][x].topWall && !cells[y][x].rightWall && !cells[y][x].bottomWall &&
                                         !cells[y][x].leftWall)) {
                    // Include cells that are part of the path or are open (not a wall)
                    vector<int> tempVector;
                    tempVector.push_back(x);
                    tempVector.push_back(y);
                    validSpaces.push_back(tempVector);

                    if (isJunction(x, y)) {
                        // If it's a junction, try to move to a valid space
                        moveFromJunction(x, y);
                    } else if (isDeadEnd(x, y)) {
                        // If it's a dead end, go back and try a different way
                        backtrackFromDeadEnd(x, y);
                    }
                }
            }
        }
    }

    /**
     * Chooses a direction among the available valid spaces based on the shortest cost.
     *
     * This function takes a vector of valid spaces and their cumulative costs and
     * selects the direction with the minimum cost. The chosen direction is returned.
     *
     * @param validSpaces A vector of coordinates representing valid spaces.
     * @param cumulativeCost A vector of vectors containing cumulative costs for each cell.
     * @return A vector representing the chosen direction.
     */
    vector<int> chooseDirection(const vector<vector<int>> &validSpaces, const vector<vector<int>> &cumulativeCost) {
        // Check if there are valid spaces
        if (validSpaces.empty()) {
            // No valid spaces, return an empty vector
            return {};
        }

        // Initialize the minimum cost with the maximum possible value
        int minCost = numeric_limits<int>::max();

        // Initialize the chosen direction
        vector<int> chosenDirection;

        // Iterate through each valid space and find the one with the minimum cost
        for (const auto &space : validSpaces) {
            int x = space[0];
            int y = space[1];

            // Check if the cumulative cost for this space is less than the current minimum cost
            if (cumulativeCost[y][x] < minCost) {
                // Update the minimum cost and the chosen direction
                minCost = cumulativeCost[y][x];
                chosenDirection = space;
            }
        }

        // Return the chosen direction
        return chosenDirection;
    }


    /**
     * Updates the current position to the specified coordinates.
     *
     * This function updates the current position to the specified x and y coordinates.
     *
     * @param x The x-coordinate of the new position.
     * @param y The y-coordinate of the new position.
     */
    void updateCurrentPosition(int x, int y) {
        previousPositions.push({x, y});
    }

    /**
     * Moves from a junction in the maze to a neighboring cell with the shortest cost.
     *
     * This function is called when the current position is identified as a junction,
     * meaning there are multiple open paths leading from it. It gets the valid spaces
     * around the junction, chooses the direction with the shortest cost, and updates
     * the current position to that direction.
     *
     * @param x The x-coordinate of the current cell.
     * @param y The y-coordinate of the current cell.
     */
    void moveFromJunction(int x, int y) {
        // Get valid spaces around the junction
        vector<vector<int>> validSpaces;
        getValidSpacesAround(x, y, validSpaces);

        // If there are valid spaces, choose the direction with the shortest cost
        if (!validSpaces.empty()) {
            // Choose the direction based on the shortest cost
            vector<int> chosenDirection = chooseDirection(validSpaces, cumulativeCost);

            if (!chosenDirection.empty()) {
                int newX = chosenDirection[0];
                int newY = chosenDirection[1];

                // Update the current position
                updateCurrentPosition(newX, newY);
            }
        }
    }

    /**
     * Backtracks from a dead end in the maze and tries a different way.
     *
     * This function is called when the current position is identified as a dead end,
     * meaning there is only one open path leading to it. The function pops the last
     * position from the stack of previous positions and updates the current position
     * to that position, allowing the algorithm to explore a different path.
     *
     * @param x The x-coordinate of the current cell.
     * @param y The y-coordinate of the current cell.
     */
    void backtrackFromDeadEnd(int x, int y) {
        // Check if there's a previous position to go back to
        if (!previousPositions.empty()) {
            // Pop the last position from the stack
            vector<int> lastPosition = previousPositions.top();
            previousPositions.pop();

            // Update the current position to the last position
            int newX = lastPosition[0];
            int newY = lastPosition[1];
            updateCurrentPosition(newX, newY);
        }
    }

    /**
     * Gets the valid spaces around the specified coordinates in the maze.
     *
     * A valid space is a cell that is either part of the current path or an open
     * cell (not blocked by walls).
     *
     * @param x The x-coordinate of the current cell.
     * @param y The y-coordinate of the current cell.
     * @param validSpaces A 2D vector to store the coordinates of valid spaces.
     */
    void getValidSpacesAround(int x, int y, vector<vector<int>> &validSpaces) {
        // Check if moving downward is a valid option
        if (isInBounds(x, y + 1) && !cells[y][x].bottomWall) {
            validSpaces.push_back({x, y + 1});
        }

        // Check if moving upward is a valid option
        if (isInBounds(x, y - 1) && !cells[y][x].topWall) {
            validSpaces.push_back({x, y - 1});
        }

        // Check if moving rightward is a valid option
        if (isInBounds(x + 1, y) && !cells[y][x].rightWall) {
            validSpaces.push_back({x + 1, y});
        }

        // Check if moving leftward is a valid option
        if (isInBounds(x - 1, y) && !cells[y][x].leftWall) {
            validSpaces.push_back({x - 1, y});
        }

        // Check for a dead end and handle it
        if (isDeadEnd(x, y)) {
            backtrackFromDeadEnd(x, y);
        }
    }


    /**
     * Checks if the given position in the maze is a junction.
     *
     * A junction is defined as a cell with three or more open paths, indicating
     * multiple ways to move from the current position.
     *
     * @param x The x-coordinate of the cell.
     * @param y The y-coordinate of the cell.
     * @return True if the cell is a junction, false otherwise.
     */
    bool isJunction(int x, int y) {
        // Initialize the count of valid exits from the current position
        int numberOfExits = 0;

        // Check if moving downward is a valid option
        if (isMovementAllowed(x, y + 1))
            numberOfExits++;

        // Check if moving upward is a valid option
        if (isMovementAllowed(x, y - 1))
            numberOfExits++;

        // Check if moving rightward is a valid option
        if (isMovementAllowed(x + 1, y))
            numberOfExits++;

        // Check if moving leftward is a valid option
        if (isMovementAllowed(x - 1, y))
            numberOfExits++;

        // If there are 3 or more valid exits, consider the position as a junction
        if (numberOfExits >= 3)
            return true;
        // If there are less than 3 valid exits, it's not a junction
        return false;

    }

    /**
     * Checks if the given position in the maze is a dead end.
     *
     * A dead end is defined as a cell with only one open path, leading to a dead end.
     *
     * @param x The x-coordinate of the cell.
     * @param y The y-coordinate of the cell.
     * @return True if the cell is a dead end, false otherwise.
     */
    bool isDeadEnd(int x, int y) {
        // Initialize the count of valid exits from the current position
        int numberOfExits = 0;
        // Check if moving downward is a valid option
        if (isMovementAllowed(x, y + 1))
            numberOfExits++;

        // Check if moving upward is a valid option
        if (isMovementAllowed(x, y - 1))
            numberOfExits++;

        // Check if moving rightward is a valid option
        if (isMovementAllowed(x + 1, y))
            numberOfExits++;

        // Check if moving leftward is a valid option
        if (isMovementAllowed(x - 1, y))
            numberOfExits++;

        // If there is only one valid exit, consider the position as a dead end
        if (numberOfExits == 1)
            return true;
        // If there is more than one valid exit, it's not a dead end
        return false;
    }

    // Define a structure to represent a cell with cost
    struct CostCell {
        int x;
        int y;
        int cost;

        CostCell(int x, int y, int cost) : x(x), y(y), cost(cost) {}
    };

    // Comparison function for priority queue
    struct CompareCost {
        bool operator()(const CostCell &a, const CostCell &b) const {
            return a.cost > b.cost;
        }
    };

    // Create a priority queue to store cells with cost
    priority_queue<CostCell, vector<CostCell>, CompareCost> pq;

    /**
     * Modified getShortestPath using Dijkstra's algorithm
     * @details This function uses Dijkstra's algorithm to find the shortest path
     *          from the top-left corner to the bottom-right corner of the maze.
     */
    void getShortestPath() {
        // Reset all visited flags
        resetVisited();

        // Initialize starting position
        int startX = 0;
        int startY = 0;

        // Add the starting cell to the priority queue with initial cost of 0
        pq.emplace(startX, startY, 0);

        // Initialize the cumulativeCost matrix
        cumulativeCost.resize(rows, vector<int>(cols, numeric_limits<int>::max()));
        cumulativeCost[startY][startX] = 0;  // Update cumulative cost for the starting cell

        // While the priority queue is not empty
        while (!pq.empty()) {
            // Pop the cell with the lowest cost
            CostCell currentCell = pq.top();
            pq.pop();
            int x = currentCell.x;
            int y = currentCell.y;

            // Check if the current cell has been visited before
            if (!cells[y][x].visited) {
                cells[y][x].visited = true;

                // Check if this cell is the destination (bottom-right corner)
                if (x == cols - 1 && y == rows - 1) {
                    // Reconstruct the shortest path from the destination to the start
                    reconstructPath(startX, startY, x, y);
                    return;
                }

                // Check if movement to the neighboring cell is allowed
                if (isMovementAllowed(x, y)) {
                    int newCost = cumulativeCost[x][y]; // Assuming each move has a cost of 1

                    pq.emplace(x, y, newCost); // Add the cell to the priority queue
                    prevCell[x][y] = {x, y};  // Update the previous cell for backtracking
                }
            }
        }
    }

    /**
     * Reconstructs the shortest path from the destination to the start using backtracking.
     *
     * @param startX The x-coordinate of the start cell.
     * @param startY The y-coordinate of the start cell.
     * @param endX The x-coordinate of the destination cell.
     * @param endY The y-coordinate of the destination cell.
     */
    void reconstructPath(int startX, int startY, int endX, int endY) {
        int x = endX;
        int y = endY;

        // Backtrack from the destination to the start
        while (x != startX || y != startY) {
            // Mark the current cell as part of the path
            cells[y][x].path = true;

            // Add the cell to the shortest path
            shortestPath.emplace_back(x, y);

            // Store the coordinates of the previous cell
            int prevX = x;
            int prevY = y;

            // Update the current position to the coordinates of the previous cell
            x = prevCell[prevY][prevX].first;
            y = prevCell[prevY][prevX].second;
        }

        // Reverse the shortest path to get the correct order
        reverse(shortestPath.begin(), shortestPath.end());
    }


    /**
     * Display the current state of the maze in the console.
     * @param maze An instance of the Maze class representing the maze to be displayed.
     * The function uses specific characters to represent walls, paths, and other elements.
     */
    static void displayMaze(const Maze &maze) {
        // Set console text colors (specific to Windows console)
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        // Loop through each row of the maze
        for (int row = 0; row < maze.rows; row++) {
            // Display the top walls of each cell in the current row
            for (int col = 0; col < maze.cols; col++) {
                if (maze.cells[row][col].topWall)
                    cout << "+---";
                else
                    cout << "+   ";
            }
            cout << "+" << endl;

            // Display the left walls, cell contents, and right walls of each cell in the current row
            for (int col = 0; col < maze.cols; col++) {
                if (maze.cells[row][col].leftWall)
                    cout << "|";
                else
                    cout << " ";

                // Check if the cell is part of the path
                if (maze.cells[row][col].path) {
                    SetConsoleTextAttribute(hConsole, RED_COLOR); // Set text color to red
                    cout << " x ";  // Display 'x' for the path
                } else {
                    SetConsoleTextAttribute(hConsole, DEFAULT_COLOR); // Set text color to default
                    cout << "   ";  // Display empty space for non-path cells
                }
            }

            // Display the right wall of the last cell in the current row
            if (maze.cells[row][maze.cols - 1].rightWall)
                cout << "|";
            else
                cout << " ";
            cout << endl;
        }

        // Display the bottom walls of each cell in the last row
        for (int col = 0; col < maze.cols; col++) {
            cout << "+---";
        }
        cout << "+" << endl;
    }
};

int main() {
    int rows;
    int cols;
    int choice = 0;

    // Get the size of maze the user wants
    cout << "Choose between: \n"
            "0. Exit \n"
            "1. 5x5 \n"
            "2. 10x10 \n"
            "3. 15x15 \n"
            "4. 20x20 \n"
            "5. 25x25 \n";
    cout << "Please enter the integer(0-5) to choose: ";
    cin >> choice;
    if (choice == 0) {
        cout << "Ending the Maze";
        return 0;
    } else if (choice == 1) {
        rows = 5;
        cols = 5;
    } else if (choice == 2) {
        rows = 10;
        cols = 10;
    } else if (choice == 3) {
        rows = 15;
        cols = 15;
    } else if (choice == 4) {
        rows = 20;
        cols = 20;
    } else if (choice == 5) {
        rows = 25;
        cols = 25;
    } else {
        cout << "Please enter a valid number between 0-5";
        return 0;
    }

    // Create maze with respective column and rows
    Maze maze(rows, cols);

    // Part 2- Gets randomly generated Maze
    maze.generateMaze();
    // Display the maze without path
    // maze.displayMaze(maze);

    // Gets the shortest path
    maze.getShortestPath();

    // Display the maze with the shortest path(x) from top left corner to a right bottom corner
    maze.displayMaze(maze);


    return 0;
}
