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
    char lastPlayer;
	int playerMoves;
	int aiMoves;
	int aiMoves2;

public:
    TicTacToe() : boardSize(3), winLength(3), currentPlayer('X'), lastPlayer('O'), playerMoves(0), aiMoves(0), aiMoves2(0) {
        board = std::vector<std::vector<char>>(boardSize, std::vector<char>(boardSize, ' '));
    }

    TicTacToe(int size, int win) : boardSize(size), winLength(win), currentPlayer('X'), lastPlayer('O'), playerMoves(0), aiMoves(0), aiMoves2(0) {
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

    char getBoardValue(int row, int col) const {
        return board[row][col];
    }

	int getBoardSize() const {
		return boardSize;
	}

    char getLastPlayer() const {
        return lastPlayer;
    }

    bool checkWin(char player) {
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j <= boardSize - winLength; j++) {
                int k;
                for (k = 0; k < winLength; k++) {
                    if (board[i][j + k] != player)
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
                    if (board[j + k][i] != player)
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
                    if (board[i + k][j + k] != player)
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
                    if (board[i + k][j - k] != player)
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
                if (board[i][j] == ' ') {
                    return false;
                }
            }
        }
        return true;
    }

    void setBoard(int newBoardSize) {
        boardSize = newBoardSize;
        std::vector<std::vector<char>> newBoard(boardSize, std::vector<char>(boardSize, ' '));
        board = newBoard;
    }

    void setWinLength(int newWinLength) {
        winLength = newWinLength;
    }

    int minimax(char player, int depth, int maxDepth, bool isMaximizing, int alpha, int beta) {
        char opponent = (player == 'O') ? 'X' : 'O';

        // Check for terminal states (win/loss/draw)
        if (checkWin(player)) {
            return (player == 'O') ? 100 - depth : depth - 100;
        }
        if (checkWin(opponent)) {
            return (opponent == 'O') ? 100 - depth : depth - 100;
        }
        if (isBoardFull() || depth == maxDepth) {
            return 0; // Draw or maximum depth reached
        }

        if (isMaximizing) {
            int bestScore = INT_MIN;
            for (int i = 0; i < boardSize; i++) {
                for (int j = 0; j < boardSize; j++) {
                    if (board[i][j] == ' ') {
                        board[i][j] = 'O';
                        int score = minimax('O', depth + 1, maxDepth, false, alpha, beta);
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
                        int score = minimax('X', depth + 1, maxDepth, true, alpha, beta);
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

    bool aiMove(Difficulty difficulty) {
        if (isBoardFull()) {
            return false;
        }

        int maxDepth;
        switch (difficulty) {
        case EASY:
            maxDepth = 1;
            break;
        case MEDIUM:
            maxDepth = 2;
            break;
        case HARD:
            maxDepth = 4;
            break;
        case ULTRA:
            maxDepth = boardSize * boardSize; // Use a high depth for ultra difficulty
            break;
        default:
            maxDepth = 2;
        }

		cout << "AI is thinking..." << endl;
		cout << difficulty << endl;

        int bestScore = INT_MIN;
        std::pair<int, int> bestMove = { -1, -1 };

        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'O';
                    int score = minimax('O', 0, maxDepth, false, INT_MIN, INT_MAX);
                    board[i][j] = ' ';
                    if (score > bestScore) {
                        bestScore = score;
                        bestMove = { i, j };
                    }
                }
            }
        }

        if (bestMove.first != -1 && bestMove.second != -1) {
            board[bestMove.first][bestMove.second] = 'O';
            lastPlayer = 'O';
            currentPlayer = 'X';
            aiMoves++;
            return true;
        }
        return false;
    }

    int minimax2(char player, int depth, int maxDepth, bool isMaximizing, int alpha, int beta) {
        char opponent = (player == 'X') ? 'X' : 'O';

        // Check for terminal states (win/loss/draw)
        if (checkWin(player)) {
            return (player == 'X') ? 100 - depth : depth - 100;
        }
        if (checkWin(opponent)) {
            return (opponent == 'X') ? 100 - depth : depth - 100;
        }
        if (isBoardFull() || depth == maxDepth) {
            return 0; // Draw or maximum depth reached
        }

        if (isMaximizing) {
            int bestScore = INT_MIN;
            for (int i = 0; i < boardSize; i++) {
                for (int j = 0; j < boardSize; j++) {
                    if (board[i][j] == ' ') {
                        board[i][j] = 'X';
                        int score = minimax('X', depth + 1, maxDepth, false, alpha, beta);
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
                        board[i][j] = 'O';
                        int score = minimax('O', depth + 1, maxDepth, true, alpha, beta);
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

    bool ai2Move(Difficulty difficulty) {
        if (isBoardFull()) {
            return false;
        }

        if (aiMoves2 % 2 == 0) {
            // First move, choose a random square
            int row, col;
            do {
                row = rand() % boardSize;
                col = rand() % boardSize;
            } while (board[row][col] != ' ');

            board[row][col] = 'X';
            lastPlayer = 'X';
            currentPlayer = 'O';
            aiMoves2++;
            return true;
        }

        int maxDepth;
        switch (difficulty) {
        case EASY:
            maxDepth = 1;
            break;
        case MEDIUM:
            maxDepth = 2;
            break;
        case HARD:
            maxDepth = 4;
            break;
        case ULTRA:
            maxDepth = boardSize * boardSize; // Use a high depth for ultra difficulty
            break;
        default:
            maxDepth = 2;
        }

        cout << "AI2 is thinking..." << endl;
        cout << difficulty << endl;

        int bestScore = INT_MIN;
        std::pair<int, int> bestMove = { -1, -1 };

        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'X';
                    int score = minimax2('X', 0, maxDepth, false, INT_MIN, INT_MAX);
                    board[i][j] = ' ';
                    if (score > bestScore) {
                        bestScore = score;
                        bestMove = { i, j };
                    }
                }
            }
        }

        if (bestMove.first != -1 && bestMove.second != -1) {
            board[bestMove.first][bestMove.second] = 'X';
            lastPlayer = 'X';
            currentPlayer = 'O';
            aiMoves2++;
            return true;
        }
        return false;
    }

    bool playerMove(int row, int col) {
        if (row < 0 || row >= boardSize || col < 0 || col >= boardSize) {
            return false;
        }

        if (board[row][col] == ' ') {
            if (lastPlayer == 'O') {
                board[row][col] = 'X';
                lastPlayer = 'X';
                playerMoves++;
            }
            return true;
        }
        return false;
    }

	void resetBoard() {
        setBoard(boardSize);
		setWinLength(winLength);
		board = std::vector<std::vector<char>>(boardSize, std::vector<char>(boardSize, ' '));
		currentPlayer = 'X';
		lastPlayer = 'O';
		playerMoves = 0;
		aiMoves = 0;
	}

    int getPlayerMoves() {
        return playerMoves;
    }

	int getAIMoves() {
		return aiMoves;
	}
};

#endif // !TICTACTOE_H
