#include "PositionHandler.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>

#define EMPTY 0
#define WPAWN 1
#define WKNIGHT 2
#define WBISHOP 3
#define WROOK 4
#define WQUEEN 5
#define WKING 6
#define BPAWN 7
#define BKNIGHT 8
#define BBISHOP 9
#define BROOK 10
#define BQUEEN 11
#define BKING 12

Position::Position(int BROWS, int BCOLS)
    : BROWS{ BROWS }, BCOLS{ BCOLS } {
    Position::board = std::vector<std::vector<int>>(BROWS, std::vector<int>(BCOLS, 0));
    Position::whiteToMove = true;
    Position::cachedResult = NULL;
    Position::winner = 0;  // white = +1, black = -1
    }

// Works for board sizes of 4, 6 or 8
// Board size of 6 has no bishops. Size 4 also has no knights
void Position::setStartingPosition() {
    for (int x = 0; x < BCOLS; x++) {
        board[x][1] = WPAWN;
        board[x][BROWS - 2] = BPAWN;
        if (x == 0 || x == BCOLS - 1) {
            board[x][0] = WROOK;
            board[x][BROWS - 1] = BROOK;
        }
        else if (BCOLS >= 6 && (x == 1 || x == BCOLS - 2)) {
            board[x][0] = WKNIGHT;
            board[x][BROWS - 1] = BKNIGHT;
        }
        else if (BCOLS == 8 && (x == 2 || x == BCOLS - 3)) {
            board[x][0] = WBISHOP;
            board[x][BROWS - 1] = BBISHOP;
        }
        else if (x == BCOLS / 2 - 1) {
            board[x][0] = WQUEEN;
            board[x][BROWS - 1] = BQUEEN;
        }
        else if (x == BCOLS / 2) {
            board[x][0] = WKING;
            board[x][BROWS - 1] = BKING;
        }
    }
    int hallo = 5;
}

void Position::cloneEssentialsFrom(Position* position) {
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            board[i][j] = position->board[i][j];
        }
    }
    whiteToMove = !position->whiteToMove;
}

int Position::countPieces() {
    int amount = 0;
    for (int i = 0; i < BROWS; i++) {
        for (int j = 0; j < BCOLS; j++) {
            if (board[i][j] != 0) {
                amount += 1;
            }
        }
    }
    return amount;
}

// Changes black pieces to white and vice versa?
// Also switches turn
Position* Position::mirror() {
    Position* p = new Position(BROWS, BCOLS);
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            int piece = board[i][BCOLS - 1 - j];
            if (piece != 0) {
                p->board[i][j] = (piece >= 7 ? -6 : 6) + piece;
            }
        }
    }
    p->whiteToMove = !whiteToMove;
    return p;
}

void Position::print() {
    for (int y = BROWS - 1; y >= 0; y--) {
        for (int x = 0; x < BCOLS; x++) {
            int v = board[x][y];
            if (v == EMPTY) {
                std::cout << ".";
            }
            else if (v == WKING) {
                std::cout << "k";
            }
            else if (v == WQUEEN) {
                std::cout << "q";
            }
            else if (v == WROOK) {
                std::cout << "r";
            }
            else if (v == WBISHOP) {
                std::cout << "b";
            }
            else if (v == WKNIGHT) {
                std::cout << "n";
            }
            else if (v == WPAWN) {
                std::cout << "p";
            }
            else if (v == BKING) {
                std::cout << "K";
            }
            else if (v == BQUEEN) {
                std::cout << "Q";
            }
            else if (v == BROOK) {
                std::cout << "R";
            }
            else if (v == BBISHOP) {
                std::cout << "B";
            }
            else if (v == BKNIGHT) {
                std::cout << "N";
            }
            else if (v == BPAWN) {
                std::cout << "P";
            }
        }
        std::cout << "\n";
    }
}

bool Position::isValidPosition(int p) {
    return true;
}

bool Position::isWhitePiece(int pval) {
    if (pval == 0) {
        return false;
    }
    if (pval < 7) {
        return true;
    }
    return false;
}

bool Position::isBlackPiece(int pval) {
    if (pval == 0) {
        return false;
    }
    if (pval > 6) {
        return true;
    }
    return false;
}

bool Position::isInsideBoard(int x, int y) {
    if (x < 0 || x >= BCOLS) {
        return false;
    }
    if (y < 0 || y >= BROWS) {
        return false;
    }
    return true;
}

