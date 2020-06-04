#pragma once
#include <iostream>
#include <fstream>
#include "Form.hpp"
#include "ResourceManager.hpp"
#include "json.hpp"

using json = nlohmann::json;

class Level
{
protected:
	bool isPaused = false;
	json loadedLevel;
	map<string, int> loadedBlocks;
	string level;
	Clock clock;
public:
	Level(const string& level_ = "level_1") {
		void preloadLevel(const string & level_);
		time = 0;
		clock.restart();
	}

	void clearLevel() {
		time = 0;
		clock.restart();
	}

	float time;

	static Level& getInstance();

	void loadLevel(RenderWindow &window, const string& level_);

	void preloadLevel(const string& level_);

	void changeLevel(RenderWindow& window);

	int getBlocksCount(const string& block_);

	bool getPaused();

	void clockRestart();

	void run() {
		time = static_cast<float> (clock.restart().asSeconds() * 0.06);
	}

	class EscMenu
	{
	private:
		

		form::Label lLevel;

		EscMenu() {
			Settings& settings = Settings::getInstance();
			rBlackout = RectangleShape(Vector2f(settings.getResolution().x, settings.getResolution().y));
			rBlackout.setFillColor(Color(0, 0, 0, 120));

			rBlackoutMenu = RectangleShape(Vector2f(settings.getResolution().x / 3, settings.getResolution().y / 2));
			rBlackoutMenu.setPosition(settings.getResolution().x / 2 - rBlackoutMenu.getSize().x / 2, settings.getResolution().y / 2 - rBlackoutMenu.getSize().y / 2);
			rBlackoutMenu.setFillColor(Color(201, 178, 151));
			rBlackoutMenu.setOutlineThickness(2);
			rBlackoutMenu.setOutlineColor(Color(66, 66, 66));

		}
	public:
		static EscMenu& getInstance();

		void escMenuToggle();

		void drawEscMenu(RenderWindow& window);
		form::Button bResume = form::Button(Settings::getInstance().getResolution().x / 2 - 110, Settings::getInstance().getResolution().y / 2 - 100, 220, 50, "Resume", 20);
		form::Button bBackToMenu = form::Button(Settings::getInstance().getResolution().x / 2 - 110, Settings::getInstance().getResolution().y / 2 + 50, 220, 50, "Menu", 20);
		RectangleShape rBlackout;
		RectangleShape rBlackoutMenu;
	};
};

