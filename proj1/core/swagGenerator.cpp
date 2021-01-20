#include <ics46/factory/DynamicFactory.hpp>
#include "swagGenerator.hpp"
#include "Maze.hpp"
#include "Direction.hpp"
#include <vector>
#include <random>
#include <iostream>
ICS46_DYNAMIC_FACTORY_REGISTER(MazeGenerator, swagGenerator, "It's swagGenerator");
using namespace std;

vector<Direction> availableWalls(int cx, int cy, Maze &maze,bool** maze_cell) {
    vector<Direction> result;
    if( (cx+1)<maze.getWidth() && maze_cell[cx+1][cy]!= true) {
        result.push_back(Direction::right);
    }
    if((cx -1) >=0 && maze_cell[cx-1][cy]!= true) {
        result.push_back(Direction::left);
    }
    if((cy-1) >=0  && maze_cell[cx][cy-1]!= true) {
        result.push_back(Direction::up);
    }

    if((cy+1)<maze.getHeight() && maze_cell[cx][cy+1]!= true) {
        result.push_back(Direction::down);
    }
    return result;
}
Direction chooseWall(vector<Direction> &adjacentWalls) {
    int size = adjacentWalls.size();
    std::random_device device;
    std::default_random_engine engine{device()};
    std::uniform_int_distribution<int> distribution{0, size-1};
    return adjacentWalls[distribution(engine)];
}

bool isNextMoveValid(int cx, int cy, Maze& maze, bool** maze_cell){
    vector<Direction> adjacentWalls = availableWalls(cx, cy, maze, maze_cell);
    if((adjacentWalls.size()>0))
    {
        return true;
    }else
    {
        return false;
    }
}
void destory_wall(int cx, int cy, Maze &maze, bool** maze_cell) {
    vector<Direction> availabeMove = availableWalls(cx, cy, maze, maze_cell);
    while(availabeMove.size()>0)
    {
        Direction move = chooseWall(availabeMove);
        if(move == Direction::right)
        {
            maze.removeWall(cx,cy, move);
            maze_cell[cx+1][cy]=true;
            if(isNextMoveValid(cx+1,cy,maze,maze_cell))
                destory_wall(cx+1,cy,maze,maze_cell);
            availabeMove.clear();// if next move not valid, back up one move
            availabeMove = availableWalls(cx,cy, maze, maze_cell);
        }
        if(move == Direction::left)
        {
            maze.removeWall(cx,cy, move);
            maze_cell[cx-1][cy]=true;
             if(isNextMoveValid(cx-1,cy,maze,maze_cell))
                destory_wall(cx-1,cy,maze,maze_cell);
            availabeMove.clear();
            availabeMove = availableWalls(cx,cy, maze, maze_cell);
        }
        if(move == Direction::down)
        {
            maze.removeWall(cx,cy, move);
            maze_cell[cx][cy+1]=true;
             if(isNextMoveValid(cx,cy+1,maze,maze_cell))
                destory_wall(cx,cy+1,maze,maze_cell);
            availabeMove.clear();
            availabeMove = availableWalls(cx,cy, maze, maze_cell);
        }
        if(move == Direction::up)
        {
            maze.removeWall(cx,cy, move);
            maze_cell[cx][cy-1]=true;
             if(isNextMoveValid(cx,cy-1,maze,maze_cell))
                destory_wall(cx,cy-1,maze,maze_cell);
             availabeMove.clear();
             availabeMove = availableWalls(cx,cy, maze, maze_cell);
        }
     }
}
void swagGenerator::generateMaze(Maze& maze)
{
    maze.addAllWalls();
 
    // get the size of maze
    int height = maze.getHeight();
    int width = maze.getWidth();
    // deaclare a dynamic 2d array with bool type
    maze_cell = new bool*[height];
    for(int i=0; i<height;i++){
        maze_cell[i] = new bool[width];
    }
    // initail 2d array, let all element be false,Since we didn't check
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            maze_cell[i][j]=false;
        }
    }
    //initial(0,0) cell as visited
    maze_cell[0][0] = true;
    // start digging with (0,0)
    destory_wall(0,0,maze,maze_cell);
}
