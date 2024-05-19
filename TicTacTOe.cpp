#include <iostream>
#include <vector>
#include <climits>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include "game.h"

using namespace std;

Difficulty aiDifficulty = ULTRA;

enum GameState {
    MENU,
    PLAYING,
    WAITING_TO_PLAY,
	WAITING_FOR_AI,
    GAME_OVER
};

void drawBoard(sf::RenderWindow& window, Game& game, sf::Font& font, int boardSize) {
    //int boardSize = 5;
    float boardSizeF = static_cast<float>(boardSize);
    float windowSizeXF = static_cast<float>(window.getSize().x);
    float windowSizeYF = static_cast<float>(window.getSize().y);
    float cellWidth = windowSizeXF / boardSizeF;
    float cellHeight = windowSizeYF / boardSizeF;

    sf::Color lineColor(200, 200, 200);
    sf::Color cellColor(50, 50, 50);
    sf::Color xColor(255, 0, 0);
    sf::Color oColor(0, 0, 255);

    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            sf::RectangleShape cell(sf::Vector2f(cellWidth, cellHeight));
            cell.setPosition(j * cellWidth, i * cellHeight);
            cell.setFillColor(cellColor);
            cell.setOutlineThickness(1);
            cell.setOutlineColor(lineColor);
            window.draw(cell);

            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(static_cast<unsigned int>(cellHeight * 0.8));
            text.setStyle(sf::Text::Bold);

            if (game.getBoardValue(i, j) == 'X') {
                text.setString("X");
                text.setFillColor(xColor);
            }
            else if (game.getBoardValue(i, j) == 'O') {
                text.setString("O");
                text.setFillColor(oColor);
            }

            sf::FloatRect textRect = text.getLocalBounds();
            text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
            text.setPosition(j * cellWidth + cellWidth / 2.0f, i * cellHeight + cellHeight / 2.0f);
            window.draw(text);
        }
    }

    for (int i = 1; i < boardSize; i++) {
        sf::Vertex line1[] = {
            sf::Vertex(sf::Vector2f(i * cellWidth, 0), lineColor),
            sf::Vertex(sf::Vector2f(i * cellWidth, windowSizeYF), lineColor)
        };
        sf::Vertex line2[] = {
            sf::Vertex(sf::Vector2f(0, i * cellHeight), lineColor),
            sf::Vertex(sf::Vector2f(windowSizeXF, i * cellHeight), lineColor)
        };
        window.draw(line1, 2, sf::Lines);
        window.draw(line2, 2, sf::Lines);
    }
}
sf::ConvexShape createRoundedRect(float width, float height, float radius, sf::Color color, float outlineThickness, sf::Color outlineColor) {
    sf::ConvexShape shape;
    shape.setPointCount(30); // Adjust the point count for smoother edges
    shape.setFillColor(color);
    shape.setOutlineThickness(outlineThickness);
    shape.setOutlineColor(outlineColor);

    // Define the points for the rounded rectangle
    if (shape.getPointCount() > 7) {
        shape.setPoint(0, sf::Vector2f(radius, 0));
        shape.setPoint(1, sf::Vector2f(width - radius, 0));
        shape.setPoint(2, sf::Vector2f(width, radius));
        shape.setPoint(3, sf::Vector2f(width, height - radius));
        shape.setPoint(4, sf::Vector2f(width - radius, height));
        shape.setPoint(5, sf::Vector2f(radius, height));
        shape.setPoint(6, sf::Vector2f(0, height - radius));
        shape.setPoint(7, sf::Vector2f(0, radius));
    }

    // Set positions for the rounded corners
    for (int i = 0; i < 8; ++i) {
        if (shape.getPointCount() > i + 8) {
            shape.setPoint(i + 8, shape.getPoint(i));
        }
        if (shape.getPointCount() > i + 16) {
            shape.setPoint(i + 16, shape.getPoint(i) + sf::Vector2f(outlineThickness, outlineThickness));
        }
    }

    // Set positions for the straight edges
    for (int i = 0; i < 4; ++i) {
        if (shape.getPointCount() > i + 24) {
            shape.setPoint(i + 24, shape.getPoint(i) + sf::Vector2f(0, outlineThickness));
        }
        if (shape.getPointCount() > i + 28) {
            shape.setPoint(i + 28, shape.getPoint(i + 4) + sf::Vector2f(0, -outlineThickness));
        }
    }

    // Adjust positions for the corners to round them
    for (int i = 0; i < 8; ++i) {
        if (shape.getPointCount() > i + 8) {
            sf::Vector2f point = shape.getPoint(i + 8);
            sf::Vector2f center = shape.getPoint(i);
            sf::Vector2f direction = point - center;
            direction /= std::sqrt(direction.x * direction.x + direction.y * direction.y);
            shape.setPoint(i + 8, center + direction * radius);
        }
    }

    return shape;
}
sf::Text createTextWithShadow(const std::string& str, const sf::Font& font, unsigned int size, sf::Color fillColor, sf::Color shadowColor, float offsetX, float offsetY) {
    sf::Text text(str, font, size);
    text.setFillColor(fillColor);
    sf::Text shadow(text);
    shadow.setFillColor(shadowColor);
    shadow.move(offsetX, offsetY);
    return text;
}

