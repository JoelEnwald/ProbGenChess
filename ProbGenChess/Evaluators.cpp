#include "Evaluators.h"
#include "PositionHandler.h"
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <row.idl>

int MINWHITEPIECE = 1;
int MAXWHITEPIECE = 6;
int MINBLACKPIECE = 7;
int MAXBLACKPIECE = 12;
int N_PIECES = 13;
std::vector<std::string> PIECE_STRINGS = { "EMPTY", "WKING", "WQUEEN", "WROOK", "WBISHOP", "WKNIGHT", "WPAWN", "BKING", "BQUEEN", "BROOK", "BBISHOP", "BKNIGHT", "BPAWN" };


class Evaluator {
private:
    int color;

    void getEvalMat(int piece) {
        std::cout << "Getting the evaluation matrix is not supported yet. \n";
    }
};


class AdvancedEvaluator6 : private Evaluator{
public:
    AdvancedEvaluator6(int color)
        :color{ color } {
        // Initialize boost tables
        double PawnBoostTable[6][6] = {
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.3, 0.33, 0.4, 0.4, 0.33, 0.3},
        {0.05, 0.08, 0.25, 0.25, 0.08, 0.08},
        {0.0, 0.0, 0.2, 0.2, 0.0, 0.0},
        {0.05, 0.01, -0.1, -0.1, 0.01, 0.05},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}};
        double KnightBoostTable[6][6] = {
         {-0.5, -0.35, -0.3, -0.3, -0.35, -0.5},
         {-0.35, -0.03, 0.08, 0.08, -0.03, -0.35},
         {-0.3, 0.1, 0.2, 0.2, 0.1, -0.3},
         {-0.3, 0.08, 0.2, 0.2, 0.08, -0.3},
         {-0.35, -0.01, 0.08, 0.08, -0.01, -0.35},
         {-0.5, -0.35, -0.3, -0.3, -0.35, -0.5} };
        double BishopBoostTable[6][6] = {
         {-0.2, -0.1, -0.1, -0.1, -0.1, -0.2},
         {-0.1, 0.01, 0.05, 0.05, 0.01, -0.1},
         {-0.1, 0.05, 0.1, 0.1, 0.05, -0.1},
         {-0.1, 0.05, 0.1, 0.1, 0.05, -0.1},
         {-0.1, 0.06, 0.05, 0.05, 0.06, -0.1},
         {-0.2, -0.1, -0.1, -0.1, -0.1, -0.2} };
        double RookBoostTable[6][6] = {
         {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
         {0.02, 0.05, 0.05, 0.05, 0.05, 0.02},
         {-0.05, 0.0, 0.0, 0.0, 0.0, -0.05},
         {-0.05, 0.0, 0.0, 0.0, 0.0, -0.05},
         {-0.05, 0.0, 0.0, 0.0, 0.0, -0.05},
         {0.0, 0.0, 0.05, 0.05, 0.0, 0.0} };
        double QueenBoostTable[6][6] = {
        {-0.2, -0.1, -0.05, -0.05, -0.1, -0.2},
        {-0.1, 0.01, 0.03, 0.03, 0.01, -0.1},
        {-0.05, 0.03, 0.05, 0.05, 0.03, -0.05},
        {0.0, 0.03, 0.05, 0.05, 0.03, -0.05},
        {-0.1, 0.04, 0.01, 0.01, 0.04, -0.1},
        {-0.2, -0.1, -0.05, -0.05, -0.1, -0.2} };
        double KingBoostTable[6][6] = {
        {-0.3, -0.4, -0.5, -0.5, -0.4, -0.3},
        {-0.3, -0.4, -0.5, -0.5, -0.4, -0.3},
        {-0.3, -0.4, -0.5, -0.5, -0.4, -0.3},
        {-0.2, -0.3, -0.4, -0.4, -0.3, -0.2},
        {0.05, 0.05, -0.1, -0.1, 0.05, 0.05},
        {0.2, 0.2, 0.0, 0.0, 0.2, 0.2} };
        double KingBoostTableEndgame[6][6] = {
        {-0.5, -0.35, -0.2, -0.2, -0.35, -0.5},
        {-0.3, -0.05, 0.15, 0.15, -0.05, -0.3},
        {-0.3, 0.1, 0.4, 0.4, 0.1, -0.3},
        {-0.3, 0.1, 0.4, 0.4, 0.1, -0.3},
        {-0.3, -0.05, 0.15, 0.15, -0.05, -0.3},
        {-0.5, -0.3, -0.3, -0.3, -0.3, -0.5} };

        // Due to reduced board size, I lowered the values of pieces other than pawn by 25%,
        // since I think pawns are stronger on a smaller board
    }

    double eval(Position* paramPosition, int color) {
        if (color == 0) {
            int hallo = 5;
        }
        else {
            int hallo = 5;
        }
        // The board is stored at different orientation to the tables. Rotate the board
        std::vector<std::vector<int>> board = rotate90(paramPosition->board);
        int board_size = board.size();
        // Count the number of pieces on the board, to check which King table to use
        int n_pieces = std::accumulate(board.begin(), board.end(), 0,[](int sum, std::vector<int> row) {
            return sum + std::count_if(row.begin(), row.end(),[](int i){
                return i != 0;
                });
            });
        if (n_pieces <= 13) {
            int hallo = 5;
        }
        // Does the board contain a King?
        if (std::find(board.begin(), board.end(), 1) == board.end() || std::find(board.begin(), board.end(), 7) == board.end()) {
            int hallo = 5;
        }
        double d = 0;
        // Add optional randomness to algorithm
        // d = random.random() - 0.5
        for (int k = 0; k < board_size; k++) {
            // ... (rest of your code here) ...
        }
    }


    // Does nothing
    void update(std::vector<std::vector<std::vector<int>>> board_states, int result) {
        int hallo = 5;
    }

    std::vector<std::vector<int>> rotate90(std::vector<std::vector<int>>& matrix) {
        // ... (implement your 90 degree rotation here) ...
    }
    public:
        int color;
        double PawnBoostTable[6][6];
        double KnightBoostTable[6][6];
        double BishopBoostTable[6][6];
        double RookBoostTable[6][6];
        double QueenBoostTable[6][6];
        double KingBoostTable[6][6];
        double KingBoostTableEndgame[6][6];
};

