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

void handleOptionsMenu(sf::RenderWindow& window, sf::Clock& clock, sf::RectangleShape& optionsPanel, float optionsPanelTargetX, float optionsPanelSpeed, bool& showOptions) {
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
}
void handleGameEvents(sf::RenderWindow& window, sf::Event& event, GameState& gameState, TicTacToe& game,
    sf::Text& introText, sf::ConvexShape& startButton, sf::Text& startText,
    sf::ConvexShape& optionsButton, sf::Text& optionsText,
    sf::ConvexShape& exitButton, sf::Text& exitText,
    sf::Text& enterPrompt, sf::RectangleShape& optionsPanel,
    sf::RectangleShape& increaseBoardSizeButton, sf::Text& increaseBoardSizeText,
    sf::RectangleShape& decreaseBoardSizeButton, sf::Text& decreaseBoardSizeText,
    sf::RectangleShape& increaseLengthSizeButton, sf::Text& increaseLengthSizeText,
    sf::RectangleShape& decreaseLengthSizeButton, sf::Text& decreaseLengthSizeText,
    sf::RectangleShape& easyButton, sf::Text& easyText,
    sf::RectangleShape& mediumButton, sf::Text& mediumText,
    sf::RectangleShape& hardButton, sf::Text& hardText,
    bool& showOptions, int& boardSize, int& winLength) {

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
void handleGameState(sf::RenderWindow& window, sf::Event& event, GameState& gameState, TicTacToe& game,
    sf::Text& enterPrompt, sf::Text& title,
    sf::ConvexShape& startButton, sf::Text& startText,
    sf::ConvexShape& optionsButton, sf::Text& optionsText,
    sf::ConvexShape& exitButton, sf::Text& exitText,
    sf::RectangleShape& optionsPanel,
    sf::RectangleShape& increaseBoardSizeButton, sf::Text& increaseBoardSizeText,
    sf::RectangleShape& decreaseBoardSizeButton, sf::Text& decreaseBoardSizeText,
    sf::RectangleShape& increaseLengthSizeButton, sf::Text& increaseLengthSizeText,
    sf::RectangleShape& decreaseLengthSizeButton, sf::Text& decreaseLengthSizeText,
    sf::RectangleShape& easyButton, sf::Text& easyText,
    sf::RectangleShape& mediumButton, sf::Text& mediumText,
    sf::RectangleShape& hardButton, sf::Text& hardText,
    sf::Text& winPlayer, sf::Text& winAI, sf::Text& tie,
    int boardSize, int winLength, sf::Font& font) {

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
    }
    else if (gameState == OPTIONS) {
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
        float boardSizeF = static_cast<float>(boardSize);
        float windowSizeXF = static_cast<float>(window.getSize().x);
        float windowSizeYF = static_cast<float>(window.getSize().y);

        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                sf::RectangleShape cell(sf::Vector2f(windowSizeXF / boardSizeF, windowSizeYF / boardSizeF));
                cell.setPosition(j * (windowSizeXF / boardSizeF), i * (windowSizeYF / boardSizeF));
                window.draw(cell);

                if (game.getBoardValue(i, j) == 'X') {
                    sf::Text x;
                    x.setFont(font);
                    x.setString("X");
                    x.setCharacterSize(24);
                    x.setFillColor(sf::Color::Red);
                    x.setPosition(j * (windowSizeXF / boardSizeF) + windowSizeXF / (boardSizeF * 2), i * (windowSizeYF / boardSizeF) + windowSizeYF / (boardSizeF * 2));
                    window.draw(x);
                }
                else if (game.getBoardValue(i, j) == 'O') {
                    sf::Text o;
                    o.setFont(font);
                    o.setString("O");
                    o.setCharacterSize(24);
                    o.setFillColor(sf::Color::Blue);
                    o.setPosition(j * (windowSizeXF / boardSizeF) + windowSizeXF / (boardSizeF * 2), i * (windowSizeYF / boardSizeF) + windowSizeYF / (boardSizeF * 2));
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
}
void handleGameLoop(sf::RenderWindow& window, sf::Clock& clock, sf::RectangleShape& optionsPanel, float optionsPanelTargetX, float optionsPanelSpeed, GameState& gameState, TicTacToe& game, bool& showOptions, int& boardSize, int& winLength,
    sf::Text& enterPrompt, sf::Text& title,
    sf::ConvexShape& startButton, sf::Text& startText,
    sf::ConvexShape& optionsButton, sf::Text& optionsText,
    sf::ConvexShape& exitButton, sf::Text& exitText,
    sf::RectangleShape& increaseBoardSizeButton, sf::Text& increaseBoardSizeText,
    sf::RectangleShape& decreaseBoardSizeButton, sf::Text& decreaseBoardSizeText,
    sf::RectangleShape& increaseLengthSizeButton, sf::Text& increaseLengthSizeText,
    sf::RectangleShape& decreaseLengthSizeButton, sf::Text& decreaseLengthSizeText,
    sf::RectangleShape& easyButton, sf::Text& easyText,
    sf::RectangleShape& mediumButton, sf::Text& mediumText,
    sf::RectangleShape& hardButton, sf::Text& hardText,
    sf::Text& winPlayer, sf::Text& winAI, sf::Text& tie,
    sf::Font& font) {

    handleOptionsMenu(window, clock, optionsPanel, optionsPanelTargetX, optionsPanelSpeed, showOptions);

    sf::Event event;
    while (window.pollEvent(event)) {
        handleGameEvents(window, event, gameState, game, title, startButton, startText, optionsButton, optionsText, exitButton, exitText, enterPrompt, optionsPanel, increaseBoardSizeButton, increaseBoardSizeText, decreaseBoardSizeButton, decreaseBoardSizeText, increaseLengthSizeButton, increaseLengthSizeText, decreaseLengthSizeButton, decreaseLengthSizeText, easyButton, easyText, mediumButton, mediumText, hardButton, hardText, showOptions, boardSize, winLength);
    }

    handleGameState(window, event, gameState, game, enterPrompt, title, startButton, startText, optionsButton, optionsText, exitButton, exitText, optionsPanel, increaseBoardSizeButton, increaseBoardSizeText, decreaseBoardSizeButton, decreaseBoardSizeText, increaseLengthSizeButton, increaseLengthSizeText, decreaseLengthSizeButton, decreaseLengthSizeText, easyButton, easyText, mediumButton, mediumText, hardButton, hardText, winPlayer, winAI, tie, boardSize, winLength, font);
    
    window.display();
}



int main() {
    // Inicjalizacja
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tic Tac Toe");
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        return 1;
    }

    // Tworzenie elementów interfejsu
    // Tworzenie tytułu
    sf::Text title = createTitle(font, window);
    sf::Text introText = createIntroText(font, window);

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

    sf::RectangleShape increaseLengthSizeButton = createIncreaseLengthSizeButton(window);
    sf::Text increaseLengthSizeText = createIncreaseLengthSizeText(font, increaseLengthSizeButton);

    sf::RectangleShape decreaseLengthSizeButton = createDecreaseLengthSizeButton(window);
    sf::Text decreaseLengthSizeText = createDecreaseLengthSizeText(font, decreaseLengthSizeButton);

    // Tworzenie przycisków do zmiany poziomu trudności
    sf::RectangleShape easyButton = createEasyButton(window);
    sf::Text easyText = createEasyText(font, easyButton);

    sf::RectangleShape mediumButton = createMediumButton(window);
    sf::Text mediumText = createMediumText(font, mediumButton);

    sf::RectangleShape hardButton = createHardButton(window);
    sf::Text hardText = createHardText(font, hardButton);

    // Teksty końcowe
    sf::Text winPlayer = createWinPlayerText(font);
    sf::Text winAI = createWinAIText(font);
    sf::Text tie = createTieText(font);

    // Ustawienia gry
    bool showOptions = false;
    int boardSize = 3;
    int winLength = 3;

    // Inicjalizacja zegara
    sf::Clock clock;
    float optionsPanelTargetX = window.getSize().x - 200;
    float optionsPanelSpeed = 200.0f;

    // Ustawienie stanu gry
    GameState gameState = INTRO;
    TicTacToe game;

    while (window.isOpen())
    {
        handleGameLoop(window, clock, optionsPanel, optionsPanelTargetX, optionsPanelSpeed, gameState, game, 
            showOptions, boardSize, winLength, enterPrompt, title, startButton, startText, optionsButton, 
            optionsText, exitButton, exitText, increaseBoardSizeButton, increaseBoardSizeText,
            decreaseBoardSizeButton, decreaseBoardSizeText, increaseLengthSizeButton, increaseLengthSizeText, 
            decreaseLengthSizeButton, decreaseLengthSizeText,
            easyButton, easyText, mediumButton, mediumText, hardButton, hardText, winPlayer, winAI, tie, font);
    }

    return 0;
}