#include "Evaluators.h"
#include "PositionHandler.h"
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <iostream>
#include <iomanip>

// Macros
#define DEPTH 3
#define EPOCHS 200
#define BROWS 6
#define BCOLS 6
#define N_PIECES 13
#define SHOWBOARD 1

class TrainingSession {
private:
    AdvEvaluator6 eval1;
    AdvEvaluator6 eval2;
public:
    AdvEvaluator6* eval_curr;
    TrainingSession() : eval_curr{NULL}, eval1{NULL}, eval2{NULL} {}

    double comparePositions(Position* pos, int player) {
        return evaluate(pos, player);
    }

    double evaluate(Position* pos, int player) {
        if (pos->cachedResult != NULL) {
            return pos->cachedResult;
        }
        double d = 0.0;
        d = eval_curr->eval(pos, player);
        pos->cachedResult = d;
        return d;
    }

    std::pair<double, int> alphabeta(Position* pos, int depth, double alpha, double beta, int player) {
        if (pos->winner == -1) {
            return std::make_pair(-1e10 - depth, 1);
        }
        if (pos->winner == 1) {
            return std::make_pair(1e10 + depth, 1);
        }
        if (depth == 0) {
            return std::make_pair(eval_curr->eval(pos, player), 1);
        }

        std::vector<Position*> P = pos->getNextPositions();
        std::sort(P.begin(), P.end(), [this, &player](const auto& a, const auto& b){
            return this->evaluate(a, player) < this->evaluate(b, player);
            });

        int n_explored = 0;
        if (player == 0) {
            std::reverse(P.begin(), P.end());
            for (int i = 0; i < P.size(); i++) {
                std::pair<double, int> result = alphabeta(P[i], depth - 1, alpha, beta, 1);
                n_explored += result.second;
                alpha = std::max(alpha, result.first);
                if (beta <= alpha) {
                    break;
                }
            }
            return std::make_pair(alpha, n_explored);
        }

        for (int i = 0; i < P.size(); i++) {
            std::pair<double, int> result = alphabeta(P[i], depth - 1, alpha, beta, 0);
            n_explored += result.second;
            beta = std::min(beta, result.first);
            if (beta <= alpha) {
                break;
            }
        }
        return std::make_pair(beta, n_explored);
    }

    double minmax(Position* pos, int depth, int player) {
        double alpha = -2e9;
        double beta = 2e9;
        std::pair<double, int> result = alphabeta(pos, depth, alpha, beta, player);
        return result.first;
    }