class AdvancedEvaluator8{

public:
    AdvancedEvaluator8(int color) : color(color) {
        double whitePawnBoostTable[8][8] = {
         {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
         {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5},
         {0.1, 0.1, 0.2, 0.3, 0.3, 0.2, 0.1, 0.1},
         {0.05, 0.05, 0.1, 0.25, 0.25, 0.1, 0.05, 0.05},
         {0.0, 0.0, 0.0, 0.2, 0.2, 0.0, 0.0, 0.0},
         {0.05, -0.05, -0.1, 0.0, 0.0, -0.1, -0.05, 0.05},
         {0.05, 0.1, 0.1, -0.2, -0.2, 0.1, 0.1, 0.05},
         {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0} };
        double whiteKnightBoostTable[8][8] = {
         {-0.5, -0.4, -0.3, -0.3, -0.3, -0.3, -0.4, -0.5},
        {-0.4, -0.2, 0.0, 0.0, 0.0, 0.0, -0.2, -0.4},
        {-0.3, 0.0, 0.1, 0.15, 0.15, 0.1, 0.0, -0.3},
        {-0.3, 0.05, 0.15, 0.2, 0.2, 0.15, 0.05, -0.3},
        {-0.3, 0.0, 0.15, 0.2, 0.2, 0.15, 0.0, -0.3},
        {-0.3, 0.05, 0.1, 0.15, 0.15, 0.1, 0.05, -0.3},
        {-0.4, -0.2, 0.0, 0.05, 0.05, 0.0, -0.2, -0.4},
        {-0.5, -0.4, -0.3, -0.3, -0.3, -0.3, -0.4, -0.5} };
        double whiteBishopBoostTable[8][8] = {
         {-0.2, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.2},
         {-0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.1},
         {-0.1, 0.0, 0.05, 0.1, 0.1, 0.05, 0.0, -0.1},
         {-0.1, 0.05, 0.05, 0.1, 0.1, 0.05, 0.05, -0.1},
         {-0.1, 0.0, 0.1, 0.1, 0.1, 0.1, 0.0, -0.1},
         {-0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, -0.1},
         {-0.1, 0.05, 0.0, 0.0, 0.0, 0.0, 0.05, -0.1},
         {-0.2, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.2} };
        double whiteRookBoostTable[8][8] = {
         {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
         {0.05, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.05},
         {-0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.05},
         {-0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.05},
         {-0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.05},
         {-0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.05},
         {-0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.05},
         {0.0, 0.0, 0.0, 0.05, 0.05, 0.0, 0.0, 0.0} };
        double whiteQueenBoostTable[8][8] = {
         {-0.2, -0.1, -0.1, -0.05, -0.05, -0.1, -0.1, -0.2},
         {-0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.1},
         {-0.1, 0.0, 0.05, 0.05, 0.05, 0.05, 0.0, -0.1},
         {-0.05, 0.0, 0.05, 0.05, 0.05, 0.05, 0.0, -0.05},
         {0.0, 0.0, 0.05, 0.05, 0.05, 0.05, 0.0, -0.05},
         {-0.1, 0.05, 0.05, 0.05, 0.05, 0.05, 0.0, -0.1},
         {-0.1, 0.0, 0.05, 0.0, 0.0, 0.0, 0.0, -0.1},
         {-0.2, -0.1, -0.1, -0.05, -0.05, -0.1, -0.1, -0.2} };
        double whiteKingBoostTable[8][8] = {
         {-0.3, -0.4, -0.4, -0.5, -0.5, -0.4, -0.4, -0.3},
         {-0.3, -0.4, -0.4, -0.5, -0.5, -0.4, -0.4, -0.3},
         {-0.3, -0.4, -0.4, -0.5, -0.5, -0.4, -0.4, -0.3},
         {-0.3, -0.4, -0.4, -0.5, -0.5, -0.4, -0.4, -0.3},
         {-0.2, -0.3, -0.3, -0.4, -0.4, -0.3, -0.3, -0.2},
         {-0.1, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.1},
         {0.2, 0.2, 0.0, 0.0, 0.0, 0.0, 0.2, 0.2},
         {0.2, 0.3, 0.1, 0.0, 0.0, 0.1, 0.3, 0.2} };
        double whiteKingBoostTableEndgame[8][8] = {
         {-0.5, -0.4, -0.3, -0.2, -0.2, -0.3, -0.4, -0.5},
         {-0.3, -0.2, -0.1, 0.0, 0.0, -0.1, -0.2, -0.3},
         {-0.3, -0.1, 0.2, 0.3, 0.3, 0.2, -0.1, -0.3},
         {-0.3, -0.1, 0.3, 0.4, 0.4, 0.3, -0.1, -0.3},
         {-0.3, -0.1, 0.3, 0.4, 0.4, 0.3, -0.1, -0.3},
         {-0.3, -0.1, 0.2, 0.3, 0.3, 0.2, -0.1, -0.3},
         {-0.3, -0.3, 0.0, 0.0, 0.0, 0.0, -0.3, -0.3},
         {-0.5, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.5} };
    }


    double eval(std::vector<std::vector<int>>& paramPosition, int color){
        int i = 0, j = 0;
        std::vector<std::vector<int>> board = rotateBoard(paramPosition);
        int board_size = board.size();

        if (board_size == 6) {
            i = 1;
            j = 1;
        }

        int n_pieces = countPieces(board);
        double d = 0.0;

        for (int k = 0; k < board_size; ++k) {
            for (int m = 0; m < board_size; ++m) {
                int x = k + i;
                int y = m + j;

                if (board[k][m] != 0) {
                    if (board[k][m] == 1) {
                        if (n_pieces <= board_size) {
                            d += 1e6 + whiteKingBoostTableEndgame[x][y];
                        }
                        else {
                            d += 1e6 + whiteKingBoostTable[x][y];
                        }
                    }
                    else if (board[k][m] == 2) {
                        d += 9 + whiteQueenBoostTable[x][y];
                    }
                    else if (board[k][m] == 3) {
                        d += 5 + whiteRookBoostTable[x][y];
                    }
                    else if (board[k][m] == 4) {
                        d += 3.3 + whiteBishopBoostTable[x][y];
                    }
                    else if (board[k][m] == 5) {
                        d += 3.2 + whiteKnightBoostTable[x][y];
                    }
                    else if (board[k][m] == 6) {
                        d += 1 + whitePawnBoostTable[x][y];
                    }
                    else if (board[k][m] == 7) {
                        if (n_pieces <= board_size) {
                            d -= 1e6 + whiteKingBoostTableEndgame[8 - x - 1][y];
                        }
                        else {
                            d -= 1e6 + whiteKingBoostTable[8 - x - 1][y];
                        }
                    }
                    else if (board[k][m] == 8) {
                        d -= 9 + whiteQueenBoostTable[8 - x - 1][y];
                    }
                    else if (board[k][m] == 9) {
                        d -= 5 + whiteRookBoostTable[8 - x - 1][y];
                    }
                    else if (board[k][m] == 10) {
                        d -= 3.3 + whiteBishopBoostTable[8 - x - 1][y];
                    }
                    else if (board[k][m] == 11) {
                        d -= 3.2 + whiteKnightBoostTable[8 - x - 1][y];
                    }
                    else if (board[k][m] == 12) {
                        d -= 1 + whitePawnBoostTable[8 - x - 1][y];
                    }
                }
            }
        }

        return d;
    }

    // Other member functions here...

    // Other member variables here...

    std::vector<std::vector<int>> rotateBoard(std::vector<std::vector<int>>& input) {
        int rows = input.size();
        int cols = input[0].size();
        std::vector<std::vector<int>> rotated(cols, std::vector<int>(rows, 0));

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                rotated[j][rows - 1 - i] = input[i][j];
            }
        }

        return rotated;
    }

