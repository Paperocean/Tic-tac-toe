#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

enum Difficulty {
    EASY,
    MEDIUM,
    HARD,
    ULTRA
};

class Game {
private:
    std::vector<std::vector<char>> board;
    int boardSize;
    int winLength;
    char currentPlayer;
    char lastPlayer;
	int playerMoves;
	int aiMoves;
	int aiMoves2;

    int totalDepth = 0;
    int numSearches = 0;

public:
    Game();
    Game(int size, int win);
    void printBoard();
    char getBoardValue(int row, int col) const;
    int getBoardSize() const;
    char getLastPlayer() const;
    bool checkWin(char player);
    bool isBoardFull();
    void setBoard(int newBoardSize);
    void setWinLength(int newWinLength);

    int minimax(char player, int depth, int maxDepth, bool isMaximizing, int alpha, int beta);
    bool aiMove(Difficulty difficulty);
    int minimax2(char player, int depth, int maxDepth, bool isMaximizing, int alpha, int beta);
    bool ai2Move(Difficulty difficulty);
    bool playerMove(int row, int col);

    void resetBoard();
    int getPlayerMoves();
    int getAIMoves();
    double averageDepth();
    void printStats(Difficulty difficulty);
};

#endif // !TICTACTOE_H
