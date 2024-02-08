#pragma once
#include "PositionHandler.h"

class AdvancedEvaluator6
{
private:
	int color;

public:
	AdvancedEvaluator6(int color);
	double eval(Position* paramPosition, int color);
	void update(std::vector<std::vector<std::vector<int>>> board_states, int result);
	std::vector<std::vector<int>> rotate90(std::vector<std::vector<int>>& matrix);
};
