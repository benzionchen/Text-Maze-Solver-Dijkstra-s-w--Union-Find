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
UnionFind::UnionFind(int n)
{
    parent.resize(n);
    rank.resize(n, 0);
    for (int i = 0; i < n; ++i)
    {
        parent[i] = i;
    }
}

int UnionFind::find(int x)
{
    if (parent[x] != x)
    {
        parent[x] = find(parent[x]);
    }
    return parent[x];
}

void UnionFind::unite(int x, int y)
{
    int rootX = find(x);
    int rootY = find(y);
    if (rootX != rootY)
    {
        if (rank[rootX] > rank[rootY])
        {
            parent[rootY] = rootX;
        }
        else if (rank[rootX] < rank[rootY])
        {
            parent[rootX] = rootY;
        }
        else
        {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
    }
}

bool UnionFind::isConnected(int x, int y)
{
    return find(x) == find(y);
}

// Function to read the maze from a file
vector<vector<char>> readMaze(const string &filename)
{
    vector<vector<char>> maze;
    ifstream file(filename);
    if (!file)
    {
        cerr << "Error: Could not open file " << filename << endl;
        exit(1);
    }

    string line;
    while (getline(file, line))
    {
        vector<char> row;
        for (size_t i = 0; i < line.size(); i += 3)
        {                            // Skip over brackets and spaces
            char cell = line[i + 1]; // Extract the character inside [ ]
            row.push_back(cell);
        }
        maze.push_back(row);
    }
    file.close();
    return maze;
}

// Function to display the maze
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

// Function to write the maze to a file
void writeMazeToFile(const string &filename, const vector<vector<char>> &maze)
{
    ofstream file(filename);
    if (!file)
    {
        cerr << "Error: Could not write to file " << filename << endl;
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

// Preprocess the maze with Union-Find
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
            if (maze[i][j] != 'x')
            { // Only consider traversable cells
                for (const auto &[dx, dy] : directions)
                {
                    int nx = i + dx, ny = j + dy;
                    if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && maze[nx][ny] != 'x')
                    {
                        uf.unite(getIndex(i, j), getIndex(nx, ny));
                    }
                }
            }
        }
    }
}

// Solve maze with Union-Find and Dijkstra
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
        cerr << "No path exists between S and E due to disconnected components." << endl;
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
