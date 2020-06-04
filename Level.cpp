#include <iostream>
#include "Level.hpp"
#include "GameObject.hpp"
#include "MainMenu.hpp"
#include "Form.hpp"

Level& Level::getInstance()
{
	static Level instance;
	return instance;
}

void Level::loadLevel(RenderWindow &window, const string& level_)
{
	if (level != level_) {
		preloadLevel(level_);
	}

	changeLevel(window);
}

void Level::preloadLevel(const string& level_)
{
	level = level_;

	ResourceManager& inst = ResourceManager::getInstance();

	ifstream levelFile("levels/" + level_ + ".json");
	levelFile >> loadedLevel;
	levelFile.close();

	for (auto texture : loadedLevel["textures"]) {
		inst.loadTexture(texture[0], texture[1]);
	}

	for (auto block : loadedLevel["blocks"]) {
		loadedBlocks.insert(make_pair(block[0], block[1]));
	}

}

void Level::changeLevel(RenderWindow &window)
{
	ResourceManager& resource = ResourceManager::getInstance();
	Settings& settings = Settings::getInstance();
	EscMenu& esc = EscMenu::getInstance();

	Clock blockClock;
	Clock ballClock;

	Thread gameTime(&Level::run, &Level::getInstance());
	gameTime.launch();

	const int WIDTH = window.getSize().x, HEIGHT = window.getSize().y;

	bool isDrawable = true, isPaused = false;
	int Lose = 0;

	form::Label lWin("YOU WON!", WIDTH / 2, 100, true, 64);
	form::Label lLose("You Lose", WIDTH / 2, 100, true, 64);

	form::Label lCount("", 50, 50, false, 32);

	Sprite sBackground(resource.getTexture("Background")), sPaddle(resource.getTexture("Paddle"));
	sBackground.scale(WIDTH / sBackground.getLocalBounds().width, HEIGHT / sBackground.getLocalBounds().height);

	sPaddle.setPosition(static_cast<float> (WIDTH / 2), static_cast<float> (HEIGHT - 50));

	FloatRect fRect = sPaddle.getLocalBounds();
	sPaddle.setOrigin(fRect.left + fRect.width / 2.0f,
		fRect.top + fRect.height / 2.0f);

	vector<Block> blocksBase(getBlocksCount("Center"), Block(2, resource.getTexture("BlockGreen"), blockClock));
	int bBase = 0;

	vector<Block> blocksSideL(getBlocksCount("Left"), Block(2, resource.getTexture("BlockRed"), blockClock));
	int bSideL = 0;

	vector<Block> blocksSideR(getBlocksCount("Right"), Block(2, resource.getTexture("BlockBlue"), blockClock));
	int bSideR = 0;

	vector<Ball> balls(1, Ball(sPaddle, resource.getTexture("Ball"), blocksSideL, blocksBase, blocksSideR, ballClock));

	int n = 0;
	for (unsigned int i = 1; i <= ceil(blocksBase.size() / 10); i++)
		for (int j = 1; j <= ceil(blocksBase.size() / 10); j++) {
			blocksBase[n].sprite.setPosition(i * blocksBase[n].sprite.getLocalBounds().width + WIDTH/5, j * blocksBase[n].sprite.getLocalBounds().height);
			FloatRect fRect = blocksBase[n].sprite.getLocalBounds();
			blocksBase[n].sprite.setOrigin(fRect.left + fRect.width / 2.0f,
				fRect.top + fRect.height / 2.0f);
			n++;
		}

	n = 0;
	for (unsigned int i = 1; i <= 1; i++)
		for (int j = 1; j <= 10; j++) {
			blocksSideL[n].sprite.setPosition(i * 42 * 1.5, j * 30);
			FloatRect fRect = blocksSideL[n].sprite.getLocalBounds();
			blocksSideL[n].sprite.setOrigin(fRect.left + fRect.width / 2.0f,
				fRect.top + fRect.height / 2.0f);
			n++;
		}

	n = 0;
	for (unsigned int i = 1; i <= 1; i++)
		for (int j = 1; j <= 10; j++) {
			blocksSideR[n].sprite.setPosition(i * 42 + WIDTH - (3 * 42), j * 30);
			FloatRect fRect = blocksSideR[n].sprite.getLocalBounds();
			blocksSideR[n].sprite.setOrigin(fRect.left + fRect.width / 2.0f,
				fRect.top + fRect.height / 2.0f);
			n++;
	}

	while (window.isOpen()) {
		Event event;
		Clock clock;
		Vector2i mouse = Mouse::getPosition(window);
		settings.updateMusic();

		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
				esc.escMenuToggle();
			}
		}

		if (!getPaused()) {
			float dx = 0;
			if (sPaddle.getPosition().x - sPaddle.getOrigin().x > 0 && (Keyboard::isKeyPressed(Keyboard::Left))) {
				sPaddle.move(-10 * time, 0);
				dx = -static_cast<float> (rand() % 5 + 2)/10.0f;
			}
			if (sPaddle.getPosition().x + sPaddle.getOrigin().x < WIDTH && (Keyboard::isKeyPressed(Keyboard::Right))) {
				sPaddle.move(10 * time, 0);
				dx = static_cast<float> (rand() % 5 + 2) / 10.0f;
			}

			for (auto& ball : balls) {
				ball.update(sPaddle, time, dx);
			}
		}
		else {
			esc.bResume.setStyle("default");
			esc.bBackToMenu.setStyle("default");

			if (Lose != 0) {
				if (esc.bResume.select(mouse)) {
					esc.bResume.setStyle("hover");
				}
			}
			else
			{
				esc.bResume.setStyle("disabled");
			}
			if (esc.bBackToMenu.select(mouse)) {
				esc.bBackToMenu.setStyle("hover");
			}

			if (event.type == Event::MouseButtonReleased)
			{
				if (event.mouseButton.button == Mouse::Left)
				{
					if (esc.bResume.select(mouse)) {
						esc.escMenuToggle();
					}
					if (esc.bBackToMenu.select(mouse)) {
						esc.escMenuToggle();
						MainMenu(window);
					}
				}
			}
		}

		window.clear();

		if (isDrawable) {
			window.draw(sBackground);
			window.draw(sPaddle);

			Lose = 0;
			for (auto& ball : balls) {
				if (ball.drawable) {
					ball.draw(window);
					Lose++;
				}
				lCount.setText(to_string(ball.count));
			}

			if (Lose == 0) {
				Level::getInstance().isPaused = true;
			}

			bBase = 0;
			for (auto &blockBase : blocksBase) {
				blockBase.draw(window);
				if (blockBase.ableToDraw) {
					bBase++;
				}
			}

			bSideL = 0;
			for (auto &blockL : blocksSideL) {
				blockL.draw(window);
				if (blockL.ableToDraw) {
					bSideL++;
				}
			}

			bSideR = 0;
			for (auto& blockR : blocksSideR) {
				blockR.draw(window);
				if (blockR.ableToDraw) {
					bSideR++;
				}
			}

			if (bBase == 0 && bSideL == 0 && bSideR == 0)
			{
				Level::getInstance().isPaused = true;
			}

			window.draw(lCount.display());

			if (getPaused()) {
				esc.drawEscMenu(window);
				if (Lose == 0) {
					window.draw(lLose.display());
				}
				if (bBase == 0 && bSideL == 0 && bSideR == 0) {
					window.draw(lWin.display());
				}
			}
		}

		window.display();
	}
}

int Level::getBlocksCount(const string& block_)
{
	return loadedBlocks[block_];
}

bool Level::getPaused() {
	return isPaused;
}

void Level::clockRestart()
{
	clock.restart();
}

Level::EscMenu& Level::EscMenu::getInstance()
{
	static EscMenu instance;
	return instance;
}

void Level::EscMenu::escMenuToggle()
{
	Level::getInstance().isPaused = !Level::getInstance().isPaused;
}

void Level::EscMenu::drawEscMenu(RenderWindow& window)
{
	window.draw(rBlackout);
	window.draw(rBlackoutMenu);
	window.draw(bResume.displayButton());
	window.draw(bResume.displayText());
	window.draw(bBackToMenu.displayButton());
	window.draw(bBackToMenu.displayText());
}
