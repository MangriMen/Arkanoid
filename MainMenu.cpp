#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Music.hpp>

#include "Form.hpp"

#include "MainMenu.hpp"
#include "LevelMenu.hpp"
#include "SettingsMenu.hpp"
#include "ResourceManager.hpp";
#include "Level.hpp"

using namespace std;
using namespace sf;
using namespace form;

void MainMenu(RenderWindow& window)
{
    const int WIDTH = window.getSize().x, HEIGHT = window.getSize().y;

    Button bPlay(WIDTH / 2 - 100, (HEIGHT / 2) - 25 - 150 , 200, 50, "Play", 20),
        bSettings(WIDTH / 2 - 100, (HEIGHT / 2) - 25, 200, 50, "Settings", 20),
        bExit(WIDTH / 2 - 100, (HEIGHT / 2) - 25 + 150, 200, 50, "Exit", 20);

    while (window.isOpen()) {
        Event event;
        Vector2i mouse = Mouse::getPosition(window);
        Settings::getInstance().updateMusic();


        window.clear(Color(34, 34, 38));

        bPlay.setStyle("default");
        bSettings.setStyle("default");
        bExit.setStyle("default");

        if (bPlay.select(mouse)) {
            bPlay.setStyle("hover");
        }
        if (bSettings.select(mouse)) {
            bSettings.setStyle("hover");
        }
        if (bExit.select(mouse)) {
            bExit.setStyle("hover");
        }

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

			if (event.type == Event::MouseButtonReleased)
			{
				if (event.mouseButton.button == Mouse::Left)
				{
                    if (bPlay.select(mouse)) {
                        Level::getInstance().clearLevel();
                        LevelMenu(window);
                    }
                    if (bSettings.select(mouse)) {
                        SettingsMenu(window);
                    }
                    if (bExit.select(mouse)) {
                        window.close();
                    }
				}
			}
		}

        window.draw(bPlay.displayButton());
        window.draw(bPlay.displayText());

        window.draw(bSettings.displayButton());
        window.draw(bSettings.displayText());

        window.draw(bExit.displayButton());
        window.draw(bExit.displayText());

        window.display();

    }
}