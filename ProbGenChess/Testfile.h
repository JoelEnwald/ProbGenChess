#pragma once
#include <array>

class TestClass
{
private:
    int color;

    std::array<std::array<double, 6>, 6> PawnBoostTable = { {
    {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.3, 0.33, 0.4, 0.4, 0.33, 0.3},
    {0.05, 0.08, 0.25, 0.25, 0.08, 0.08},
    {0.0, 0.0, 0.2, 0.2, 0.0, 0.0},
    {0.05, 0.01, -0.1, -0.1, 0.01, 0.05},
    {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
    } };

    /*
    double PawnBoostTable[6][6];
    */

public:
    TestClass(int color);
};