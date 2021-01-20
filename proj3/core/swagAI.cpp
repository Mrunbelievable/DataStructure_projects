// MyOthelloAI.cpp
#include <ics46/factory/DynamicFactory.hpp>
#include "swagAI.hpp"


ICS46_DYNAMIC_FACTORY_REGISTER(OthelloAI, zhiched::swagAI, "zhiched(Required)");


// finding the avaliable validMoves for the game tree
std::vector<std::pair<int,int>> zhiched::swagAI::validMoves(const OthelloGameState& state)
{
    std::vector<std::pair<int,int>> validMoves;
    for(unsigned int i=0; i<state.board().width(); i++)
    {
        for(unsigned int j=0; j<state.board().height(); j++)
        {
            if(state.isValidMove(i,j))
            {
                validMoves.push_back(std::pair<int,int>(i,j));
            }
        }
    }
    return validMoves;
}
//evaluate the score
int zhiched::swagAI::evaluation(OthelloGameState& state, OthelloCell myColor)
{
    int score = 0;
    if(myColor==OthelloCell::white)
    {
        score = state.whiteScore()-state.blackScore();
    }else
    {
        score = state.blackScore()-state.whiteScore();
    }
    return score;
}
int zhiched::swagAI::search(OthelloGameState& state, int depth, OthelloCell myColor)
{
    OthelloCell currentColor;
    if(state.isBlackTurn())
        currentColor = OthelloCell::black;
    else 
        currentColor = OthelloCell::white;

    if(depth==0)
    {
        return evaluation(state, currentColor);
    }else
    {
        if(myColor==currentColor)
        {
            int max = -10000;
            std::vector<std::pair<int,int>> avaliableMoves = validMoves(state);
            for(unsigned int i=0;i<avaliableMoves.size();i++)
            {
                int x = avaliableMoves[i].first;
                int y = avaliableMoves[i].second;
                std::unique_ptr<OthelloGameState> new_state = state.clone();
                new_state->makeMove(x,y);
                int eval = search(*new_state,depth-1,myColor);
                if(eval > max)
                {
                    max = eval;
                }
            }
            return max;
        }else
        {
            int min = 10000;
            std::vector<std::pair<int,int>> avaliableMoves=validMoves(state);
            for(unsigned int i=0;i<avaliableMoves.size();i++)
            {
                int x = avaliableMoves[i].first;
                int y = avaliableMoves[i].second;
                std::unique_ptr<OthelloGameState> new_state = state.clone();
                new_state->makeMove(x, y);
                int eval = search(*new_state,depth-1,myColor);
                if(eval < min)
                 {
                    min = eval;
                }
            }
            return min;
        }

    }
}
std::pair<int, int> zhiched::swagAI::chooseMove(const OthelloGameState& state)
{   // implementation of the AI goes here
    OthelloCell myColor;
    if(state.isWhiteTurn())
    {
        myColor = OthelloCell::white;
    }
    else
        myColor = OthelloCell::black;
    std::vector<std::pair<int,int>> avaliableMoves = validMoves(state);
    std::pair<int,int> bestMove;
    int maxValue = -99999;
    for(unsigned int i=0;i<avaliableMoves.size();i++)
    {
        std::unique_ptr<OthelloGameState> new_state = state.clone();
        int x = avaliableMoves[i].first;
        int y = avaliableMoves[i].second;
        new_state->makeMove(x,y);
        int value = search(*new_state, 4, myColor);
        if(maxValue<value)
        {
            maxValue = value;
            bestMove = std::make_pair(x, y);
        }
    }

    return bestMove;

}