int main() {
    // Inicjalizacja
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tic Tac Toe");
    //window.setVerticalSyncEnabled(true);

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        return 1;
    }

    // Ustawienie stanu gry
    GameState gameState = MENU;
    Game game;

    // Ustawienia gry
    int boardSize = 3;
    int winLength = 3;

    // Tło
    sf::VertexArray background(sf::Quads, 4);
    background[0].position = sf::Vector2f(0, 0);
    background[1].position = sf::Vector2f(window.getSize().x, 0);
    background[2].position = sf::Vector2f(window.getSize().x, window.getSize().y);
    background[3].position = sf::Vector2f(0, window.getSize().y);
    background[0].color = sf::Color(0, 50, 50);   // Dark green base
    background[1].color = sf::Color(0, 70, 70);   // Adjusted shade
    background[2].color = sf::Color(0, 90, 90);   // Adjusted shade
    background[3].color = sf::Color(0, 110, 110); // Adjusted shade

    // Create buttons and text
    sf::Text title = createTextWithShadow("TIC TAC TOE", font, 58, sf::Color(255, 255, 255), sf::Color(0, 0, 0, 150), 2, 2);
    title.setStyle(sf::Text::Bold);
    title.setPosition(sf::Vector2f(window.getSize().x - window.getSize().x + 60, 80));
    sf::ConvexShape titleButtonUltra = createRoundedRect(350, 58, 5, sf::Color::Transparent, 2, sf::Color::Transparent);
    titleButtonUltra.setPosition(title.getPosition().x, title.getPosition().y);

    sf::ConvexShape startButton = createRoundedRect(200, 50, 10, sf::Color::White, 2, sf::Color::Black);
    startButton.setPosition(title.getPosition().x, 200);
    sf::Text startText("Start Game", font, 24);
    startText.setFillColor(sf::Color::Black);
    startText.setPosition(startButton.getPosition().x + 30, startButton.getPosition().y + 10);

    sf::ConvexShape exitButton = createRoundedRect(200, 50, 10, sf::Color::White, 2, sf::Color::Black);
    exitButton.setPosition(title.getPosition().x, 300);
    sf::Text exitText("Exit", font, 24);
    exitText.setFillColor(sf::Color::Black);
    exitText.setPosition(exitButton.getPosition().x + 85, exitButton.getPosition().y + 10);

    sf::ConvexShape optionsPanel = createRoundedRect(250, 500, 10, sf::Color(100, 100, 200, 150), 2, sf::Color::Black);
    optionsPanel.setPosition(window.getSize().x - 300, 50);


    sf::Text boardSizeText = createTextWithShadow("BOARD SIZE", font, 25, sf::Color::White, sf::Color::Black, 2, 2);
    boardSizeText.setPosition(optionsPanel.getPosition().x + 50, optionsPanel.getPosition().y + 35);

    // Increase board size button
    sf::ConvexShape increaseBoardSizeButton = createRoundedRect(40, 40, 5, sf::Color::White, 2, sf::Color::Black);
    increaseBoardSizeButton.setPosition(optionsPanel.getPosition().x + 180, optionsPanel.getPosition().y + 80);
    sf::Text increaseBoardSizeText = createTextWithShadow("+", font, 30, sf::Color::Black, sf::Color::White, 2, 2);
    increaseBoardSizeText.setPosition(increaseBoardSizeButton.getPosition().x + 10, increaseBoardSizeButton.getPosition().y);

    sf::Text boardSizeNumText = createTextWithShadow(std::to_string(boardSize), font, 48, sf::Color::White, sf::Color::Black, 2, 2);
    boardSizeNumText.setPosition(optionsPanel.getPosition().x + 120, optionsPanel.getPosition().y + 70);

    // Decrease board size button
    sf::ConvexShape decreaseBoardSizeButton = createRoundedRect(40, 40, 5, sf::Color::White, 2, sf::Color::Black);
    decreaseBoardSizeButton.setPosition(optionsPanel.getPosition().x + 40, optionsPanel.getPosition().y + 80);
    sf::Text decreaseBoardSizeText = createTextWithShadow("-", font, 30, sf::Color::Black, sf::Color::White, 2, 2);
    decreaseBoardSizeText.setPosition(decreaseBoardSizeButton.getPosition().x + 10, decreaseBoardSizeButton.getPosition().y);

    sf::Text winLengthText = createTextWithShadow("WIN LENGTH", font, 25, sf::Color::White, sf::Color::Black, 2, 2);
    winLengthText.setPosition(optionsPanel.getPosition().x + 50, optionsPanel.getPosition().y + 130);

    // Increase win length button
    sf::ConvexShape increaseWinLengthButton = createRoundedRect(40, 40, 5, sf::Color::White, 2, sf::Color::Black);
    increaseWinLengthButton.setPosition(optionsPanel.getPosition().x + 180, optionsPanel.getPosition().y + 170);
    sf::Text increaseWinLengthText = createTextWithShadow("+", font, 30, sf::Color::Black, sf::Color::White, 2, 2);
    increaseWinLengthText.setPosition(increaseWinLengthButton.getPosition().x + 10, increaseWinLengthButton.getPosition().y);

    sf::Text winLengthNumText = createTextWithShadow(std::to_string(winLength), font, 48, sf::Color::White, sf::Color::Black, 2, 2);
    winLengthNumText.setPosition(optionsPanel.getPosition().x + 120, optionsPanel.getPosition().y + 160);

    // Decrease win length button
    sf::ConvexShape decreaseWinLengthButton = createRoundedRect(40, 40, 5, sf::Color::White, 2, sf::Color::Black);
    decreaseWinLengthButton.setPosition(optionsPanel.getPosition().x + 40, optionsPanel.getPosition().y + 170);
    sf::Text decreaseWinLengthText = createTextWithShadow("-", font, 30, sf::Color::Black, sf::Color::White, 2, 2);
    decreaseWinLengthText.setPosition(decreaseWinLengthButton.getPosition().x + 10, decreaseWinLengthButton.getPosition().y);

    // Easy button
    sf::ConvexShape easyButton = createRoundedRect(200, 50, 5, sf::Color::White, 2, sf::Color::Black);
    easyButton.setPosition(optionsPanel.getPosition().x + 25, optionsPanel.getPosition().y + 240);
    sf::Text easyText = createTextWithShadow("Easy", font, 30, sf::Color::Black, sf::Color::White, 2, 2);
    easyText.setPosition(easyButton.getPosition().x + 75, easyButton.getPosition().y + 5);

    // Medium button
    sf::ConvexShape mediumButton = createRoundedRect(200, 50, 5, sf::Color::White, 2, sf::Color::Black);
    mediumButton.setPosition(optionsPanel.getPosition().x + 25, optionsPanel.getPosition().y + 300);
    sf::Text mediumText = createTextWithShadow("Medium", font, 30, sf::Color::Black, sf::Color::White, 2, 2);
    mediumText.setPosition(mediumButton.getPosition().x + 55, mediumButton.getPosition().y + 5);

    // Hard button
    sf::ConvexShape hardButton = createRoundedRect(200, 50, 5, sf::Color::White, 2, sf::Color::Black);
    hardButton.setPosition(optionsPanel.getPosition().x + 25, optionsPanel.getPosition().y + 360);
    sf::Text hardText = createTextWithShadow("Hard", font, 30, sf::Color::Black, sf::Color::White, 2, 2);
    hardText.setPosition(hardButton.getPosition().x + 75, hardButton.getPosition().y + 5);

    // Teksty końcowe
    sf::Text winPlayer;
    winPlayer.setFont(font);
    winPlayer.setString("Player X has won!");
    winPlayer.setCharacterSize(40); // Increase font size for emphasis
    winPlayer.setFillColor(sf::Color(255, 69, 0)); // Orange-red color for a vibrant look
    winPlayer.setStyle(sf::Text::Bold);
    winPlayer.setLetterSpacing(1);
    winPlayer.setOutlineColor(sf::Color::Black);
    winPlayer.setOutlineThickness(2);
    winPlayer.setPosition(window.getSize().x / 2 - 150, window.getSize().y / 2 - 50);

    sf::Text winAI;
	winAI.setFont(font);
	winAI.setString("AI has won!");
	winAI.setCharacterSize(40); // Increase font size for emphasis
	winAI.setFillColor(sf::Color(0, 191, 255)); // Deep sky blue color for a vibrant look
	winAI.setStyle(sf::Text::Bold);
	winAI.setLetterSpacing(1);
	winAI.setOutlineColor(sf::Color::Black);
	winAI.setOutlineThickness(2);
	winAI.setPosition(window.getSize().x / 2 - 150, window.getSize().y / 2 - 50);

	sf::Text tie;
	tie.setFont(font);
	tie.setString("It's a tie!");
	tie.setCharacterSize(40); // Increase font size for emphasis
	tie.setFillColor(sf::Color(255, 255, 0)); // Yellow color for a vibrant look
	tie.setStyle(sf::Text::Bold);
	tie.setLetterSpacing(1);
	tie.setOutlineColor(sf::Color::Black);
	tie.setOutlineThickness(2);
	tie.setPosition(window.getSize().x / 2 - 100, window.getSize().y / 2 - 50);

	// Monit o grze
	sf::Text enterPrompt = createTextWithShadow("Press Enter to start the game", font, 40, sf::Color::White, sf::Color::Transparent, 2, 2);
	enterPrompt.setStyle(sf::Text::Bold | sf::Text::Underlined);
	enterPrompt.setPosition(window.getSize().x / 2 - 250, window.getSize().y / 2 - 50);
    enterPrompt.setStyle(sf::Text::Bold);
    enterPrompt.setLetterSpacing(1);
    enterPrompt.setOutlineColor(sf::Color::Black);
    enterPrompt.setOutlineThickness(4);

    sf::Text retryPrompt;
    retryPrompt.setFont(font);
    retryPrompt.setString("Press R to play again");
    retryPrompt.setCharacterSize(24);
    retryPrompt.setFillColor(sf::Color::White);
	retryPrompt.setPosition(winPlayer.getPosition().x, winPlayer.getPosition().y + 70);
    retryPrompt.setOutlineColor(sf::Color::Black);
    retryPrompt.setOutlineThickness(2);

    sf::Text menuPrompt;
    menuPrompt.setFont(font);
    menuPrompt.setString("Press Enter to return to menu");
    menuPrompt.setCharacterSize(24);
    menuPrompt.setFillColor(sf::Color::White);
    menuPrompt.setPosition(winPlayer.getPosition().x, winPlayer.getPosition().y + 100);
    menuPrompt.setOutlineColor(sf::Color::Black);
    menuPrompt.setOutlineThickness(2);

    int loop = true;
	int counter = 0;

    while (window.isOpen())
    {
		// Sprawdzanie zdarzeń
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                if (startButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && gameState == MENU) {
                    gameState = WAITING_TO_PLAY;
                }

                if (exitButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && gameState == MENU) {
                    window.close();
                }

                if (gameState == MENU) {
                    if (increaseBoardSizeButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        boardSize++;
                        game.setBoard(boardSize);
                        boardSizeNumText.setString(std::to_string(boardSize));
                    }
                    if (decreaseBoardSizeButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        if (boardSize > 3) {
                            boardSize--;
                            game.setBoard(boardSize);
                            boardSizeNumText.setString(std::to_string(boardSize));
                        }
                    }
                    if (increaseWinLengthButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        if (winLength < boardSize) {
                            winLength++;
                            game.setWinLength(winLength);
                            winLengthNumText.setString(std::to_string(winLength));
                        }
                    }
                    if (decreaseWinLengthButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        if (winLength > 3) {
                            winLength--;
                            game.setWinLength(winLength);
                            winLengthNumText.setString(std::to_string(winLength)); 
                        }
                    }
                    if (easyButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
						mediumButton.setFillColor(sf::Color::White);
						hardButton.setFillColor(sf::Color::White);
                        aiDifficulty = EASY;
                        easyButton.setFillColor(sf::Color::Green);
                    }
                    if (mediumButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
						easyButton.setFillColor(sf::Color::White);
						hardButton.setFillColor(sf::Color::White);
                        aiDifficulty = MEDIUM;
                        mediumButton.setFillColor(sf::Color::Green);
                    }
                    if (hardButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
						easyButton.setFillColor(sf::Color::White);
						mediumButton.setFillColor(sf::Color::White);
                        aiDifficulty = HARD;
                        hardButton.setFillColor(sf::Color::Green);
                    }
                    if (titleButtonUltra.getGlobalBounds().contains(mousePos.x, mousePos.y) && gameState == MENU) {
                        easyButton.setFillColor(sf::Color::White);
                        mediumButton.setFillColor(sf::Color::White);
						hardButton.setFillColor(sf::Color::White);
						aiDifficulty = ULTRA;
                    }
                }

            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    if (gameState == WAITING_TO_PLAY) {
                        gameState = PLAYING;
                    }
                    else if (gameState == GAME_OVER) {
						game.resetBoard();
                        gameState = MENU;
                    }
                }
                else if (event.key.code == sf::Keyboard::R) {
                    if (gameState == GAME_OVER) {
                        game.resetBoard();
                        game.setBoard(boardSize);
                        game.setWinLength(winLength);
                        gameState = PLAYING;
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (gameState == WAITING_TO_PLAY) {
                    gameState = PLAYING;
                }
                else if (gameState == GAME_OVER) {
                    game.resetBoard();
                    game.setBoard(boardSize);
                    game.setWinLength(winLength);
                    gameState = PLAYING;
                }
            }
        }

        if (gameState == WAITING_TO_PLAY) {
            window.draw(enterPrompt);
        }
        else if (gameState == MENU) {
            window.clear();

            // Rysowanie
            window.draw(background);
            window.draw(title);
            window.draw(titleButtonUltra);
            window.draw(startButton);
            window.draw(startText);
            window.draw(exitButton);
            window.draw(exitText);
            window.draw(optionsPanel);
            window.draw(increaseBoardSizeButton);
            window.draw(increaseBoardSizeText);
            window.draw(boardSizeText);
            window.draw(boardSizeNumText);
            window.draw(decreaseBoardSizeButton);
            window.draw(decreaseBoardSizeText);
            window.draw(increaseWinLengthButton);
            window.draw(increaseWinLengthText);
            window.draw(winLengthText);
            window.draw(winLengthNumText);
            window.draw(decreaseWinLengthButton);
            window.draw(decreaseWinLengthText);
            window.draw(easyButton);
            window.draw(easyText);
            window.draw(mediumButton);
            window.draw(mediumText);
            window.draw(hardButton);
            window.draw(hardText);

        }
        else if (gameState == PLAYING || gameState == WAITING_FOR_AI) {
            window.clear(sf::Color(50, 50, 50));
			drawBoard(window, game, font, boardSize);

            // Obsługa zdarzeń myszy
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                int row = mousePos.y / (window.getSize().y / game.getBoardSize());
                int col = mousePos.x / (window.getSize().x / game.getBoardSize());
                bool validMove = false;
            
                while (!validMove) {
                    mousePos = sf::Mouse::getPosition(window);
                    row = mousePos.y / (window.getSize().y / game.getBoardSize());
                    col = mousePos.x / (window.getSize().x / game.getBoardSize());
            
                    validMove = game.playerMove(row, col);
            
                    window.clear();
                    drawBoard(window, game, font, boardSize);
                    window.display();
            
                    if (!validMove) {
                        sf::Event newEvent;
                        bool eventReceived = false;
                        while (!eventReceived) {
                            while (window.pollEvent(newEvent)) {
                                if (newEvent.type == sf::Event::MouseButtonPressed && newEvent.mouseButton.button == sf::Mouse::Left) {
                                    eventReceived = true;
                                    break;
                                }
                            }
                        }
                    }
                }
            
                char winner = game.checkWin('X') ? 'X' : game.checkWin('O') ? 'O' : ' ';
                if (winner != ' ' || game.isBoardFull()) {
                    gameState = GAME_OVER;
                }
                else {
                    gameState = WAITING_FOR_AI;
                }
            
                if (gameState == WAITING_FOR_AI && game.aiMove(aiDifficulty)) {
                    char winner = game.checkWin('X') ? 'X' : game.checkWin('O') ? 'O' : ' ';
                    if (winner != ' ' || game.isBoardFull()) {
                        gameState = GAME_OVER;
                    }
                    else {
                        gameState = PLAYING;
                    }
                }

                //game.printStats(aiDifficulty);

     //           if (game.ai2Move(ULTRA)) {
     //               char winner = game.checkWin('X') ? 'X' : game.checkWin('O') ? 'O' : ' ';
     //               if (winner != ' ' || game.isBoardFull()) {
					//	counter++;
					//	game.resetBoard();
     //                   //gameState = GAME_OVER;
     //               }
     //               else {
     //                   gameState = WAITING_FOR_AI;
     //               }
     //           }
     //           //sf::sleep(sf::milliseconds(2000));

     //           if (gameState == WAITING_FOR_AI && game.aiMove(aiDifficulty)) {
     //               char winner = game.checkWin('X') ? 'X' : game.checkWin('O') ? 'O' : ' ';
     //               if (winner != ' ' || game.isBoardFull()) {
     //                   counter++;
     //                   game.resetBoard();
     //                   //gameState = GAME_OVER;
     //               }
     //               else {
     //                   gameState = PLAYING;
     //               }
     //           }

     //           game.printStats(aiDifficulty);

     //           if (counter >= 500) {
					//gameState = GAME_OVER;
     //           }

            }
        }
        else if (gameState == GAME_OVER) {
            // Check if it's a tie
            if (game.isBoardFull() && !game.checkWin('X') && !game.checkWin('O')) {
                window.draw(tie);
            }
            else {
                // Determine the winner
                if (game.getLastPlayer() == 'X') {
                    window.draw(winPlayer);
                    sf::Text playerMoves;
                    playerMoves.setFont(font);
                    playerMoves.setString("Player moves: " + std::to_string(game.getPlayerMoves()));
                    playerMoves.setCharacterSize(24);
                    playerMoves.setFillColor(sf::Color::White);
					playerMoves.setPosition(menuPrompt.getPosition().x, menuPrompt.getPosition().y + 50);
                    window.draw(playerMoves);
                }
                else if (game.getLastPlayer() == 'O') {
                    window.draw(winAI);
                    sf::Text aiMoves;
                    aiMoves.setFont(font);
                    aiMoves.setString("AI moves: " + std::to_string(game.getAIMoves()));
                    aiMoves.setCharacterSize(24);
                    aiMoves.setFillColor(sf::Color::White);
                    aiMoves.setPosition(menuPrompt.getPosition().x, menuPrompt.getPosition().y + 50);
                    window.draw(aiMoves);
                }
            }
            window.draw(retryPrompt);
            window.draw(menuPrompt);
        }
        window.display();
    }

    return 0;
}