#include "OthelloAI.hpp"
#include <vector>

namespace zhiched
{
    class swagAI : public OthelloAI
    {
    public:
        std::pair<int, int> chooseMove(const OthelloGameState& state);
        int search(OthelloGameState& state, int depth, OthelloCell myColor);
        int evaluation(OthelloGameState& state, OthelloCell myColor);
        std::vector<std::pair<int,int>> validMoves(const OthelloGameState& state);
    };
}

