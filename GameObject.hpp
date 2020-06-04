#pragma once
#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>
#include "Form.hpp"

//float normalise(const float value);

class Block {
public:
	Sprite sprite;
	bool ableToDraw;
	bool isDamaged;
	Clock clock;
	Time damaged_timer;

	Block();

	Block(int type_, Texture& texture, Clock clock_);

	int getHp();

	void update();

	void draw(RenderWindow& window);

	void damaged(int damage);

private:
	int hp;
	int type;
};

class Ball {
public:
	float dx = static_cast<float>(rand() % 3 - 6);
	float dy = -5;
	float speed = 8;
	Sprite sprite;
	Clock clock;
	Vector2f velocity;
	vector<Block>* blocksSideL;
	vector<Block>* blocksBase;
	vector<Block>* blocksSideR;
	int count = 3;
	bool drawable = true;

	Ball(const Sprite& paddle_, Texture& texture_, vector<Block>& blocksLeft_, vector<Block>& blocksCenter_, vector<Block>& blocksRight_, Clock clock_);

	Ball(Vector2f startPosition_, Texture& texture_, vector<Block> &blocksLeft_, vector<Block>& blocksCenter_, vector<Block>& blocksRight_, Clock clock_);

	void lifedown();

	float normalise(float value);

	void kick(float x, float y);

	void PaddleCollision(Sprite &paddle, float dx_);

	void collisionReflecting(Block &block);

	void BlockCollision();

	void BorderCollision();

	void update(Sprite &paddle, float time_, float dx_);

	void draw(RenderWindow& window);
};