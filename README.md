Hello!

Welcome to my 2270 Final Project where I built a Maze Solver that can solve any size grid with as many obstacles ("`x`") as possible. The algorithm will traverse from Start "`S`" to End "`E`" 
using the Union-Find Data Structure and Dijkstra's algorithm. This is a text-based solver which means that if you change how the obstacles look, move around the "`S`" or "`E`", 
or even move around the obstacles, as long as it's in the proper format, the algorithm will be able to traverse appropriately from start to end. 

I originally wanted to do something related to maps and GPS but it looked like another classmate (Faisal) did it so I didn't want to seem like I was copying his idea. But I was still intrigued about 
Union-Find and how its runtime is the inverse Ackerman function (`O(α(n))`). I first came across this Data Structure in my 3104 Algorithms class, and learned that it saves computational bandwidth by 
avoiding the execution of Dijkstra's algorithm on a maze that has no theoretical solution. As the maze dynamically changes with new obstacles "`x`" being added and removed, Union-Find is well suited 
for these types of scenarios because it allows for efficient merging and querying of connected components. If this was a LARGE maze (1000s+ cells with many permutations) naive connectivity checks 
are computationally expensive. Preprocessing the maze is `O(nlog*n)` time where n is the number of cells, and connectivity checks between cells is close to constant time. I encourage the user to add 
1 thousand rows and columns with randomized obstacles and see how this algorithm handles it! 

To run the algorithm, make sure you're in the correct directory ("MazeSolver") and paste `g++ main.cpp MazeSolver.cpp -o MazeSolver` into the terminal. Then `./MazeSolver` to run it. 

MazeSolver.h is the header file that declares all of my declarations for classes and functions used. Here is where I declared Union-Find Class and its purpose is to check if two nodes
are in the same connected component. Some key things to take note of: `find(int x)` finds the root of the set containing `x`. `unite(int x, int y)` merges the sets containing `x` and `y`. 
`isConnected(int x, int y)` checks if x and y belong to the same set. I also need to read the maze, converting the file into a 2D vector, display the maze in the terminal, write an
unsolved or solved maze into a new file, combine both Union-Find and Dijkstra's algorithm to solve the maze. 

MazeSolver.cpp is the main implementation where Union-Find is used to track components + merge components & checks using path compression (optimizing `find`) and union by rank (optimizing `unite`). 
I had to do preprocessing with Union-Find where I connect adjacent traversable cells into components and I had to made sure these were not obstacles. For each cell in the maze, if it's
traversable, attempt to connect it to the adjacent traversable cells, and I can use helper function to map the 2D coordinates to unique 1D index for Union-Find. Then I used Dijkstra's
to find the shortest path using a priority queue `std::set` to store cells sorted by their distance from `S`. Then I initialize distances to all cells as infinity except `S` which is 0.
Process the cell with the smallest distance, and for each neighbor cell, update its distance if a shorter path is found and update its parent for reconstruction. I had to account for 
potentially unsolveable mazes, and if that's the case, the program terminates early.

Lastly, main.cpp was there to call the functions and tie it all together. It calls `solveMazeWithUnionFindAndDijkstra` where connectivity is checked + runs Dijkstra's Algorithm. If
the maze is solvable, it displays and writes the solved maze to `solved_maze.txt` and if "S" and "E" are disconnected, the program reports unsolvable. 

![unsolveable](https://github.com/user-attachments/assets/94eadc6e-eb63-4032-8bd1-7c76bede5bbb)

When there are changes, the maze will dynamically find a new solution unless it's impossible. Shown below: 
Adding new obstacles: ![maze will dynamically find new solution](https://github.com/user-attachments/assets/efff6b39-330f-4f5e-bfff-ce7fc087bb19)

Moving around "S" and "E": ![can solve it even if moving S and E around](https://github.com/user-attachments/assets/dcb3f665-99e1-456e-8594-6ad38c2d9230)

Even if we increase the grid size: ![even if we increase the grid size it should solve it](https://github.com/user-attachments/assets/ad73edd3-fa22-4977-9819-eaac0f02e39a)

One of the things I struggled with in this project was making sure that the algorithm was properly 1) parsing the maze and 2) returning a maze that is also properly parsed. This is
a two part problem where when the new maze is being drawn, that it's not adding any additional brackets or elements. ![improper maze parsing](https://github.com/user-attachments/assets/0f62a5c9-a009-4bd9-8abe-2d085f2be8d4)
This improper parsing of the maze was happening when the loop `for (char cell : line)` adds every character in the input line into the row vector. So that meant that the content of
the maze cells along with the brackets were getting redundantly parsed in the maze, causing format issues. 

Another issue I ran into was that my computer was not recognizing the proper compiler that I had installed. I installed MinGW, but for some reason, it kept recognizing the v6.3.0 of 
it rather than using the updated compiler to run my code. And because of that, I ran into issues with not being able to run certain types of features only available to C++ 11 or higher.
I finally got it to work by deleting all my PATHs in the virtual environment, reinstalling MinGW64 from several different websites, and through trial and error, I was able to get the v14.2.0 one
working. 

I also explored potentially using OpenCV for this project, but I couldn't get OpenCV to run properly on my computer and my program couldn't recognize the library so I eventually pivoted away from that idea.

This concludes my final project! 