    int countPieces(std::vector<std::vector<int>>& board) {
        int count = 0;
        for (const auto& row : board) {
            count += std::count_if(row.begin(), row.end(), [](int piece) { return piece != 0; });
        }
        return count;
    }

    private:
        int color;
        double whitePawnBoostTable[8][8];
        double whiteKnightBoostTable[8][8];
        double whiteBishopBoostTable[8][8];
        double whiteRookBoostTable[8][8];
        double whiteQueenBoostTable[8][8];
        double whiteKingBoostTable[8][8];
        double whiteKingBoostTableEndgame[8][8];

};

class LearningEvaluator{
public:
    LearningEvaluator(int color, std::vector<std::vector<int>> startingpos, int brows = 6, int bcols = 6) : color(color) {
        srand(1234321);
        // 0 for white, 1 for black
        // Gather how many pieces of each type there were in the beginning
        startingpiecenumbers = std::vector<int>(N_PIECES, 0);
        for (int i = 0; i < brows; i++) {
            for (int j = 0; j < bcols; j++) {
                if (startingpos[i][j] > 0) {
                    startingpiecenumbers[startingpos[i][j]] += 1;
                }
            }
        }
        N_wins = 1;
        N_losses = 1;
        N_games = 2;
        // How many times each piece has appeared in each location. Initialize with ones to avoid errors
        N_appearances = std::vector<std::vector<std::vector<int>>>(brows + 1, std::vector<std::vector<int>>(bcols + 1, std::vector<int>(N_PIECES, 2)));
        // How many times each piece has appeared in each location, on winning boards
        N_appearances_win = std::vector<std::vector<std::vector<int>>>(brows + 1, std::vector<std::vector<int>>(bcols + 1, std::vector<int>(N_PIECES, 1)));
        // How many times each piece has appeared in each location, on losing boards
        N_appearances_loss = std::vector<std::vector<std::vector<int>>>(brows + 1, std::vector<std::vector<int>>(bcols + 1, std::vector<int>(N_PIECES, 1)));
        // Momentum for counting later games with more weight. The players grow more intelligent,
        // so later games should matter more, in theory.
        gameweight = 1;
        gameweight_increase = 1;
        // Interpretable piece-location values
        piecelocationvalues = std::vector<std::vector<std::vector<int>>>(brows + 1, std::vector<std::vector<int>>(bcols + 1, std::vector<int>(N_PIECES, 0)));
        piecevalues = std::vector<int>(N_PIECES, 0);
    }

