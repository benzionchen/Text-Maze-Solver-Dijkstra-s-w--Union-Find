#include "MazeSolver.h"
#include <iostream>

using namespace std;

int main()
{
    string inputFile = "maze.txt";
    string outputFile = "solved_maze.txt";

    vector<vector<char>> maze = readMaze(inputFile);

    pair<int, int> start = {-1, -1}, end = {-1, -1};
    for (int i = 0; i < maze.size(); ++i)
    {
        for (int j = 0; j < maze[i].size(); ++j)
        {
            if (maze[i][j] == 'S')
                start = {i, j};
            if (maze[i][j] == 'E')
                end = {i, j};
        }
    }

    if (start == make_pair(-1, -1) || end == make_pair(-1, -1))
    {
        cerr << "Error: Maze must contain start ('S') and end ('E') points." << endl;
        return 1;
    }

    if (solveMazeWithUnionFindAndDijkstra(maze, start, end))
    {
        cout << "Solved Maze:" << endl;
        displayMaze(maze);
        writeMazeToFile(outputFile, maze);
    }
    else
    {
        cerr << "Maze is unsolvable." << endl;
    }

    return 0;
}
