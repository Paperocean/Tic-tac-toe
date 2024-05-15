#include <iostream>
#include <vector>
#include <climits>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include "tictactoe.h"
#include "ui_helpers.h"

using namespace std;

Difficulty aiDifficulty = EASY;
enum GameState {
    INTRO,
    MENU,
    PLAYING,
    OPTIONS,
    HIDE_OPTIONS,
    WAITING_FOR_ENTER
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tic Tac Toe");
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
		std::cerr << "Failed to load font" << std::endl;
		return 1;
    }

    // Tworzenie tytułu
    sf::Text title = createTitle(font, window);

    // Tworzenie monitu o naciśnięciu Enter
    sf::Text enterPrompt = createEnterPrompt(font);

    // Tworzenie przycisków
    sf::ConvexShape startButton = createStartButton();
    sf::Text startText = createStartText(font, startButton);

    sf::ConvexShape optionsButton = createOptionsButton();
    sf::Text optionsText = createOptionsText(font, optionsButton);

    sf::ConvexShape exitButton = createExitButton();
    sf::Text exitText = createExitText(font, exitButton);

    // Tworzenie panelu opcji
    sf::RectangleShape optionsPanel = createOptionsPanel(window);

    // Tworzenie przycisków do zmiany rozmiaru planszy
    sf::RectangleShape increaseBoardSizeButton = createIncreaseBoardSizeButton(window);
    sf::Text increaseBoardSizeText = createIncreaseBoardSizeText(font, increaseBoardSizeButton);

    sf::RectangleShape decreaseBoardSizeButton = createDecreaseBoardSizeButton(window);
    sf::Text decreaseBoardSizeText = createDecreaseBoardSizeText(font, decreaseBoardSizeButton);


    bool showOptions = false;
    int boardSize = 3;
    int winLength = 3;

    sf::Clock clock;
    float optionsPanelTargetX = window.getSize().x - 200;
    float optionsPanelSpeed = 200.0f;


    GameState gameState = INTRO;
    TicTacToe game;

    while (window.isOpen())
    {
        sf::Time dt = clock.restart();

        if (showOptions) {
            if (optionsPanel.getPosition().x > optionsPanelTargetX) {
                float newX = optionsPanel.getPosition().x - optionsPanelSpeed * dt.asSeconds();
                if (newX < optionsPanelTargetX) {
                    newX = optionsPanelTargetX;
                }
                optionsPanel.setPosition(newX, optionsPanel.getPosition().y);
            }
        }
        else {
            if (optionsPanel.getPosition().x < window.getSize().x) {
                float newX = optionsPanel.getPosition().x + optionsPanelSpeed * dt.asSeconds();
                if (newX > window.getSize().x) {
                    newX = window.getSize().x;
                }
                optionsPanel.setPosition(newX, optionsPanel.getPosition().y);
            }
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (gameState == INTRO) {
                window.clear(sf::Color(50, 50, 50));
                window.draw(introText);
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Enter) {
                        gameState = MENU;
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (startButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    if (gameState == MENU) {
                        game.setBoard(boardSize);
                        game.setWinLength(winLength);
                        gameState = WAITING_FOR_ENTER;
                    }
                }
                if (optionsButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && gameState != PLAYING) {
                    if (gameState == MENU) {
                        gameState = OPTIONS;
						showOptions = true;
                    }
                    else if (gameState == OPTIONS) {
                        gameState = HIDE_OPTIONS;
						showOptions = false;
                    }
                    else if (gameState == HIDE_OPTIONS) {
                        gameState = OPTIONS;
						showOptions = true;
                    }
                }
                if (exitButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Enter && gameState == WAITING_FOR_ENTER) {
                        gameState = PLAYING;
                    }
                }
                if (showOptions) {
                    if (increaseBoardSizeButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        boardSize++;
                    }
                    if (decreaseBoardSizeButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        if (boardSize > 3) {
                            boardSize--;
                        }
                    }
                    if (increaseLengthSizeButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        if (winLength < boardSize)
                            winLength++;
                    }
                    if (decreaseLengthSizeButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        if (winLength > 3) {
                            winLength--;
                        }
                    }
                    if (easyButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        aiDifficulty = EASY;
                        easyButton.setFillColor(sf::Color::Green);
                        mediumButton.setFillColor(sf::Color::White);
                        hardButton.setFillColor(sf::Color::White);
                    }
                    else if (mediumButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        aiDifficulty = MEDIUM;
                        easyButton.setFillColor(sf::Color::White);
                        mediumButton.setFillColor(sf::Color::Green);
                        hardButton.setFillColor(sf::Color::White);
                    }
                    else if (hardButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        aiDifficulty = HARD;
                        easyButton.setFillColor(sf::Color::White);
                        mediumButton.setFillColor(sf::Color::White);
                        hardButton.setFillColor(sf::Color::Green);
                    }
                }
            }
        }

        if (gameState == WAITING_FOR_ENTER) {
            window.draw(enterPrompt);
        }

        if (gameState == MENU) {
            window.clear(sf::Color(50, 50, 50));
            window.draw(title);
            startButton.setFillColor(sf::Color::White);
            window.draw(startButton);
            window.draw(startText);
            window.draw(optionsButton);
            window.draw(optionsText);
            window.draw(exitButton);
            window.draw(exitText);
        }else if (gameState == OPTIONS) {
            window.draw(optionsPanel);
            sf::Text dialogText;
            dialogText.setFont(font);
            dialogText.setString("Board size: " + std::to_string(boardSize) + "\nWin length: " + std::to_string(winLength));
            dialogText.setCharacterSize(24);
            dialogText.setFillColor(sf::Color::White);
            dialogText.setPosition(window.getSize().x - 180, 80);
            window.draw(dialogText);

            window.draw(increaseBoardSizeButton);
            window.draw(increaseBoardSizeText);
            window.draw(decreaseBoardSizeButton);
            window.draw(decreaseBoardSizeText);
            window.draw(increaseLengthSizeButton);
            window.draw(increaseLengthSizeText);
            window.draw(decreaseLengthSizeButton);
            window.draw(decreaseLengthSizeText);

            window.draw(easyButton);
            window.draw(easyText);
			window.draw(mediumButton);
			window.draw(mediumText);
			window.draw(hardButton);
			window.draw(hardText);
        }
        else if (gameState == PLAYING) {

            // Rysuj planszę gry
            for (int i = 0; i < boardSize; i++) {
                for (int j = 0; j < boardSize; j++) {
                    sf::RectangleShape cell(sf::Vector2f(window.getSize().x / boardSize, window.getSize().y / boardSize));
                    cell.setPosition(j * (window.getSize().x / boardSize), i * (window.getSize().y / boardSize));
                    window.draw(cell);

                    if (game.getBoardValue(i, j) == 'X') {
                        sf::Text x;
                        x.setFont(font);
                        x.setString("X");
                        x.setCharacterSize(24);
                        x.setFillColor(sf::Color::Red);
                        x.setPosition(j * (window.getSize().x / boardSize) + window.getSize().x / (boardSize * 2), i * (window.getSize().y / boardSize) + window.getSize().y / (boardSize * 2));
                        window.draw(x);
                    }
                    else if (game.getBoardValue(i, j) == 'O') {
                        sf::Text o;
                        o.setFont(font);
                        o.setString("O");
                        o.setCharacterSize(24);
                        o.setFillColor(sf::Color::Blue);
                        o.setPosition(j * (window.getSize().x / boardSize) + window.getSize().x / (boardSize * 2), i * (window.getSize().y / boardSize) + window.getSize().y / (boardSize * 2));
                        window.draw(o);
                    }
                }
            }

            // Obsługa zdarzeń myszy
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                int row = mousePos.y / (window.getSize().y / boardSize);
                int col = mousePos.x / (window.getSize().x / boardSize);
                if (game.makeMove(row, col)) {
                    if (game.checkWin()) {
                        window.draw(winPlayer);
                        gameState = MENU;
                    }
                    else if (game.isBoardFull()) {
                        window.draw(tie);
                        gameState = MENU;
                    }
                    else {
                        game.aiMove(EASY);
                        if (game.checkWin()) {
                            window.draw(winAI);
                            gameState = MENU;
                        }
                        else if (game.isBoardFull()) {
                            window.draw(tie);
                            gameState = MENU;
                        }
                    }
                }
            }
        }

        window.display();
    }

    return 0;
}