    void update(std::vector<std::vector<std::vector<int>>> board_states, int result) {
        // Number of games up by one
        N_games += gameweight;
        // If the result was a victory, number of wins up by one
        if (result == 1) {
            N_wins += gameweight;
        }
        // If it was a loss, number of losses up by one
        else if (result == -1) {
            N_losses += gameweight;
        }
        // Add appearances of all pieces on the board
        for (auto& board_state : board_states) {
            // Keep track of pieces on board, to figure out pieces eaten
            std::vector<int> piecesonboard(N_PIECES, 0);
            for (int i = 0; i < board_state.size(); i++) {
                for (int j = 0; j < board_state[0].size(); j++) {
                    int piece = board_state[i][j];
                    piecesonboard[piece] += 1;
                    if (result == 1) {
                        N_appearances_win[i][j][piece] += gameweight;
                    }
                    else if (result == -1) {
                        N_appearances_loss[i][j][piece] += gameweight;
                    }
                }
            }
            // Add appearances of eaten pieces
            std::vector<int> pieceseaten = subtract(startingpiecenumbers, piecesonboard);
            if (result == 1) {
                N_appearances_win[board_state.size()][board_state[0].size()] = add(N_appearances_win[board_state.size()][board_state[0].size()], multiply(pieceseaten, gameweight));
            }
            if (result == -1) {
                N_appearances_loss[board_state.size()][board_state[0].size()] = add(N_appearances_loss[board_state.size()][board_state[0].size()], multiply(pieceseaten, gameweight));
            }
        }
        // Update piece-location values
        // FIX THIS
        //piecelocationvalues = subtract(log(N_appearances_win), log(N_appearances_loss));
        // Update piece values
        // FIX THIS
        //piecevalues = multiply(-1, piecelocationvalues[board_state.size()][board_state[0].size()]);
        // Give more weight to later games
        gameweight += gameweight_increase;
    }