    int countPieces(std::vector<std::vector<int>> board) {
        int size = board.size();
        int count = 0;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (board[i][j] != 0) {
                    count++;
                }
            }
        }
        return count;
    }

    void printMatrix(std::vector<std::vector<char>> matrixToPrint) {
        for (int i = 0; i < BROWS; i++) {
            for (char c : matrixToPrint[i]) {
                std::cout << c;
            }
            std::cout << "\n";
        }
    }

    std::pair<std::vector<std::vector<std::vector<std::vector<int>>>>, std::vector<std::vector<std::vector<std::vector<int>>>>> startSession() {
        std::vector<std::vector<std::vector<std::vector<int>>>> gamesdata;
        std::vector<std::vector<std::vector<std::vector<int>>>> wingamesdata;

        int gamecounter = 1;
        std::vector<std::vector<char>> matrixToPrint;

        int eval1_win_counter = 0;
        int eval2_win_counter = 0;

        std::vector<double> winratios1to2;

        Position p(BROWS, BCOLS);
        p.setStartingPosition();
        // eval1 gets white pieces, eval2 black ones
        AdvEvaluator6 eval1(0);
        AdvEvaluator6 eval2(1);

        // Loop for games
        while (gamecounter <= EPOCHS) {
            if ((gamecounter % 50) == 1) {
                std::cout << "Wins and losses are " << eval1_win_counter << " to " << eval2_win_counter << "\n";
            }

            // All board states reached in a game, as a list
            std::vector<std::vector<std::vector<int>>> gameboard_states;

            std::cout << "Game number " << gamecounter << "\n";

            // Set the starting position for the game
            Position p(BROWS, BCOLS);
            p.setStartingPosition();
            // Print the starting position
            if (SHOWBOARD) {
                p.print();
                std::cout << "\n";
            }
            // Add initial board to board states
            gameboard_states.push_back(p.board);

            int ms = std::time(0) * 1000;

            int turnCounter = 0;
            // Loop for turns
            while (turnCounter < 100) {
                std::vector<Position*> nextPositions = p.getNextPositions();

                if (p.winner == 1) {
                    std::cout << "White won.\n";
                    eval1.update(gameboard_states, 1);
                    eval2.update(gameboard_states, -1);
                    eval1_win_counter += 1;
                    break;
                }
                if (p.winner == -1) {
                    std::cout << "Black won.\n";
                    eval1.update(gameboard_states, -1);
                    eval2.update(gameboard_states, 1);
                    eval2_win_counter += 1;
                    break;
                }
                if (turnCounter == 99) {
                    std::cout << "It was a draw.\n";
                    eval1.update(gameboard_states, 0);
                    eval2.update(gameboard_states, 0);
                    break;
                }
                if (nextPositions.empty()) {
                    std::cout << "No more available moves.\n";
                    break;
                }
                // If the game has not ended, find next move
                else {
                    double cutoff = std::max(0.1, (100 - gamecounter) / 100.0);
                    // Pick a random position, with decreasing probability as games are played
                    if ((std::rand() % 100) / 100.0 <= 0.05) {
                        p = *nextPositions[std::rand() % nextPositions.size()];
                    }
                    // Choose the best position
                    else {
                        Position bestPosition(BROWS, BCOLS);
                        if (p.whiteToMove) {
                            eval_curr = &eval1;
                            double maxVal = -1e9;
                            for (int i = 0; i < nextPositions.size(); i++) {
                                double val = minmax(nextPositions[i], DEPTH, 0);
                                // White tries to maximise
                                if (maxVal < val) {
                                    bestPosition = *nextPositions[i];
                                    maxVal = val;
                                }
                            }
                        }
                        else {
                            eval_curr = &eval2;
                            double minVal = 1e9;
                            for (int i = 0; i < nextPositions.size(); i++) {
                                double val = minmax(nextPositions[i], DEPTH, 0);
                                // Black tries to minimise
                                if (minVal > val) {
                                    bestPosition = *nextPositions[i];
                                    minVal = val;
                                }
                            }
                        }
                    }
                }
                turnCounter++;
            }
            gamecounter++;
        }
        return std::make_pair(gamesdata, wingamesdata);
    }
};

void analysegames(std::vector<std::vector<std::vector<std::vector<int>>>> games, std::vector<std::vector<std::vector<std::vector<int>>>> wingames) {
    std::vector<double> piececounts(13, 0.0);
    std::vector<double> piececounts_wins(13, 0.0);

    for (int i = 0; i < games.size(); i++) {
        std::vector<double> piececounts_game(13, 0.0);
        for (int j = 0; j < games[i].size(); j++) {
            for (int k = 0; k < BROWS; k++) {
                for (int l = 0; l < BCOLS; l++) {
                    int piece = games[i][j][k][l];
                    piececounts_game[piece] += 1;
                }
            }
        }
        for (int m = 0; m < piececounts_game.size(); m++) {
            piececounts_game[m] /= games[i].size();
        }
        for (int n = 0; n < piececounts.size(); n++) {
            piececounts[n] += piececounts_game[n];
        }
    }
    for (int o = 0; o < piececounts.size(); o++) {
        piececounts[o] /= games.size();
    }

    for (int i = 0; i < wingames.size(); i++) {
        std::vector<double> piececounts_wingame(13, 0.0);
        for (int j = 0; j < wingames[i].size(); j++) {
            for (int k = 0; k < BROWS; k++) {
                for (int l = 0; l < BCOLS; l++) {
                    int piece = wingames[i][j][k][l];
                    piececounts_wingame[piece] += 1;
                }
            }
        }
        for (int m = 0; m < piececounts_wingame.size(); m++) {
            piececounts_wingame[m] /= wingames[i].size();
        }
        for (int n = 0; n < piececounts_wins.size(); n++) {
            piececounts_wins[n] += piececounts_wingame[n];
        }
    }
    for (int o = 0; o < piececounts_wins.size(); o++) {
        piececounts_wins[o] /= wingames.size();
    }

    int hallo = 5;
}

int main() {
    TrainingSession session;
    std::pair<std::vector<std::vector<std::vector<std::vector<int>>>>, std::vector<std::vector<std::vector<std::vector<int>>>>> result = session.startSession();
    std::vector<std::vector<std::vector<std::vector<int>>>> gamesdata = result.first;
    std::vector<std::vector<std::vector<std::vector<int>>>> wingamesdata = result.second;
    analysegames(gamesdata, wingamesdata);
    int hallo = 5;
}