#include "MazeSolver.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <limits>
#include <set>

using namespace std;

// Directions for moving in the grid: up, down, left, right
const vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

// Union-Find implementation
// I discovered this data structure in my 3104 class as a part of the Kruskal lecture timestamped @ 5:22
// https://youtu.be/N8K2quW4Fjs?si=PeT9CR1Qbl2avkzP&t=322
UnionFind::UnionFind(int n)
{
    parent.resize(n);           // Create array with size n where each element stores its parent
    rank.resize(n, 0);          // Initialize the rank (tree depth) of each element to 0
    for (int i = 0; i < n; ++i) // Sets each element as its own parent initially (self-loop), each element is in its own set
    {
        parent[i] = i; // Set each element to be its own parent, each element starts in its own set basically
    }
}

int UnionFind::find(int x)
{
    if (parent[x] != x) // If x is not its own parent, recursively find its parents' root
    {
        parent[x] = find(parent[x]); // Compress path by directly linking x to its root
    }
    return parent[x]; // Parent[x] is x's root
}

void UnionFind::unite(int x, int y) // Merges the set x and y unioning by rank
{
    int rootX = find(x); // First, find roots of x and y
    int rootY = find(y);
    if (rootX != rootY) // Only merge if x and y are in different sets & then compare ranks of x's root and y's root
    {
        if (rank[rootX] > rank[rootY]) // If x's root is deeper, make y's root a child of rootX
        {
            parent[rootY] = rootX;
        }
        else if (rank[rootX] < rank[rootY]) // Vice Versa
        {
            parent[rootX] = rootY;
        }
        else
        {
            parent[rootY] = rootX; // If both trees have same depth, just make rootY a child of rootX and increment rootX's rank
            rank[rootX]++;
        }
    }
}

bool UnionFind::isConnected(int x, int y) // If x and y are in the same set
{
    return find(x) == find(y);
}

// Function to read the maze from a file
vector<vector<char>> readMaze(const string &filename)
{
    vector<vector<char>> maze;
    ifstream file(filename); // Open file, for each line, extract the character inside each set of brackets
    if (!file)
    {
        cerr << "Error: Cannot open file " << filename << endl; // Error message if file is not openable
        exit(1);
    }

    string line;
    while (getline(file, line)) // Read each line of the file into line
    {
        vector<char> row;
        for (size_t i = 0; i < line.size(); i += 3) // Iterate over the line
        {                                           // Skip over brackets and spaces
            char cell = line[i + 1];                // Extract the character inside [ ], every cell is 3 chars wide, extract char inside brackets (S,_, x, or E)
            row.push_back(cell);                    // Add character to current row
        }
        maze.push_back(row); // Add each row to the maze grid
    }
    file.close(); // Close file + Return maze
    return maze;
}

// Function to display the maze and print it in the Terminal (make-shift UI)
void displayMaze(const vector<vector<char>> &maze)
{
    for (const auto &row : maze)
    {
        for (char cell : row)
        {
            cout << "[" << cell << "]";
        }
        cout << endl;
    }
}

// Function to write the maze to a file with the idea to show whether the maze can or cannot be solved, and write it to a file
// following the bracket format ("[ ]")
void writeMazeToFile(const string &filename, const vector<vector<char>> &maze)
{
    ofstream file(filename);
    if (!file)
    {
        cerr << "Error: Cannot write to file " << filename << endl;
        exit(1);
    }

    for (const auto &row : maze)
    {
        for (char cell : row)
        {
            file << "[" << cell << "]";
        }
        file << endl;
    }
    file.close();
}

// Preprocess the maze with Union-Find data structure by connecting traversable cells
// Need to map 2D coordinates to 1D indices
void preprocessMazeWithUnionFind(const vector<vector<char>> &maze, UnionFind &uf)
{
    int rows = maze.size();
    int cols = maze[0].size();
    auto getIndex = [&](int x, int y)
    { return x * cols + y; }; // Map 2D coordinates to 1D index

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if (maze[i][j] != 'x') // Only process cells that aren't obstacles, a.k.a. only traversable cells
            {
                for (const auto &[dx, dy] : directions)
                {
                    int nx = i + dx, ny = j + dy;
                    if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && maze[nx][ny] != 'x')
                    {
                        uf.unite(getIndex(i, j), getIndex(nx, ny)); // For each cell that is traversable, connect it to its valid neighbors using "unite"
                    }
                }
            }
        }
    }
}

// Solve maze with Union-Find and Dijkstra
// Check if S and E are in the same component (if not, maze = unsolvable)
bool solveMazeWithUnionFindAndDijkstra(vector<vector<char>> &maze, const pair<int, int> &start, const pair<int, int> &end)
{
    int rows = maze.size();
    int cols = maze[0].size();
    UnionFind uf(rows * cols);

    preprocessMazeWithUnionFind(maze, uf);

    auto getIndex = [&](int x, int y)
    { return x * cols + y; };

    if (!uf.isConnected(getIndex(start.first, start.second), getIndex(end.first, end.second)))
    {
        cerr << "No path exists between S and E due to being disconnected." << endl;
        return false;
    }

    // Proceed with Dijkstra's algorithm
    vector<vector<int>> distance(rows, vector<int>(cols, numeric_limits<int>::max()));
    vector<vector<pair<int, int>>> parent(rows, vector<pair<int, int>>(cols, {-1, -1}));
    set<pair<int, pair<int, int>>> pq; // {distance, {x, y}}

    distance[start.first][start.second] = 0;
    pq.insert({0, start});

    while (!pq.empty())
    {
        auto [dist, position] = *pq.begin();
        pq.erase(pq.begin());

        int x = position.first;
        int y = position.second;

        if (position == end)
        {
            // Reconstruct the path
            pair<int, int> current = end;
            while (current != start)
            {
                auto [px, py] = parent[current.first][current.second];
                if (current != end)
                {
                    maze[current.first][current.second] = '.';
                }
                current = {px, py};
            }
            return true;
        }

        for (const auto &[dx, dy] : directions)
        {
            int nx = x + dx;
            int ny = y + dy;

            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && maze[nx][ny] != 'x')
            {
                int newDist = dist + 1;
                if (newDist < distance[nx][ny])
                {
                    pq.erase({distance[nx][ny], {nx, ny}});
                    distance[nx][ny] = newDist;
                    parent[nx][ny] = {x, y};
                    pq.insert({newDist, {nx, ny}});
                }
            }
        }
    }

    cerr << "No path found between S and E despite being connected." << endl;
    return false; // No path found
}
