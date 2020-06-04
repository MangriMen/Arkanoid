#include <iostream>
#include <SFML/Graphics.hpp>

#include "Form.hpp"
#include "LevelMenu.hpp"
#include "MainMenu.hpp"
#include "ResourceManager.hpp"
#include "Level.hpp"

using namespace std;
using namespace sf;
using namespace form;

void LevelMenu(RenderWindow& window)
{
    Level& level = Level::getInstance();
    Font font;
    font.loadFromFile("fonts/Roboto/Roboto-Regular.ttf");

    const int WIDTH = window.getSize().x, HEIGHT = window.getSize().y;

    bool isWindow = true;

    Label lTop("Levels", WIDTH / 2, 40, true, 36);

    Button bMenu(40, HEIGHT - 90, 200, 50, "Menu", 20);

    vector<ImageButton> levels;
    
    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < 6; j++) {
            levels.push_back(ImageButton(55 + j * 200, 100 + i * 180, 177.7, 100, "levels/level_" + to_string(i + j + 1) + ".png"));
        }
    }

    while (isWindow) {
        Event event;
        Vector2i mouse = Mouse::getPosition(window);
        Settings::getInstance().updateMusic();

        window.clear(Color(34, 34, 38));

        for (int i = 0; i < levels.size(); i++) {
            levels[i].setStyle("default");
        }

        bMenu.setStyle("default");

        for (int i = 0; i < levels.size(); i++) {
            if (levels[i].select(mouse)) {
                levels[i].setStyle("hover");
            }
        }

        if (bMenu.select(mouse)) {
            bMenu.setStyle("hover");
        }

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == Event::MouseButtonReleased) {
                if (event.mouseButton.button == Mouse::Left) {
                    if (bMenu.select(mouse)) {
                        MainMenu(window);
                    }
                    if (levels[0].select(mouse)) {
                        isWindow = false;
                        level.loadLevel(window, "level_1");
                    }
                    if (levels[1].select(mouse)) {
                        isWindow = false;
                        level.loadLevel(window, "level_2");
                    }
                    if (levels[2].select(mouse)) {
                        isWindow = false;
                        level.loadLevel(window, "level_3");
                    }
                    if (levels[3].select(mouse)) {
                        isWindow = false;
                        level.loadLevel(window, "level_4");
                    }
                    if (levels[4].select(mouse)) {
                        isWindow = false;
                        level.loadLevel(window, "level_5");
                    }
                    if (levels[5].select(mouse)) {
                        isWindow = false;
                        level.loadLevel(window, "level_6");
                    }
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                MainMenu(window);
            }
        }

        window.draw(lTop.display());

        window.draw(bMenu.displayButton());
        window.draw(bMenu.displayText());

        for (auto level : levels) {
            window.draw(level.displayButton());
            window.draw(level.displaySprite());
        }

        window.display();
    }
}
