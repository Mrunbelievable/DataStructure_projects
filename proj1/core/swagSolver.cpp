#include <ics46/factory/DynamicFactory.hpp>
#include "swagSolver.hpp"
#include "Maze.hpp"
#include "Direction.hpp"
#include "MazeSolution.hpp"
#include <vector>
#include <random>
#include <iostream>
ICS46_DYNAMIC_FACTORY_REGISTER(MazeSolver, swagSolver, "SwagSolver");
using namespace std;

bool solutionComplete; // for checking if the solution found
Direction chooseDirection(const vector<Direction> &directions) {
    int size = directions.size();
    std::random_device device;
    std::default_random_engine engine{device()};
    std::uniform_int_distribution<int> distribution{0,size - 1};
    return directions[distribution(engine)];
}
vector<Direction> adjacentUnvisited(int cx, int cy, bool** maze_cell, const Maze& maze){
    vector<Direction> exitDirection;
    if(!maze.wallExists(cx, cy, Direction::right)
        && maze_cell[cx+1][cy]== false)
    {
        exitDirection.push_back(Direction::right);
    }
    if(!maze.wallExists(cx, cy, Direction::left)
        && maze_cell[cx-1][cy]== false)
    {
        exitDirection.push_back(Direction::left);
    }

    if(!maze.wallExists(cx, cy, Direction::up)
        && maze_cell[cx][cy-1]== false)
    {
        exitDirection.push_back(Direction::up);
    }
    if(!maze.wallExists(cx, cy, Direction::down)
        && maze_cell[cx][cy+1]== false) {
        exitDirection.push_back(Direction::down);
    }
    return exitDirection;
}
bool isNextMoveValid(int cx, int cy,bool** maze_cell,const Maze& maze,pair<int,int> endPoint)
{
    if(cx==endPoint.first && cy==endPoint.second)
        solutionComplete = true;
    vector <Direction> availableDirections = adjacentUnvisited(cx,cy,maze_cell, maze);
    if(availableDirections.size()>0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
void findPath(const Maze& maze, bool** maze_cell, pair<int,int> endPoint,
    int cx, int cy, MazeSolution& mazeSolution){
    vector <Direction> availableDirections = adjacentUnvisited(cx,cy,maze_cell, maze);
    
    while(availableDirections.size()>0)
    {
        int nx,ny;
        Direction next_move = chooseDirection(availableDirections);
        if(next_move == Direction::right)
        {
            nx = cx+1;
            ny = cy;
            maze_cell[nx][ny]=true;
            mazeSolution.extend(Direction::right);        
            if(isNextMoveValid(nx,ny,maze_cell,maze,endPoint))
                findPath(maze,maze_cell,endPoint,nx,ny,mazeSolution);
            if(solutionComplete)
                return;
            mazeSolution.backUp();
        }
        if(next_move == Direction::left)
        {
            nx = cx-1;
            ny = cy;
            maze_cell[nx][ny]=true;
            mazeSolution.extend(Direction::left);
            if(isNextMoveValid(nx,ny,maze_cell,maze,endPoint))
                findPath(maze,maze_cell,endPoint,nx,ny,mazeSolution);
            if(solutionComplete)
                return;
            mazeSolution.backUp();
        }
        if(next_move == Direction::down)
        {
            nx = cx;
            ny = cy+1;
            maze_cell[nx][ny]=true;
            mazeSolution.extend(Direction::down);
            if(isNextMoveValid(nx,ny,maze_cell,maze,endPoint))
                findPath(maze,maze_cell,endPoint,nx,ny,mazeSolution);
            if(solutionComplete)
                return;
            mazeSolution.backUp();
        }
        if(next_move == Direction::up)
        {
            nx = cx;
            ny = cy-1;
            maze_cell[nx][ny]=true;
            mazeSolution.extend(Direction::up);
            if(isNextMoveValid(nx,ny,maze_cell,maze,endPoint))
                findPath(maze,maze_cell,endPoint,nx,ny,mazeSolution);
            if(solutionComplete)
                return;
            mazeSolution.backUp();
        }
        availableDirections.clear();
        availableDirections = adjacentUnvisited(cx,cy,maze_cell, maze);

    }
}
void swagSolver::solveMaze(const Maze& maze, MazeSolution& mazeSolution)
{
    mazeSolution.restart();
    int width = maze.getWidth();
    int height = maze.getHeight();
    pair<int, int> endPoint = mazeSolution.getEndingCell();
    maze_cell = new bool*[height];
    // declear dynamic 2d array with type bool
    for(int i=0; i<height;i++){
        maze_cell[i] = new bool[width];
    }
    // initail 2d array, let all element be false,Since we didn't check
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            maze_cell[i][j]=false;
        }
    }
    maze_cell[0][0] = true;
    solutionComplete = false;
    findPath(maze, maze_cell, endPoint, 0,0, mazeSolution);
}
