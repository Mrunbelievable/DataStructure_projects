#ifndef SWAGGENERATOR_HPP
#define SWAGGENERATOR_HPP
#include "Maze.hpp"
#include "MazeGenerator.hpp"

class swagGenerator : public MazeGenerator
{
    public:
        void generateMaze(Maze& maze) override;
    private:
    bool** maze_cell;
};


#endif

