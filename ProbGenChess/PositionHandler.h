#pragma once
#include <vector>

class Position
{
public:
    std::vector<std::vector<int>> board;
    int BROWS;
    int BCOLS;
    bool whiteToMove;
    double cachedResult;
    int winner;
    // Allowed movement vectors
    int NX[8] = { -2, -2, -1, -1, 1, 1, 2, 2 };
    int NY[8] = { 1, -1, 2, -2, 2, -2, 1, -1 };
    int BX[4][7] = {
    {1,2,3,4,5,6,7},
    {1,2,3,4,5,6,7},
    {-1,-2,-3,-4,-5,-6,-7},
    {-1,-2,-3,-4,-5,-6,-7} };
    int BY[4][7] = {
    {1,2,3,4,5,6,7},
    {-1,-2,-3,-4,-5,-6,-7},
    {1,2,3,4,5,6,7},
    {-1,-2,-3,-4,-5,-6,-7} };
    int RX[4][7] = {
    {0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0},
    {1,2,3,4,5,6,7},
    {-1,-2,-3,-4,-5,-6,-7} };
    int RY[4][7] = {
    {1,2,3,4,5,6,7},
    {-1,-2,-3,-4,-5,-6,-7},
    {0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0} };
    int KX[8] = { 1,1,1,0,0,-1,-1,-1 };
    int KY[8] = { 1,0,-1,1,-1,1,0,-1 };

    Position(int BROWS, int BCOLS);
    void setStartingPosition();
    void cloneEssentialsFrom(Position* position);
    int countPieces();
    Position* mirror();
    void print();
    bool isValidPosition(int p);
    bool isWhitePiece(int pval);
    bool isBlackPiece(int pval);
    bool isInsideBoard(int x, int y);
    bool squaresContainSameColoredPieces(int x, int y, int x2, int y2);
    int checkWin(int x, int y);
    std::vector<Position*> getNextPositions();
};