#include "game.h"
#include <chrono>
using namespace std;

int minimaxCalls = 0;
std::chrono::microseconds minimaxTime;

Game::Game() : boardSize(3), winLength(3), currentPlayer('X'), lastPlayer('O'), playerMoves(0), aiMoves(0), aiMoves2(0) {
    board = std::vector<std::vector<char>>(boardSize, std::vector<char>(boardSize, ' '));
}

Game::Game(int size, int win) : boardSize(size), winLength(win), currentPlayer('X'), lastPlayer('O'), playerMoves(0), aiMoves(0), aiMoves2(0) {
    board = std::vector<std::vector<char>>(boardSize, std::vector<char>(boardSize, ' '));
}

void Game::printBoard() {
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

char Game::getBoardValue(int row, int col) const {
    return board[row][col];
}

int Game::getBoardSize() const {
    return boardSize;
}

char Game::getLastPlayer() const {
    return lastPlayer;
}

bool Game::checkWin(char player) {
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0, k; j <= boardSize - winLength; j++) {
            for (k = 0; k < winLength; k++) {
                if (board[i][j + k] != player)
                    break;
            }
            if (k == winLength)
                return true;
        }
    }

    for (int i = 0; i < boardSize; i++) {
        for (int j = 0, k; j <= boardSize - winLength; j++) {
            for (k = 0; k < winLength; k++) {
                if (board[j + k][i] != player)
                    break;
            }
            if (k == winLength)
                return true;
        }
    }

    for (int i = 0; i <= boardSize - winLength; i++) {
        for (int j = 0, k; j <= boardSize - winLength; j++) {
            for (k = 0; k < winLength; k++) {
                if (board[i + k][j + k] != player)
                    break;
            }
            if (k == winLength)
                return true;
        }
    }

    for (int i = 0; i <= boardSize - winLength; i++) {
        for (int j = boardSize - 1, k; j >= winLength - 1; j--) {
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

bool Game::isBoardFull() {
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (board[i][j] == ' ') {
                return false;
            }
        }
    }
    return true;
}

void Game::setBoard(int newBoardSize) {
    boardSize = newBoardSize;
    std::vector<std::vector<char>> newBoard(boardSize, std::vector<char>(boardSize, ' '));
    board = newBoard;
}

void Game::setWinLength(int newWinLength) {
    winLength = newWinLength;
}

int Game::minimax(char player, int depth, int maxDepth, bool isMaximizing, int alpha, int beta) {
    totalDepth += depth;
    numSearches++;

    minimaxCalls++;
    auto start = std::chrono::high_resolution_clock::now();


    char opponent = (player == 'O') ? 'X' : 'O';

    // Check for terminal states (win/loss/draw)
    if (checkWin(player)) {
        auto end = std::chrono::high_resolution_clock::now();
        minimaxTime += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        return (player == 'O') ? 100 - depth : depth - 100;
    }
    if (checkWin(opponent)) {
        auto end = std::chrono::high_resolution_clock::now();
        minimaxTime += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        return (opponent == 'O') ? 100 - depth : depth - 100;
    }
    if (isBoardFull() || depth == maxDepth) {
        auto end = std::chrono::high_resolution_clock::now();
        minimaxTime += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
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
        auto end = std::chrono::high_resolution_clock::now();
        minimaxTime += std::chrono::duration_cast<std::chrono::microseconds>(end - start);

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
        auto end = std::chrono::high_resolution_clock::now();
        minimaxTime += std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        return bestScore;
    }
}

bool Game::aiMove(Difficulty difficulty) {
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

    /*cout << "AI is thinking..." << endl;
    cout << difficulty << endl;*/

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

int Game::minimax2(char player, int depth, int maxDepth, bool isMaximizing, int alpha, int beta) {
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

bool Game::ai2Move(Difficulty difficulty) {
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

bool Game::playerMove(int row, int col) {
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

void Game::resetBoard() {
    setBoard(boardSize);
    setWinLength(winLength);
    board = std::vector<std::vector<char>>(boardSize, std::vector<char>(boardSize, ' '));
    currentPlayer = 'X';
    lastPlayer = 'O';
    playerMoves = 0;
    aiMoves = 0;

	totalDepth = 0;
	numSearches = 0;
	minimaxCalls = 0;
	minimaxTime = std::chrono::microseconds(0);
}

int Game::getPlayerMoves() {
    return playerMoves;
}

int Game::getAIMoves() {
    return aiMoves;
}

double Game::averageDepth() {
    if (numSearches == 0) {
        return 0;
    }
    else {
        return static_cast<double>(totalDepth) / numSearches;
    }
}

void Game::printStats(Difficulty difficulty) {
	if (!averageDepth() || !minimaxCalls || minimaxTime.count() == 0) {
		return;
	}
    std::ofstream file("performance_data.csv", std::ios::app);
    file << "Board size," << "Win length," << "Difficulty," << "Average search depth," << "Minimax calls," << "Minimax time\n";
    file << boardSize << ",";
    file << winLength << ",";
    file << difficulty << ",";
    file << averageDepth() << ",";
    file << minimaxCalls << ",";
    file << minimaxTime.count() << "\n";

    file.close();
}