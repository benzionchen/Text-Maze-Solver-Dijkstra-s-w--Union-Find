#ifndef MAZESOLVER_H
#define MAZESOLVER_H

#include <vector>
#include <string>
#include <utility>

// Union-Find class declaration
class UnionFind
{
public:
    UnionFind(int n);
    int find(int x);
    void unite(int x, int y);
    bool isConnected(int x, int y);

private:
    std::vector<int> parent, rank;
};

// Function declarations
std::vector<std::vector<char>> readMaze(const std::string &filename);
void displayMaze(const std::vector<std::vector<char>> &maze);
void writeMazeToFile(const std::string &filename, const std::vector<std::vector<char>> &maze);
void preprocessMazeWithUnionFind(const std::vector<std::vector<char>> &maze, UnionFind &uf);
bool solveMazeWithUnionFindAndDijkstra(std::vector<std::vector<char>> &maze, const std::pair<int, int> &start, const std::pair<int, int> &end);

#endif // MAZESOLVER_H