bool Position::squaresContainSameColoredPieces(int x, int y, int x2, int y2) {
    if (isWhitePiece(board[x][y]) && isWhitePiece(board[x2][y2])) {
        return true;
    }
    if (isBlackPiece(board[x][y]) && isBlackPiece(board[x2][y2])) {
        return true;
    }
    return false;
}

int Position::checkWin(int x, int y) {
    // This is a piece just about to be captured.
    // If white king, black wins, and vice versa
    if (board[x][y] == WKING) {
        return -1;
    }
    if (board[x][y] == BKING) {
        return 1;
    }
    return 0;
}

std::vector<Position*> Position::getNextPositions() {
    std::vector<Position*> ret;

    for (int x = 0; x < board.size(); x++) {
        for (int y = 0; y < board.size(); y++) {
            int pval = board[x][y];

            if (pval == EMPTY) {
                continue;
            }
            if (whiteToMove != isWhitePiece(pval)) {
                continue;
            }

            // PIECE SPECIFIC STUFF
            if (pval == WKING || pval == BKING) {
                for (int i = 0; i < sizeof(KX)/sizeof(KX[0]); i++) {
                    int x2 = KX[i] + x;
                    int y2 = KY[i] + y;
                    if (!isInsideBoard(x2, y2)) {
                        continue;
                    }
                    if (squaresContainSameColoredPieces(x, y, x2, y2)) {
                        continue;
                    }
                    Position* p = new Position(BROWS, BCOLS);
                    p->cloneEssentialsFrom(this);
                    p->winner = checkWin(x2, y2);
                    p->board[x2][y2] = board[x][y];
                    p->board[x][y] = EMPTY;
                    ret.push_back(p);
                }
                continue;
            }

            if (pval == WQUEEN || pval == BQUEEN) {
                // Queens can move like bishops:
                for (int i = 0; i < sizeof(BX)/sizeof(BX[0]); i++) {
                    // For all the directions
                    for (int j = 0; j < sizeof(BX[i])/sizeof(BX[i][0]); j++) {
                        // Once a direction is obstructed, finish!
                        int x2 = BX[i][j] + x;
                        int y2 = BY[i][j] + y;
                        if (!isInsideBoard(x2, y2)) {
                            break;
                        }
                        if (squaresContainSameColoredPieces(x, y, x2, y2)) {
                            break;
                        }

                        Position* p = new Position(BROWS, BCOLS);
                        p->cloneEssentialsFrom(this);
                        p->winner = checkWin(x2, y2);
                        p->board[x2][y2] = board[x][y];
                        p->board[x][y] = EMPTY;
                        ret.push_back(p);

                        if (board[x2][y2] != EMPTY) {
                            // Ate it, and finished direction
                            break;
                        }
                    }
                }

                // Queens can also move like rooks:
                for (int i = 0; i < sizeof(RX) / sizeof(RX[0]); i++) {
                    // for all the directions
                    for (int j = 0; j < sizeof(RX[i]) / sizeof(RX[i][0]); j++) {
                        // once a direction is obstructed, finish
                        int x2 = RX[i][j] + x;
                        int y2 = RY[i][j] + y;
                        if (!isInsideBoard(x2, y2)) {
                            break;
                        }
                        if (squaresContainSameColoredPieces(x, y, x2, y2)) {
                            break;
                        }

                        Position* p = new Position(BROWS, BCOLS);
                        p->cloneEssentialsFrom(this);
                        p->winner = checkWin(x2, y2);
                        p->board[x2][y2] = board[x][y];
                        p->board[x][y] = EMPTY;
                        ret.push_back(p);

                        if (board[x2][y2] != EMPTY) {
                            // ate it, and finished direction
                            break;
                        }
                    }
                }
                continue;
            }

            if (pval == WROOK || pval == BROOK) {
                for (int i = 0; i < sizeof(RX) / sizeof(RX[0]); i++) {
                    // for all the directions
                    for (int j = 0; j < sizeof(RX[i]) / sizeof(RX[i][0]); j++) {
                        // once a direction is obstructed, finish
                        int x2 = RX[i][j] + x;
                        int y2 = RY[i][j] + y;
                        if (!isInsideBoard(x2, y2)) {
                            break;
                        }
                        if (squaresContainSameColoredPieces(x, y, x2, y2)) {
                            break;
                        }

                        Position* p = new Position(BROWS, BCOLS);
                        p->cloneEssentialsFrom(this);
                        p->winner = checkWin(x2, y2);
                        p->board[x2][y2] = board[x][y];
                        p->board[x][y] = EMPTY;
                        ret.push_back(p);

                        if (board[x2][y2] != EMPTY) {
                            // ate it, and finished direction
                            break;
                        }
                    }
                }
                continue;
            }

            if (pval == WBISHOP || pval == BBISHOP) {
                for (int i = 0; i < sizeof(BX) / sizeof(BX[0]); i++) {
                    // For all the directions
                    for (int j = 0; j < sizeof(BX[i]) / sizeof(BX[i][0]); j++) {
                        // Once a direction is obstructed, finish!
                        int x2 = BX[i][j] + x;
                        int y2 = BY[i][j] + y;
                        if (!isInsideBoard(x2, y2)) {
                            break;
                        }
                        if (squaresContainSameColoredPieces(x, y, x2, y2)) {
                            break;
                        }

                        Position* p = new Position(BROWS, BCOLS);
                        p->cloneEssentialsFrom(this);
                        p->winner = checkWin(x2, y2);
                        p->board[x2][y2] = board[x][y];
                        p->board[x][y] = EMPTY;
                        ret.push_back(p);

                        if (board[x2][y2] != EMPTY) {
                            // Ate it, and finished direction
                            break;
                        }
                    }
                }
                continue;
            }

            if (pval == WKNIGHT || pval == BKNIGHT) {
                for (int i = 0; i < sizeof(NX) / sizeof(NX[0]); i++) {
                    int x2 = NX[i] + x;
                    int y2 = NY[i] + y;
                    if (!isInsideBoard(x2, y2)) {
                        continue;
                    }
                    if (squaresContainSameColoredPieces(x, y, x2, y2)) {
                        continue;
                    }

                    Position* p = new Position(BROWS, BCOLS);
                    p->cloneEssentialsFrom(this);
                    p->winner = checkWin(x2, y2);
                    p->board[x2][y2] = board[x][y];
                    p->board[x][y] = EMPTY;
                    ret.push_back(p);
                }
                continue;
            }

            if (pval == WPAWN) {
                std::vector<bool> allowedMoves(4, true);
                // 1 step forward
                allowedMoves[0] = (isInsideBoard(x, y + 1) && (board[x][y + 1] == EMPTY));
                // 2 steps forward (not in Los Alamos chess)
                allowedMoves[1] = (BROWS == 8 && isInsideBoard(x, y + 2) && y == 1 && allowedMoves[0] && board[x][y + 2] == EMPTY);
                // eat left
                allowedMoves[2] = isInsideBoard(x - 1, y + 1) && isBlackPiece(board[x - 1][y + 1]);
                // eat right
                allowedMoves[3] = isInsideBoard(x + 1, y + 1) && isBlackPiece(board[x + 1][y + 1]);

                if (allowedMoves[0]) {
                    if (y + 1 != BROWS - 1) {
                        Position* p = new Position(BROWS, BCOLS);
                        p->cloneEssentialsFrom(this);
                        p->board[x][y + 1] = pval;
                        p->board[x][y] = EMPTY;
                        ret.push_back(p);
                    }
                    // Pawn promotion
                    else {
                        Position* p = new Position(BROWS, BCOLS);
                        p->cloneEssentialsFrom(this);
                        p->board[x][y + 1] = WKNIGHT;
                        p->board[x][y] = EMPTY;
                        ret.push_back(p);
                        if (BCOLS == 8) {
                            p = new Position(BROWS, BCOLS);
                            p->cloneEssentialsFrom(this);
                            p->board[x][y + 1] = WBISHOP;
                            p->board[x][y] = EMPTY;
                            ret.push_back(p);
                        }
                        p = new Position(BROWS, BCOLS);
                        p->cloneEssentialsFrom(this);
                        p->board[x][y + 1] = WROOK;
                        p->board[x][y] = EMPTY;
                        ret.push_back(p);
                        p = new Position(BROWS, BCOLS);
                        p->cloneEssentialsFrom(this);
                        p->board[x][y + 1] = WQUEEN;
                        p->board[x][y] = EMPTY;
                        ret.push_back(p);
                    }
                }

                if (allowedMoves[1]) {
                    Position* p = new Position(BROWS, BCOLS);
                    p->cloneEssentialsFrom(this);
                    p->board[x][y + 2] = pval;
                    p->board[x][y] = EMPTY;
                    ret.push_back(p);
                }

                if (allowedMoves[2]) {
                    if (y + 1 != BROWS - 1) {
                        Position* p = new Position(BROWS, BCOLS);
                        p->cloneEssentialsFrom(this);
                        p->winner = checkWin(x - 1, y + 1);
                        p->board[x - 1][y + 1] = pval;
                        p->board[x][y] = EMPTY;
                        ret.push_back(p);
                    }
                    // Pawn promotion
                    else {
                        Position* p = new Position(BROWS, BCOLS);
                        p->cloneEssentialsFrom(this);
                        p->winner = checkWin(x - 1, y + 1);
                        p->board[x - 1][y + 1] = WKNIGHT;
                        p->board[x][y] = EMPTY;
                        ret.push_back(p);
                        if (BCOLS == 8) {
                            p = new Position(BROWS, BCOLS);
                            p->cloneEssentialsFrom(this);
                            p->winner = checkWin(x - 1, y + 1);
                            p->board[x - 1][y + 1] = WBISHOP;
                            p->board[x][y] = EMPTY;
                            ret.push_back(p);
                        }
                        p = new Position(BROWS, BCOLS);
                        p->cloneEssentialsFrom(this);
                        p->winner = checkWin(x - 1, y + 1);
                        p->board[x - 1][y + 1] = WQUEEN;
                        p->board[x][y] = EMPTY;
                        ret.push_back(p);
                        if (allowedMoves[3]) {
                            if (y + 1 != BROWS - 1) {
                                Position* p = new Position(BROWS, BCOLS);
                                p->cloneEssentialsFrom(this);
                                p->winner = checkWin(x + 1, y + 1);
                                p->board[x + 1][y + 1] = pval;
                                p->board[x][y] = EMPTY;
                                ret.push_back(p);
                            }
                            // Pawn promotion
                            else {
                                Position* p = new Position(BROWS, BCOLS);
                                p->cloneEssentialsFrom(this);
                                p->winner = checkWin(x + 1, y + 1);
                                p->board[x + 1][y + 1] = WKNIGHT;
                                p->board[x][y] = EMPTY;
                                ret.push_back(p);
                                if (BCOLS == 8) {
                                    p = new Position(BROWS, BCOLS);
                                    p->cloneEssentialsFrom(this);
                                    p->winner = checkWin(x + 1, y + 1);
                                    p->board[x + 1][y + 1] = WBISHOP;
                                    p->board[x][y] = EMPTY;
                                    ret.push_back(p);
                                }
                                p = new Position(BROWS, BCOLS);
                                p->cloneEssentialsFrom(this);
                                p->winner = checkWin(x + 1, y + 1);
                                p->board[x + 1][y + 1] = WROOK;
                                p->board[x][y] = EMPTY;
                                ret.push_back(p);
                                p = new Position(BROWS, BCOLS);
                                p->cloneEssentialsFrom(this);
                                p->winner = checkWin(x + 1, y + 1);
                                p->board[x + 1][y + 1] = WQUEEN;
                                p->board[x][y] = EMPTY;
                                ret.push_back(p);
                            }
                            continue;
                        }
                        if (pval == BPAWN) {
                            std::vector<bool> allowedMoves(4, true);
                            // 1 step forward
                            allowedMoves[0] = (isInsideBoard(x, y - 1) && (board[x][y - 1] == EMPTY));
                            // 2 steps forward (not in Los Alamos chess)
                            allowedMoves[1] = BROWS == 8 && isInsideBoard(x, y - 2) && y == 6 &&
                                allowedMoves[0] && board[x][y - 2] == EMPTY;
                            // eat left
                            allowedMoves[2] = isInsideBoard(x - 1, y - 1) && isWhitePiece(board[x - 1][y - 1]);
                            // eat right
                            allowedMoves[3] = isInsideBoard(x + 1, y - 1) && isWhitePiece(board[x + 1][y - 1]);

                            if (allowedMoves[0]) {
                                if (y - 1 != 0) {
                                    Position* p = new Position(BROWS, BCOLS);
                                    p->cloneEssentialsFrom(this);
                                    p->board[x][y - 1] = pval;
                                    p->board[x][y] = EMPTY;
                                    // Doesn't this slow down the code, due to memory reallocation?
                                    ret.push_back(p);
                                }
                                else {
                                    Position* p = new Position(BROWS, BCOLS);
                                    p->cloneEssentialsFrom(this);
                                    p->board[x][y - 1] = BKNIGHT;
                                    p->board[x][y] = EMPTY;
                                    ret.push_back(p);
                                    if (BCOLS == 8) {
                                        p = new Position(BROWS, BCOLS);
                                        p->cloneEssentialsFrom(this);
                                        p->board[x][y - 1] = BBISHOP;
                                        p->board[x][y] = EMPTY;
                                        ret.push_back(p);
                                    }
                                    p = new Position(BROWS, BCOLS);
                                    p->cloneEssentialsFrom(this);
                                    p->board[x][y - 1] = BROOK;
                                    p->board[x][y] = EMPTY;
                                    ret.push_back(p);
                                    p = new Position(BROWS, BCOLS);
                                    p->cloneEssentialsFrom(this);
                                    p->board[x][y - 1] = BQUEEN;
                                    p->board[x][y] = EMPTY;
                                    ret.push_back(p);
                                }
                            }
                        }
                        if (allowedMoves[1]) {
                            Position* p = new Position(BROWS, BCOLS);
                            p->cloneEssentialsFrom(this);
                            p->board[x][y - 2] = pval;
                            p->board[x][y] = EMPTY;
                            ret.push_back(p);
                        }
                                    if (allowedMoves[2]) {
                                        if (y - 1 != 0) {
                                            Position* p = new Position(BROWS, BCOLS);
                                            p->cloneEssentialsFrom(this);
                                            p->winner = checkWin(x - 1, y - 1);
                                            p->board[x - 1][y - 1] = pval;
                                            p->board[x][y] = EMPTY;
                                            ret.push_back(p);
                                        }
                                        else {
                                            Position* p = new Position(BROWS, BCOLS);
                                            p->cloneEssentialsFrom(this);
                                            p->winner = checkWin(x - 1, y - 1);
                                            p->board[x - 1][y - 1] = BKNIGHT;
                                            p->board[x][y] = EMPTY;
                                            ret.push_back(p);
                                            if (BCOLS == 8) {
                                                p = new Position(BROWS, BCOLS);
                                                p->cloneEssentialsFrom(this);
                                                p->winner = checkWin(x - 1, y - 1);
                                                p->board[x - 1][y - 1] = BBISHOP;
                                                p->board[x][y] = EMPTY;
                                                ret.push_back(p);
                                            }
                                            p = new Position(BROWS, BCOLS);
                                            p->cloneEssentialsFrom(this);
                                            p->winner = checkWin(x - 1, y - 1);
                                            p->board[x - 1][y - 1] = BROOK;
                                            p->board[x][y] = EMPTY;
                                            ret.push_back(p);
                                            p = new Position(BROWS, BCOLS);
                                            p->cloneEssentialsFrom(this);
                                            p->winner = checkWin(x - 1, y - 1);
                                            p->board[x - 1][y - 1] = BQUEEN;
                                            p->board[x][y] = EMPTY;
                                            ret.push_back(p);
                                        }
                                    }

                        if (allowedMoves[3]) {
                            if (y - 1 != 0) {
                                Position* p = new Position(BROWS, BCOLS);
                                p->cloneEssentialsFrom(this);
                                p->winner = checkWin(x + 1, y - 1);
                                p->board[x + 1][y - 1] = pval;
                                p->board[x][y] = EMPTY;
                                ret.push_back(p);
                            }
                            else {
                                Position* p = new Position(BROWS, BCOLS);
                                p->cloneEssentialsFrom(this);
                                p->winner = checkWin(x + 1, y - 1);
                                p->board[x + 1][y - 1] = BKNIGHT;
                                p->board[x][y] = EMPTY;
                                ret.push_back(p);
                                if (BCOLS == 8) {
                                    p = new Position(BROWS, BCOLS);
                                    p->cloneEssentialsFrom(this);
                                    p->winner = checkWin(x + 1, y - 1);
                                    p->board[x + 1][y - 1] = BBISHOP;
                                    p->board[x][y] = EMPTY;
                                    ret.push_back(p);
                                }
                                p = new Position(BROWS, BCOLS);
                                p->cloneEssentialsFrom(this);
                                p->winner = checkWin(x + 1, y - 1);
                                p->board[x + 1][y - 1] = BROOK;
                                p->board[x][y] = EMPTY;
                                ret.push_back(p);
                                p = new Position(BROWS, BCOLS);
                                p->cloneEssentialsFrom(this);
                                p->winner = checkWin(x + 1, y - 1);
                                p->board[x + 1][y - 1] = BQUEEN;
                                p->board[x][y] = EMPTY;
                                ret.push_back(p);
                            }
                            continue;
                        }

                        return ret;
                    }
                }

                return ret;
            }
        }
    }
}