    double eval(Position* position, int player) {
        double val = 0;
        int rows = position->board.size();
        int cols = position->board[0].size();
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int piece = position->board[i][j];
                // FIX THIS
                //val += log(N_appearances_win[i][j][piece]) - log(N_appearances_loss[i][j][piece]);
            }
        }
        if (player == 0) {
            return val;
        }
        else {
            return -val;
        }
    }

private:
    int color;
    std::vector<int> startingpiecenumbers;
    int N_wins;
    int N_losses;
    int N_games;
    std::vector<std::vector<std::vector<int>>> N_appearances;
    std::vector<std::vector<std::vector<int>>> N_appearances_win;
    std::vector<std::vector<std::vector<int>>> N_appearances_loss;
    int gameweight;
    int gameweight_increase;
    std::vector<std::vector<std::vector<int>>> piecelocationvalues;
    std::vector<int> piecevalues;

    std::vector<int> subtract(std::vector<int> a, std::vector<int> b) {
        std::vector<int> result(a.size());
        std::transform(a.begin(), a.end(), b.begin(), result.begin(), std::minus<int>());
        return result;
    }

    std::vector<int> add(std::vector<int> a, std::vector<int> b) {
        std::vector<int> result(a.size());
        std::transform(a.begin(), a.end(), b.begin(), result.begin(), std::plus<int>());
        return result;
    }

    std::vector<int> multiply(std::vector<int> a, int b) {
        std::vector<int> result(a.size());
        std::transform(a.begin(), a.end(), result.begin(), [](float num, float b){ return num * b; });
        return result;
    }

    std::vector<int> log(std::vector<int> a) {
        std::vector<int> result(a.size());
        // Is num an int here, or 
        std::transform(a.begin(), a.end(), result.begin(), [](float num){ return std::log(num); });
        return result;
    }
};

