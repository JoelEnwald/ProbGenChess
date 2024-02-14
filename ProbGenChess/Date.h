#ifndef DATE_H
#define DATE_H
#include <array>

class Date
{
private:
    int m_year{};
    int m_month{};
    int m_day{};
    std::array<std::array<double, 6>, 6> PawnBoostTable;
    std::array<std::array<double, 6>, 6> KingBoostTable;

public:
    Date(int year, int month, int day);

    void print() const;

    int getYear() const { return m_year; }
    int getMonth() const { return m_month; }
    int getDay() const { return m_day; }
};

#endif