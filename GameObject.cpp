#include <iostream>
#include "GameObject.hpp"
#include "ResourceManager.hpp"

using namespace std;

Block::Block() {
	ableToDraw = true;
	damaged_timer = {};
	isDamaged = false;
	hp = 1;
	type = 1;
}

Block::Block(int type_, Texture& texture, Clock clock_) {
	ableToDraw = true;
	isDamaged = false;
	clock = clock_;
	clock.restart();
	hp = type_;
	type = type_;
	sprite.setTexture(texture);
}

int Block::getHp() {
	return hp;
}

void Block::update() {
	ResourceManager &res = ResourceManager::getInstance();
	if (hp < type) {
		sprite.setTexture(res.getTexture("BlockWhite"));
	}
	if (hp < 1) {
		ableToDraw = false;
	}
	else {
		ableToDraw = true;
	}
}

void Block::draw(RenderWindow& window) {
	if (ableToDraw)
		window.draw(sprite);
}

void Block::damaged(int damage) {
	if (clock.getElapsedTime().asMilliseconds() > 200) {
		hp -= damage;
		update();
		clock.restart();
	}
}

Ball::Ball(const Sprite& paddle_, Texture& texture_, vector<Block>& blocksLeft_, vector<Block>& blocksCenter_, vector<Block>& blocksRight_, Clock clock_)
{
	blocksSideL = &blocksLeft_;
	blocksBase = &blocksCenter_;
	blocksSideR = &blocksRight_;
	sprite.setPosition(paddle_.getPosition() + Vector2f(0, -50));
	FloatRect fRect = sprite.getLocalBounds();
	sprite.setOrigin(fRect.left + fRect.width / 2.0f,
		fRect.top + fRect.height / 2.0f);
	sprite.setTexture(texture_);
	velocity = Vector2f(normalise(dx), normalise(dy));
	clock = clock_;
	clock.restart();
}

Ball::Ball(Vector2f startPosition_, Texture& texture_, vector<Block>& blocksLeft_, vector<Block>& blocksCenter_, vector<Block>& blocksRight_, Clock clock_)
{
	blocksSideL = &blocksLeft_;
	blocksBase = &blocksCenter_;
	blocksSideR = &blocksRight_;
	sprite.setPosition(startPosition_ + Vector2f(rand() % 200 - 100, rand() % 20 - 10));
	FloatRect fRect = sprite.getLocalBounds();
	sprite.setOrigin(fRect.left + fRect.width / 2.0f,
		fRect.top + fRect.height / 2.0f);
	sprite.setTexture(texture_);
	velocity = Vector2f(normalise(dx), normalise(dy));
	clock = clock_;
	clock.restart();
}

void Ball::lifedown()
{
	if (count > 1) {
		count--;
		sprite.setPosition(Vector2f(600, 600) + Vector2f(0, -50));
	}
	else {
		drawable = false;
	}
}

float Ball::normalise(float value) {
	return value / sqrt(dx * dx + dy * dy);
}

void Ball::kick(float x, float y) {
	sprite.move(0, 0);
	sprite.move(x * 1, y * 1);
}

void Ball::collisionReflecting(Block &block) {
	if (block.ableToDraw && sprite.getGlobalBounds().intersects(block.sprite.getGlobalBounds())) {
		
		float bx = velocity.x;
		float by = velocity.y;
		if (sprite.getPosition().x < (block.sprite.getPosition().x - block.sprite.getOrigin().x))
			bx = -bx;
		if (sprite.getPosition().x + sprite.getOrigin().x > (block.sprite.getPosition().x + block.sprite.getOrigin().x))
			bx = -bx;
		if (sprite.getPosition().y < (block.sprite.getPosition().y - block.sprite.getOrigin().y))
			by = -by;
		if (sprite.getPosition().y + sprite.getOrigin().y > (block.sprite.getPosition().y + block.sprite.getOrigin().y)) {
			by = -by;
		}
		kick(normalise(bx), normalise(by));
		velocity = Vector2f(normalise(bx), normalise(by));
		dx = bx;
		dy = by;
		if (clock.getElapsedTime().asSeconds() * 1000 > 20) {
			block.damaged(1);
			clock.restart();
		}
	}
}

void Ball::PaddleCollision(Sprite& paddle, float dx_)
{
	if (sprite.getGlobalBounds().intersects(paddle.getGlobalBounds())) {
		float pX = velocity.x;
		float pY = velocity.y;
		if (sprite.getPosition().x < (paddle.getPosition().x - paddle.getOrigin().x + 2) || sprite.getPosition().x > (paddle.getPosition().x + paddle.getOrigin().x - 2)) {
			pX = -pX;
		}
		pY = -pY;
		if ((sprite.getPosition().y + sprite.getOrigin().y) > (paddle.getPosition().y - paddle.getOrigin().y)) {
			kick(pX, (pY + 100));
		}
		kick(pX, pY);
		velocity = Vector2f(pX, pY);
		dx_ != 0 ? dx = pX + dx_ : dx = pX;
		dy = pY;
	}

}

void Ball::BlockCollision()
{
	for (auto& blockBase : *blocksBase) {
		collisionReflecting(blockBase);
	}

	for (auto& blockL : *blocksSideL) {
		collisionReflecting(blockL);
	}

	for (auto& blockR : *blocksSideR) {
		collisionReflecting(blockR);
	}
}

void Ball::BorderCollision()
{
	float borderX = velocity.x;
	float borderY = velocity.y;
	bool wallHit = false;
	if (sprite.getPosition().x < 0) {
		borderX = -borderX;
		wallHit = true;
	}
	if (sprite.getPosition().x + sprite.getOrigin().x > Settings::getInstance().getResolution().x) {
		borderX = -borderX;
		wallHit = true;
	}
	if (sprite.getPosition().y < 0) {
		borderY = -borderY;
		wallHit = true;
	}
	if (sprite.getPosition().y + sprite.getOrigin().y > Settings::getInstance().getResolution().y) {
		borderY = -borderY;
		wallHit = true;
		lifedown();
	}
	if (wallHit) {
		kick(normalise(borderX), normalise(borderY));
		velocity = Vector2f(normalise(borderX), normalise(borderY));
		dx = borderX;
		dy = borderY;
	}
}

void Ball::update(Sprite &paddle_, float time_, float dx_)
{
	if (drawable) {
		velocity = Vector2f(normalise(dx), normalise(dy));

		BlockCollision();

		PaddleCollision(paddle_, dx_);

		BorderCollision();

		sprite.move(velocity.x * speed * time_, velocity.y * speed * time_);
	}
}

void Ball::draw(RenderWindow& window)
{
	window.draw(sprite);
}