// expmain.cpp
//
// ICS 46 Spring 2020
// Project #1: Dark at the End of the Tunnel
//
// Do whatever you'd like here.  This is intended to allow you to experiment
// with the given classes in the darkmaze library, or with your own
// algorithm implementations, outside of the context of the GUI or
// Google Test.
#include <memory>
#include "Maze.hpp"
#include "MazeFactory.hpp"
#include "swagSolver.hpp"
#include "MazeSolution.hpp"
#include "swagGenerator.hpp"
#include "MazeSolutionFactory.hpp"
int main()
{
    // Create a 10x10 maze
    std::unique_ptr<Maze> maze = MazeFactory{}.createMaze(10, 10);

    swagGenerator generator;
    generator.generateMaze(*maze);
    swagSolver solver;
    std::unique_ptr<MazeSolution> mazeSolution = MazeSolutionFactory{}.createMazeSolution(*maze); 

    solver.solveMaze(*maze,*mazeSolution);
    return 0;
}

