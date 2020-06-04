#include <iostream>
#include <SFML/Graphics.hpp>

#include "Form.hpp"

#include "SettingsMenu.hpp"
#include "MainMenu.hpp"
#include "ResourceManager.hpp";
#include "Level.hpp"

using namespace std;
using namespace sf;
using namespace form;

void SettingsMenu(RenderWindow& window)
{
    Font font;
    font.loadFromFile("fonts/Roboto/Roboto-Regular.ttf");

    const int WIDTH = window.getSize().x, HEIGHT = window.getSize().y;

    Label lTop("Settings", WIDTH / 2, 40, true, 36);

    Button bResolution(WIDTH / 2 - 75, HEIGHT / 2 - 120, 150, 40, "");

    Button bMusicState(WIDTH / 2 - 75, HEIGHT / 2 - 20, 150, 40, ""),
        bMusicLow(WIDTH / 2 - 125, HEIGHT / 2 - 20, 40, 40, "-"),
        bMusicHigh(WIDTH / 2 + 85, HEIGHT / 2 - 20, 40, 40, "+");

    Button bMenu(40, HEIGHT - 90, 200, 50, "Menu", 20);

    while (window.isOpen()) {
        Event event;
        Vector2i mouse = Mouse::getPosition(window);
        Settings::getInstance().updateMusic();

        bResolution.setText(to_string(window.getSize().x) + "x" + to_string(window.getSize().y));
        bMusicState.setText(to_string(static_cast<int>(ceil(Settings::getInstance().getVolume("ambient")))));

        window.clear(Color(34, 34, 38));

        bResolution.setStyle("default");

        bMusicState.setStyle("default");

        bMusicHigh.setStyle("default");
        bMusicLow.setStyle("default");

        bMenu.setStyle("default");

        if (bResolution.select(mouse)) {
            bResolution.setStyle("hover");
            bResolution.setText("Fullscreen n/f");
        }

        if (bMusicState.select(mouse)) {
            bMusicState.setStyle("hover");
            bMusicState.setText("Pause/Play");
        }

        if (bMusicHigh.select(mouse)) {
            bMusicHigh.setStyle("hover");
        }
        if (bMusicLow.select(mouse)) {
            bMusicLow.setStyle("hover");
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
                    if (bResolution.select(mouse)) {
                        Settings::getInstance().switchFullScreen(window);
                        return SettingsMenu(window);
                    }
                    if (bMusicState.select(mouse)) {
                        Settings::getInstance().switchMusic();
                    }
                    if (bMusicHigh.select(mouse)) {
                        Settings::getInstance().setVolume("ambient", 5);
                    }
                    if (bMusicLow.select(mouse)) {
                        Settings::getInstance().setVolume("ambient", -5);
                    }
                    if (bMenu.select(mouse)) {
                        MainMenu(window);
                    }
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                MainMenu(window);
            }
        }

        window.draw(lTop.display());

        window.draw(bResolution.displayButton());
        window.draw(bResolution.displayText());

        window.draw(bMusicState.displayButton());
        window.draw(bMusicState.displayText());

        window.draw(bMusicHigh.displayButton());
        window.draw(bMusicHigh.displayText());
        window.draw(bMusicLow.displayButton());
        window.draw(bMusicLow.displayText());

        window.draw(bMenu.displayButton());
        window.draw(bMenu.displayText());

        window.display();
    }
}
