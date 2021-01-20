#ifndef SWAGSOLVER_HPP
#define SWAGSOLVER_HPP
#include "MazeSolver.hpp"
#include "Maze.hpp"
class swagSolver : public MazeSolver
{
    public:
        void solveMaze(const Maze& maze, MazeSolution& mazeSolution) override;
    private:
        bool** maze_cell;
};


#endif

