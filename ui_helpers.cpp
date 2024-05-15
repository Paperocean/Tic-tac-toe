#include "ui_helpers.h"
sf::ConvexShape createRoundedRectangle(float width, float height, float radius)
{
    sf::ConvexShape shape;
    shape.setPointCount(8);

    shape.setPoint(0, sf::Vector2f(radius, 0));
    shape.setPoint(1, sf::Vector2f(width - radius, 0));
    shape.setPoint(2, sf::Vector2f(width, radius));
    shape.setPoint(3, sf::Vector2f(width, height - radius));
    shape.setPoint(4, sf::Vector2f(width - radius, height));
    shape.setPoint(5, sf::Vector2f(radius, height));
    shape.setPoint(6, sf::Vector2f(0, height - radius));
    shape.setPoint(7, sf::Vector2f(0, radius));

    return shape;
}
sf::Text createTitle(sf::Font& font, sf::Window& window) {
    sf::Text title;
    title.setFont(font);
    title.setString("Tic Tac Toe");
    title.setCharacterSize(40);
    title.setFillColor(sf::Color::Cyan);
    title.setStyle(sf::Text::Bold);
    sf::FloatRect textRect = title.getLocalBounds();
    title.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    title.setPosition(sf::Vector2f(window.getSize().x / 2.0f, 100));
	return title;
}

sf::Text createEnterPrompt(sf::Font& font) {
	sf::Text enterPrompt;
	enterPrompt.setFont(font);
	enterPrompt.setString("Press Enter to start the game");
	enterPrompt.setCharacterSize(24);
	enterPrompt.setFillColor(sf::Color::White);
	enterPrompt.setPosition(50, 50);
	return enterPrompt;
}

sf::ConvexShape createStartButton() {
	sf::ConvexShape startButton = createRoundedRectangle(220, 70, 15);
	startButton.setPosition(290, 190);
	return startButton;
}
sf::Text createStartText(sf::Font& font, sf::ConvexShape& startButton) {
	sf::Text startText;
	startText.setFont(font);
	startText.setString("Start");
	startText.setCharacterSize(30);
	startText.setFillColor(sf::Color::Black);
	sf::FloatRect textRect = startText.getLocalBounds();
	startText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	startText.setPosition(sf::Vector2f(startButton.getPosition().x + startButton.getGlobalBounds().width / 2.0f, startButton.getPosition().y + startButton.getGlobalBounds().height / 2.0f));
	return startText;
}

sf::ConvexShape createOptionsButton() {
	sf::ConvexShape optionsButton = createRoundedRectangle(200, 50, 15);
	optionsButton.setPosition(300, 300);
	return optionsButton;
}
sf::Text createOptionsText(sf::Font& font, sf::ConvexShape& optionsButton) {
	sf::Text optionsText;
	optionsText.setFont(font);
	optionsText.setString("Options");
	optionsText.setCharacterSize(24);
	optionsText.setFillColor(sf::Color::Black);
	sf::FloatRect textRect = optionsText.getLocalBounds();
	optionsText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	optionsText.setPosition(sf::Vector2f(optionsButton.getPosition().x + optionsButton.getGlobalBounds().width / 2.0f, optionsButton.getPosition().y + optionsButton.getGlobalBounds().height / 2.0f));
	return optionsText;
}

sf::ConvexShape createExitButton() {
	sf::ConvexShape exitButton = createRoundedRectangle(200, 50, 15);
	exitButton.setPosition(300, 400);
	return exitButton;
}
sf::Text createExitText(sf::Font& font, sf::ConvexShape& exitButton) {
	sf::Text exitText;
	exitText.setFont(font);
	exitText.setString("Exit");
	exitText.setCharacterSize(24);
	exitText.setFillColor(sf::Color::Black);
	sf::FloatRect textRect = exitText.getLocalBounds();
	exitText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	exitText.setPosition(sf::Vector2f(exitButton.getPosition().x + exitButton.getGlobalBounds().width / 2.0f, exitButton.getPosition().y + exitButton.getGlobalBounds().height / 2.0f));
	return exitText;
}

sf::RectangleShape createOptionsPanel(sf::Window& window) {
	sf::RectangleShape optionsPanel(sf::Vector2f(200, window.getSize().y));
	optionsPanel.setPosition(window.getSize().x - 200, 0);
	optionsPanel.setFillColor(sf::Color(70, 70, 70));
	return optionsPanel;
}

