#include <iostream>
#include <ctime>
#include <filesystem>

#include <SFML/Graphics.hpp>
#include <SFML/Audio/Music.hpp>

#include "MainMenu.hpp"
#include "ResourceManager.hpp";

using namespace std;
using namespace sf;


int main() {
	srand(static_cast<unsigned int>(time(NULL)));
	Clock clock;

	ContextSettings settings;
	settings.antialiasingLevel = 12;

	ResourceManager &gameResources = ResourceManager::getInstance();
	Settings &gameSettings = Settings::getInstance();

	RenderWindow window(VideoMode(gameSettings.getResolution().x, gameSettings.getResolution().y), "Arkanoid", Style::Close);
	//window.setVerticalSyncEnabled(true);

	Texture loadingImage;
	loadingImage.loadFromFile("images/loading.png");
	loadingImage.setSmooth(true);

	Sprite loading;
	loading.setTexture(loadingImage);
	loading.setScale(Vector2f(window.getSize().x / loading.getGlobalBounds().width, window.getSize().y / loading.getGlobalBounds().height));
	while (window.isOpen()) {
		Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.draw(loading);

		if (clock.getElapsedTime().asSeconds() > 1) {
			if (Settings::getInstance().successfulLoaded()) {
				MainMenu(window);
			}
		}

		window.display();
	}
}