# include <iostream>
#include "CS4412Pj5Weir.h"
#include <random>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <Windows.h> // For changing text colors in the Windows console
#include <algorithm>
#include <stack>

using namespace std;

/**
 * Author: Victoria Weir
 * Class: CS4412
 * Date: 10-19-23
 * Details: Project 5 - Maze and Graphs
 */

// Cell class to represent each cell in the maze
class Cell {
public:
    bool topWall;
    bool rightWall;
    bool bottomWall;
    bool leftWall;
    bool visited;
    bool path;

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

// Maze class that should create and work with the maze
class Maze {
public:
    int rows;
    int cols;
    vector<vector<Cell>> cells;

    Maze(int numRows, int numCols) : rows(numRows), cols(numCols) {
        // Initialize the maze with cells
        cells.resize(rows, vector<Cell>(cols));
    }
    // Generate the maze using a randomized depth-first search algorithm
    void generateMaze() {
        // Create a path from the top-left corner (start) to the bottom-right corner (end)
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                if (row == 0) {
                    cells[row][col].topWall = false;
                    if (col > 0) cells[row][col - 1].rightWall = false;
                }
                if (col == 0) {
                    cells[row][col].leftWall = false;
                    if (row > 0) cells[row - 1][col].bottomWall = false;
                }
            }
        }

        srand(time(NULL)); // Initialize random number generator
        visitCell(rows - 1, cols - 1); // Start the generation process from the bottom-right cell
    }

    void visitCell(int row, int col) {
        cells[row][col].visited = true;

        // Define the order in which you want to check neighboring cells
        int directions[] = {1, 2, 3, 4};
        std::shuffle(directions, directions + 4, std::mt19937(std::random_device()()));

        for (int i = 0; i < 4; i++) {
            if (directions[i] == 1 && row > 0 && !cells[row - 1][col].visited) {
                // Knock down the top wall
                cells[row][col].topWall = false;
                cells[row - 1][col].bottomWall = false;
                visitCell(row - 1, col);
            } else if (directions[i] == 2 && col < cols - 1 && !cells[row][col + 1].visited) {
                // Knock down the right wall
                cells[row][col].rightWall = false;
                cells[row][col + 1].leftWall = false;
                visitCell(row, col + 1);
            } else if (directions[i] == 3 && row < rows - 1 && !cells[row + 1][col].visited) {
                // Knock down the bottom wall
                cells[row][col].bottomWall = false;
                cells[row + 1][col].topWall = false;
                visitCell(row + 1, col);
            } else if (directions[i] == 4 && col > 0 && !cells[row][col - 1].visited) {
                // Knock down the left wall
                cells[row][col].leftWall = false;
                cells[row][col - 1].rightWall = false;
                visitCell(row, col - 1);
            }
        }
    }
// Function to display the maze with the path marked with asterisks
    void displayMaze(const Maze& maze) {
        // Set console text colors (specific to Windows console)
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        for (int row = 0; row < maze.rows; row++) {
            for (int col = 0; col < maze.cols; col++) {
                if (maze.cells[row][col].topWall)
                    cout << "+---";
                else
                    cout << "+   ";
            }
            cout << "+" << endl;
            for (int col = 0; col < maze.cols; col++) {
                if (maze.cells[row][col].leftWall)
                    cout << "|";
                else
                    cout << " ";
                if (maze.cells[row][col].path) {
                    SetConsoleTextAttribute(hConsole, 12); // Set text color to red
                    cout << " x ";
                    SetConsoleTextAttribute(hConsole, 15); // Set text color back to white
                } else {
                    cout << "   ";
                }
            }
            if (maze.cells[row][maze.cols - 1].rightWall)
                cout << "|";
            else
                cout << " ";
            cout << endl;
        }
        for (int col = 0; col < maze.cols; col++) {
            cout << "+---";
        }
        cout << "+" << endl;
    }

};





int main() {
    int rows = 0;
    int cols = 0;
    int choice = 0;

    // Get size of maze the user wants
    cout << "Choose between: \n"
            "0. Exit \n"
            "1. 5x5 \n"
            "2. 10x10 \n"
            "3. 15x15 \n"
            "4. 20x20 \n"
            "5. 25x25 \n";
    cout << "Please enter the integer(0-5) to choose: ";
    cin >> choice;
    if(choice == 0){
        cout << "Ending the Maze";
        return 0;
    }
    else if(choice== 1){
        rows = 5;
        cols = 5;
    }
    else if(choice== 2){
        rows = 10;
        cols = 10;
    }
    else if(choice== 3){
        rows = 15;
        cols = 15;
    }
    else if(choice== 4){
        rows = 20;
        cols = 20;
    }
    else if(choice==5){
        rows = 25;
        cols = 25;
    }
    else{
        cout<< "Please enter a valid number between 0-5";
        return 0;
    }

    // Create maze with respective column and rows
    Maze maze(rows, cols);

    // Part 1- Should just display the maze without any paths but have the shape
    //maze.displayMaze(maze);

    // Part 2- Gets randomly generated Maze and shortest path

    // Generate the maze
    maze.generateMaze();

    // Display the maze
    maze.displayMaze(maze);

    return 0;
}
