#pragma once
#include <vector>

class Position
{
public:
    Position(int BROWS = 6, int BCOLS = 6);
    void setStartingPosition();
    std::vector<Position*> getNextPositions();
    void print();
    std::vector<std::vector<int>> board;
    int BROWS;
    int BCOLS;
    bool whiteToMove;
    double cachedResult;
    int winner;
};