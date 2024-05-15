#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

enum Difficulty {
    EASY,
    MEDIUM,
    HARD,
    ULTRA
};

class TicTacToe {
private:
    std::vector<std::vector<char>> board;
    int boardSize;
    int winLength;
    char currentPlayer;

public:
    TicTacToe() : boardSize(3), winLength(3), currentPlayer('X') {
        board = std::vector<std::vector<char>>(boardSize, std::vector<char>(boardSize, ' '));
    }

    TicTacToe(int size, int win) : boardSize(size), winLength(win), currentPlayer('X') {
        board = std::vector<std::vector<char>>(boardSize, std::vector<char>(boardSize, ' '));
    }

    void printBoard() {
        for (int i = 0; i < boardSize; i++) {
            if (i == 0) cout << "  ";
            cout << i << " ";
            if (i == boardSize - 1) {
                cout << endl;
            }
        }
        for (int i = 0; i < boardSize; i++) {
            cout << i << " ";
            for (int j = 0; j < boardSize; j++) {
                std::cout << board[i][j] << " ";
            }
            std::cout << "\n";
        }
    }

    bool makeMove(int row, int col) {
        if (row >= 0 && row < boardSize && col >= 0 && col < boardSize && board[row][col] == ' ') {
            board[row][col] = currentPlayer;
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
            return true;
        }
        return false;
    }
    char getBoardValue(int row, int col) const {
        return board[row][col];
    }

    bool checkWin() {
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j <= boardSize - winLength; j++) {
                int k;
                for (k = 0; k < winLength; k++) {
                    if (board[i][j + k] != currentPlayer)
                        break;
                }
                if (k == winLength)
                    return true;
            }
        }

        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j <= boardSize - winLength; j++) {
                int k;
                for (k = 0; k < winLength; k++) {
                    if (board[j + k][i] != currentPlayer)
                        break;
                }
                if (k == winLength)
                    return true;
            }
        }

        for (int i = 0; i <= boardSize - winLength; i++) {
            for (int j = 0; j <= boardSize - winLength; j++) {
                int k;
                for (k = 0; k < winLength; k++) {
                    if (board[i + k][j + k] != currentPlayer)
                        break;
                }
                if (k == winLength)
                    return true;
            }
        }

        for (int i = 0; i <= boardSize - winLength; i++) {
            for (int j = boardSize - 1; j >= winLength - 1; j--) {
                int k;
                for (k = 0; k < winLength; k++) {
                    if (board[i + k][j - k] != currentPlayer)
                        break;
                }
                if (k == winLength)
                    return true;
            }
        }

        return false;
    }

    bool isBoardFull() {
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                if (board[i][j] == ' ')
                    return false;
            }
        }
        return true;
    }

    int scoreBoard() {
        if (checkWin()) {
            return (currentPlayer == 'O') ? 1 : -1;
        }
        return 0;
    }

    void setBoard(int newBoardSize) {
        boardSize = newBoardSize;
    }

    void setWinLength(int newWinLength) {
        winLength = newWinLength;
    }

    int minimax(int depth, int maxDepth, bool isMaximizing, int alpha, int beta) {
        if (depth == maxDepth || isBoardFull()) {
            return scoreBoard();
        }

        if (isMaximizing) {
            int bestScore = INT_MIN;
            for (int i = 0; i < boardSize; i++) {
                for (int j = 0; j < boardSize; j++) {
                    if (board[i][j] == ' ') {
                        board[i][j] = 'O';
                        int score = minimax(depth + 1, maxDepth, false, alpha, beta);
                        board[i][j] = ' ';
                        bestScore = std::max(score, bestScore);
                        alpha = std::max(alpha, score);
                        if (beta <= alpha) {
                            return bestScore;
                        }
                    }
                }
            }
            return bestScore;
        }
        else {
            int bestScore = INT_MAX;
            for (int i = 0; i < boardSize; i++) {
                for (int j = 0; j < boardSize; j++) {
                    if (board[i][j] == ' ') {
                        board[i][j] = 'X';
                        int score = minimax(depth + 1, maxDepth, true, alpha, beta);
                        board[i][j] = ' ';
                        bestScore = std::min(score, bestScore);
                        beta = std::min(beta, score);
                        if (beta <= alpha) {
                            return bestScore;
                        }
                    }
                }
            }
            return bestScore;
        }
    }

    void aiMove(Difficulty difficulty) {
        int bestScore = INT_MIN;
        std::pair<int, int> move;
        int maxDepth;

        switch (difficulty) {
        case EASY:
            maxDepth = 1;
            break;
        case MEDIUM:
            maxDepth = 2;
            break;
        case HARD:
            maxDepth = boardSize;
            break;
        case ULTRA:
            maxDepth = boardSize * boardSize;
        }

        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'O';
                    int score = minimax(0, maxDepth, false, INT_MIN, INT_MAX);
                    board[i][j] = ' ';
                    if (score > bestScore) {
                        bestScore = score;
                        move = { i, j };
                    }
                }
            }
        }
        board[move.first][move.second] = 'O';
        currentPlayer = 'X';
    }
};

#endif // !TICTACTOE_H