sf::RectangleShape createIncreaseBoardSizeButton(sf::Window& window) {
	sf::RectangleShape increaseBoardSizeButton(sf::Vector2f(50, 50));
	increaseBoardSizeButton.setPosition(window.getSize().x - 150, 150);
	return increaseBoardSizeButton;
}
sf::Text createIncreaseBoardSizeText(sf::Font& font, sf::RectangleShape& increaseBoardSizeButton) {
	sf::Text increaseBoardSizeText;
	increaseBoardSizeText.setFont(font);
	increaseBoardSizeText.setString("+");
	increaseBoardSizeText.setCharacterSize(24);
	increaseBoardSizeText.setFillColor(sf::Color::Black);
	sf::FloatRect textRect = increaseBoardSizeText.getLocalBounds();
	increaseBoardSizeText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	increaseBoardSizeText.setPosition(sf::Vector2f(increaseBoardSizeButton.getPosition().x + increaseBoardSizeButton.getGlobalBounds().width / 2.0f, increaseBoardSizeButton.getPosition().y + increaseBoardSizeButton.getGlobalBounds().height / 2.0f));
	return increaseBoardSizeText;
}

sf::RectangleShape createDecreaseBoardSizeButton(sf::Window& window) {
	sf::RectangleShape decreaseBoardSizeButton(sf::Vector2f(50, 50));
	decreaseBoardSizeButton.setPosition(window.getSize().x - 100, 150);
	return decreaseBoardSizeButton;
}
sf::Text createDecreaseBoardSizeText(sf::Font& font, sf::RectangleShape& decreaseBoardSizeButton) {
	sf::Text decreaseBoardSizeText;
	decreaseBoardSizeText.setFont(font);
	decreaseBoardSizeText.setString("-");
	decreaseBoardSizeText.setCharacterSize(24);
	decreaseBoardSizeText.setFillColor(sf::Color::Black);
	sf::FloatRect textRect = decreaseBoardSizeText.getLocalBounds();
	decreaseBoardSizeText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	decreaseBoardSizeText.setPosition(sf::Vector2f(decreaseBoardSizeButton.getPosition().x + decreaseBoardSizeButton.getGlobalBounds().width / 2.0f, decreaseBoardSizeButton.getPosition().y + decreaseBoardSizeButton.getGlobalBounds().height / 2.0f));
	return decreaseBoardSizeText;
}

sf::RectangleShape createIncreaseLengthSizeButton(sf::Window& window) {
	sf::RectangleShape increaseLengthSizeButton(sf::Vector2f(50, 50));
	increaseLengthSizeButton.setPosition(window.getSize().x - 150, 200);
	return increaseLengthSizeButton;
}
sf::Text createIncreaseLengthSizeText(sf::Font& font, sf::RectangleShape& increaseLengthSizeButton) {
	sf::Text increaseLengthSizeText;
	increaseLengthSizeText.setFont(font);
	increaseLengthSizeText.setString("+");
	increaseLengthSizeText.setCharacterSize(24);
	increaseLengthSizeText.setFillColor(sf::Color::Black);
	sf::FloatRect textRect = increaseLengthSizeText.getLocalBounds();
	increaseLengthSizeText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	increaseLengthSizeText.setPosition(sf::Vector2f(increaseLengthSizeButton.getPosition().x + increaseLengthSizeButton.getGlobalBounds().width / 2.0f, increaseLengthSizeButton.getPosition().y + increaseLengthSizeButton.getGlobalBounds().height / 2.0f));
	return increaseLengthSizeText;
}

sf::RectangleShape createDecreaseLengthSizeButton(sf::Window& window) {
	sf::RectangleShape decreaseLengthSizeButton(sf::Vector2f(50, 50));
	decreaseLengthSizeButton.setPosition(window.getSize().x - 100, 200);
	return decreaseLengthSizeButton;
}
sf::Text createDecreaseLengthSizeText(sf::Font& font, sf::RectangleShape& decreaseLengthSizeButton) {
	sf::Text decreaseLengthSizeText;
	decreaseLengthSizeText.setFont(font);
	decreaseLengthSizeText.setString("-");
	decreaseLengthSizeText.setCharacterSize(24);
	decreaseLengthSizeText.setFillColor(sf::Color::Black);
	sf::FloatRect textRect = decreaseLengthSizeText.getLocalBounds();
	decreaseLengthSizeText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	decreaseLengthSizeText.setPosition(sf::Vector2f(decreaseLengthSizeButton.getPosition().x + decreaseLengthSizeButton.getGlobalBounds().width / 2.0f, decreaseLengthSizeButton.getPosition().y + decreaseLengthSizeButton.getGlobalBounds().height / 2.0f));
	return decreaseLengthSizeText;
}