class BasicEvaluator{
public:
    BasicEvaluator(int color) : color(color) {
        double blackBoostTable[8][8] = {
         {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
         {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
         {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
         {0.0, 0.0, 0.0015, 0.002, 0.002, 0.0015, 0.0, 0.0},
         {0.0, 0.0, 0.002, 0.003, 0.003, 0.002, 0.0, 0.0},
         {0.0, 0.0, 0.0015, 0.001, 0.001, 0.0015, 0.0, 0.0},
         {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
         {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0} };
        double whiteBoostTable[8][8] = {
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0015, 0.001, 0.001, 0.0015, 0.0, 0.0},
        {0.0, 0.0, 0.002, 0.003, 0.003, 0.002, 0.0, 0.0},
        {0.0, 0.0, 0.0015, 0.002, 0.002, 0.0015, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0} };
    }

    double eval(Position* paramPosition) {
        int i = 0; int j = 0;
        // If we are using a 6x6-board, shift the indexes referring to the boost tables by 1,
        // so that the middle part of it will be used
        if (paramPosition->board.size() == 6) {
            i = 1;
            j = 1;
        }
        double d = 0;
        // Add randomness to algorithm
        //d = random.random() - 0.5
        for (int k = 0; k < paramPosition->board.size(); k++) {
            for (int m = 0; m < paramPosition->board[k].size(); m++) {
                if (paramPosition->board[k][m] != 0) {
                    if (paramPosition->board[k][m] == 1) {
                        d += 1e9;
                    }
                    else if (paramPosition->board[k][m] == 2) {
                        d += 9;
                    }
                    else if (paramPosition->board[k][m] == 3) {
                        d += 5.25;
                    }
                    else if (paramPosition->board[k][m] == 4) {
                        d += 3.25;
                    }
                    else if (paramPosition->board[k][m] == 5) {
                        d += 3;
                    }
                    else if (paramPosition->board[k][m] == 6) {
                        d += 1;
                    }
                    else if (paramPosition->board[k][m] == 7) {
                        d -= 1e9;
                    }
                    else if (paramPosition->board[k][m] == 8) {
                        d -= 8.5 - 0.05 * m;
                    }
                    else if (paramPosition->board[k][m] == 9) {
                        d -= 4.75 - 0.05 * m;
                    }
                    else if (paramPosition->board[k][m] == 10) {
                        d -= 2.75 - 0.05 * m;
                    }
                    else if (paramPosition->board[k][m] == 11) {
                        d -= 2.5 - 0.05 * m;
                    }
                    else if (paramPosition->board[k][m] == 12) {
                        d -= 1 - 0.1 * m;
                    }
                    // Check which boost table to use, white or black one. Are we looking at a white or black piece?
                    if (MINWHITEPIECE <= paramPosition->board[k][m] && paramPosition->board[k][m] <= MAXWHITEPIECE) {
                        d += whiteBoostTable[m + j][k + i];
                    }
                    else if (MINBLACKPIECE <= paramPosition->board[k][m] && paramPosition->board[k][m] <= MAXBLACKPIECE) {
                        d -= blackBoostTable[m + j][k + i];
                    }
                }
            }
        }
        return d;
    }

public:
    int color;
    std::vector<std::vector<double>> blackBoostTable;
    std::vector<std::vector<double>> whiteBoostTable;
};

class LearningPiecevalueEvaluator{
public:
    LearningPiecevalueEvaluator(int color) : color(color), BROWS(6), BCOLS(6) {
        srand(1234321);
        // 0 for white, 1 for black
        // How many boards have appeared in games. Initialize with 2's
        boardcounts = 2;
        // How many boards have appeared in won games. Initialize with 1's
        winboardcounts = 1;
        // For weighted averages
        gameweight = 1;
        // How many times each piece has appeared over all games. Initialize with twos
        piececounts = std::vector<int>(N_PIECES, 2);
        // How many times each piece has appeared over all won games. Initialize with ones
        piecewincounts = std::vector<int>(N_PIECES, 1);
        piecevalues = std::vector<double>(N_PIECES, 0.0);
    }

    // For result, 1 = win, -1 = loss, 0 = draw
    void update(std::vector<std::vector<std::vector<int>>> board_states, int result) {
        // IF result was not a draw
        if (result != 0) {
            for (auto& board_state : board_states) {
                for (int i = 0; i < BROWS; i++) {
                    for (int j = 0; j < BCOLS; j++) {
                        int piece = board_state[i][j];
                        // Ignore empty squares, assume they have no effect on probabilities
                        if (piece > 0) {
                            piececounts[piece] += gameweight;
                            if (result == 1) {
                                piecewincounts[piece] += gameweight;
                            }
                        }
                    }
                }
                boardcounts += gameweight;
                if (result == 1) {
                    winboardcounts += gameweight;
                }
            }
            // For each piece, calculate the values so they can be quickly added together in eval-function
            for (int piece = 1; piece < N_PIECES; piece++) {
                piecevalues[piece] = log(piecewincounts[piece] / piececounts[piece]) - log(winboardcounts / boardcounts);
            }
        }
        gameweight += 1;
    }

    double eval(Position* position, int player) {
        double totalvalue = 0;
        for (int i = 0; i < BROWS; i++) {
            for (int j = 0; j < BCOLS; j++) {
                int piece = position->board[i][j];
                if (piece > 0) {
                    totalvalue += piecevalues[piece];
                }
            }
        }
        if (player == 0) {
            return totalvalue;
        }
        else {
            return -totalvalue;
        }
    }


public:
    int color;
    int BROWS;
    int BCOLS;
    int boardcounts;
    int winboardcounts;
    int gameweight;
    std::vector<int> piececounts;
    std::vector<int> piecewincounts;
    std::vector<double> piecevalues;
};

int color = 0;
AdvancedEvaluator6 hallo = AdvancedEvaluator6(color);