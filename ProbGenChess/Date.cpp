#include "Date.h"
#include <iostream>

Date::Date(int year, int month, int day) // constructor definition
    : m_year{ year }
    , m_month{ month }
    , m_day{ day }
{
    PawnBoostTable = { {
    {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.3, 0.33, 0.4, 0.4, 0.33, 0.3},
    {0.05, 0.08, 0.25, 0.25, 0.08, 0.08},
    {0.0, 0.0, 0.2, 0.2, 0.0, 0.0},
    {0.05, 0.01, -0.1, -0.1, 0.01, 0.05},
    {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
    } };
    KingBoostTable = { {
    {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.3, 0.33, 0.4, 0.4, 0.33, 0.3},
    {0.05, 0.08, 0.25, 0.25, 0.08, 0.08},
    {0.0, 0.0, 0.2, 0.2, 0.0, 0.0},
    {0.05, 0.01, -0.1, -0.1, 0.01, 0.05},
    {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
    } };
}

void Date::print() const // print function definition
{
    std::cout << "Date(" << m_year << ", " << m_month << ", " << m_day << ")\n";
};