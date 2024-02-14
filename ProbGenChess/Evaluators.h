#pragma once
#include "PositionHandler.h"
#include <array>

class AdvEvaluator6
{
private:
    int color;
    std::array<std::array<double, 6>, 6> PawnBoostTable;
    std::array<std::array<double, 6>, 6> KnightBoostTable;
    std::array<std::array<double, 6>, 6> BishopBoostTable;
    std::array<std::array<double, 6>, 6> RookBoostTable;
    std::array<std::array<double, 6>, 6> QueenBoostTable;
    std::array<std::array<double, 6>, 6> KingBoostTable;
    std::array<std::array<double, 6>, 6> KingBoostTableEndgame;
    
    /*
    double PawnBoostTable[6][6];
    double KnightBoostTable[6][6];
    double BishopBoostTable[6][6];
    double RookBoostTable[6][6];
    double QueenBoostTable[6][6];
    double KingBoostTable[6][6];
    double KingBoostTableEndgame[6][6];
    */

public:
    AdvEvaluator6(int color);
	double eval(Position* paramPosition, int color);
	void update(std::vector<std::vector<std::vector<int>>> board_states, int result);
	std::vector<std::vector<int>> rotate90(std::vector<std::vector<int>>& matrix);
};
