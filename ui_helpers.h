#ifndef UI_HELPERS_H
#define UI_HELPERS_H

#include <SFML/Graphics.hpp>

sf::ConvexShape createRoundedRectangle(float width, float height, float radius);

sf::Text createTitle(sf::Font& font, sf::Window& window);
sf::Text createEnterPrompt(sf::Font& font);

sf::ConvexShape createStartButton();
sf::Text createStartText(sf::Font& font, sf::ConvexShape& startButton);

sf::ConvexShape createOptionsButton();
sf::Text createOptionsText(sf::Font& font, sf::ConvexShape& optionsButton);

sf::ConvexShape createExitButton();
sf::Text createExitText(sf::Font& font, sf::ConvexShape& exitButton);

sf::RectangleShape createOptionsPanel(sf::Window& window);

sf::RectangleShape createIncreaseBoardSizeButton(sf::Window& window);
sf::Text createIncreaseBoardSizeText(sf::Font& font, sf::RectangleShape& increaseBoardSizeButton);

sf::RectangleShape createDecreaseBoardSizeButton(sf::Window& window);
sf::Text createDecreaseBoardSizeText(sf::Font& font, sf::RectangleShape& decreaseBoardSizeButton);
sf::RectangleShape createIncreaseLengthSizeButton(sf::Window& window);
sf::Text createIncreaseLengthSizeText(sf::Font& font, sf::RectangleShape& increaseLengthSizeButton);

sf::RectangleShape createDecreaseLengthSizeButton(sf::Window& window);
sf::Text createDecreaseLengthSizeText(sf::Font& font, sf::RectangleShape& decreaseLengthSizeButton);

sf::RectangleShape createEasyButton(sf::Window& window);
sf::Text createEasyText(sf::Font& font, sf::RectangleShape& easyButton);

sf::RectangleShape createMediumButton(sf::Window& window);
sf::Text createMediumText(sf::Font& font, sf::RectangleShape& mediumButton);
sf::RectangleShape createHardButton(sf::Window& window);
sf::Text createHardText(sf::Font& font, sf::RectangleShape& hardButton);

sf::Text createWinPlayerText(sf::Font& font);
sf::Text createWinAIText(sf::Font& font);
sf::Text createTieText(sf::Font& font);
sf::Text createIntroText(sf::Font& font, sf::Window& window);

sf::Text createRetryPrompt(sf::Font& font);
sf::Text createMenuPrompt(sf::Font& font);

#endif // UI_HELPERS_H