sf::RectangleShape createEasyButton(sf::Window& window) {
	sf::RectangleShape easyButton(sf::Vector2f(150, 50));
	easyButton.setPosition(window.getSize().x - 150, 250);
	return easyButton;
}
sf::Text createEasyText(sf::Font& font, sf::RectangleShape& easyButton) {
	sf::Text easyText;
	easyText.setFont(font);
	easyText.setString("Easy");
	easyText.setCharacterSize(24);
	easyText.setFillColor(sf::Color::Black);
	sf::FloatRect textRect = easyText.getLocalBounds();
	easyText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	easyText.setPosition(sf::Vector2f(easyButton.getPosition().x + easyButton.getGlobalBounds().width / 2.0f, easyButton.getPosition().y + easyButton.getGlobalBounds().height / 2.0f));
	return easyText;
}

sf::RectangleShape createMediumButton(sf::Window& window) {
	sf::RectangleShape mediumButton(sf::Vector2f(150, 50));
	mediumButton.setPosition(window.getSize().x - 150, 300);
	return mediumButton;
}
sf::Text createMediumText(sf::Font& font, sf::RectangleShape& mediumButton) {
	sf::Text mediumText;
	mediumText.setFont(font);
	mediumText.setString("Medium");
	mediumText.setCharacterSize(24);
	mediumText.setFillColor(sf::Color::Black);
	sf::FloatRect textRect = mediumText.getLocalBounds();
	mediumText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	mediumText.setPosition(sf::Vector2f(mediumButton.getPosition().x + mediumButton.getGlobalBounds().width / 2.0f, mediumButton.getPosition().y + mediumButton.getGlobalBounds().height / 2.0f));
	return mediumText;
}

sf::RectangleShape createHardButton(sf::Window& window) {
	sf::RectangleShape hardButton(sf::Vector2f(150, 50));
	hardButton.setPosition(window.getSize().x - 150, 350);
	return hardButton;
}
sf::Text createHardText(sf::Font& font, sf::RectangleShape& hardButton) {
	sf::Text hardText;
	hardText.setFont(font);
	hardText.setString("Hard");
	hardText.setCharacterSize(24);
	hardText.setFillColor(sf::Color::Black);
	sf::FloatRect textRect = hardText.getLocalBounds();
	hardText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	hardText.setPosition(sf::Vector2f(hardButton.getPosition().x + hardButton.getGlobalBounds().width / 2.0f, hardButton.getPosition().y + hardButton.getGlobalBounds().height / 2.0f));
	return hardText;
}

sf::Text createWinPlayerText(sf::Font& font) {
	sf::Text winPlayer;
	winPlayer.setFont(font);
	winPlayer.setString("Player X has won!");
	winPlayer.setCharacterSize(24);
	winPlayer.setFillColor(sf::Color::Red);
	winPlayer.setStyle(sf::Text::Bold | sf::Text::Underlined);
	return winPlayer;
}
sf::Text createWinAIText(sf::Font& font){
	sf::Text winAI;
	winAI.setFont(font);
	winAI.setString("AI has won!");
	winAI.setCharacterSize(24);
	winAI.setFillColor(sf::Color::Red);
	winAI.setStyle(sf::Text::Bold | sf::Text::Underlined);
	return winAI;
}
sf::Text createTieText(sf::Font& font) {
	sf::Text tie;
	tie.setFont(font);
	tie.setString("Tie");
	tie.setCharacterSize(24);
	tie.setFillColor(sf::Color::Red);
	tie.setStyle(sf::Text::Bold | sf::Text::Underlined);
	return tie;
}
sf::Text createIntroText(sf::Font& font, sf::Window& window) {
	sf::Text introText;
	introText.setFont(font);
	introText.setString("Gameplay Instructions:\n\nPlayer X starts the game.\n\nTo make a move, click on an empty cell.\n\nThe goal is to get a line of three of your own marks \n(vertical, horizontal, or diagonal).\n\nPress Enter to continue.");
	introText.setCharacterSize(24);
	introText.setFillColor(sf::Color::White);
	sf::FloatRect textRect = introText.getLocalBounds();
	introText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	introText.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));
	return introText;
}


