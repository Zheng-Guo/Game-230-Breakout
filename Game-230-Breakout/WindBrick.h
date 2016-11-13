#pragma once
#include <SFML\Graphics.hpp>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include "GameConstants.h"
#include "Brick.h"

using namespace sf;
using namespace std;

class WindBrick :public Brick {
private:
	vector<shared_ptr<Brick>> &bricks;
	const int animationRefreshRate;
	int refreshCounter;
public:
	WindBrick(float x, float y, int d, int s, vector<shared_ptr<Brick>> &b, bool e = false) :Brick(x, y, d, s),
		bricks(b),
		animationRefreshRate(Refresh_Frequency / 10),
		refreshCounter(0) {
		animation.setOrigin(x / 2*3, y / 2*3);
		animation.setSize(Vector2f(Brick_Width * 3, Brick_Height * 3));
		Texture texture;
		texture.loadFromFile(string(Texture_Folder) + '/' + Texture_Brick_Subfolder + '/' + "wind.png");
		animationTextures.push_back(texture);
		animation.setTexture(&animationTextures[0]);
	}
	virtual Interaction interact(Ball &ball);
	virtual int act(Player &p);
	virtual void upgradeBricks(bool upgrade);
	virtual void setDisplay();
	virtual bool isNormal() { return false; }
	virtual void setPosition(float x, float y);
	virtual void animate();
	virtual bool isNull() { return false; }
};

Interaction WindBrick::interact(Ball &ball) {
	Interaction i = bounce(ball);
	float newXSpeed, newYSpeed;
	float deltaSpeed = (rand() % 3 + 1) / 10.0;
	newXSpeed = ball.getVelocity().x > 0 ? ball.getVelocity().x + deltaSpeed : ball.getVelocity().x - deltaSpeed;
	deltaSpeed = (rand() % 3 + 1) / 10.0;
	newYSpeed = ball.getVelocity().y > 0 ? ball.getVelocity().y + deltaSpeed : ball.getVelocity().y - deltaSpeed;
	ball.setXSpeed(newXSpeed);
	ball.setYSpeed(newYSpeed);
	int damage = 0;
	if (i.xFlip || i.yFlip) {
		int damage = Brick_Duribility / 4;
		if (nullUpgraded)
			damage =1;
		durability -= damage;
		if (durability > Brick_Duribility / 4 * 3)
			setTexture(&textures[0]);
		else if (durability > Brick_Duribility / 2)
			setTexture(&textures[1]);
		else if (durability > Brick_Duribility / 4)
			setTexture(&textures[2]);
		else if (durability >0)
			setTexture(&textures[3]);
		else {
			durability = 0;
			setTexture(nullptr);
			animation.setTexture(nullptr);
			i.score = score;
			upgradeBricks(false);
		}
	}
	return i;
}

int WindBrick::act(Player &p) {
	
	return 0;
}

void WindBrick::upgradeBricks(bool upgrade) {
	for (shared_ptr<Brick> b : bricks) {
		if (b->isNormal() && 
			b->getPosition().x>=getPosition().x-Brick_Width&&b->getPosition().x<=getPosition().x +Brick_Width&&
			b->getPosition().y>=getPosition().y-Brick_Height&&b->getPosition().y<=getPosition().y+Brick_Height) {
			if (upgrade) {
				b->setWindUpgrade(true);
			}
			else {
				b->setWindUpgrade(false);
			}
		}
	}
}

void WindBrick::setDisplay() {
	if (!isEmpty) {
		durability = Brick_Duribility;
		setTexture(&textures[0]);
	}
}

void WindBrick::setPosition(float x, float y) {
	RectangleShape::setPosition(x, y);
	animation.setPosition(getSize().x/2+x, getSize().y/2+y);
	background.setPosition(x, y);
}

void WindBrick::animate() {
	if (refreshCounter < animationRefreshRate) {
		++refreshCounter;
	}
	else {
		refreshCounter = 0;
		animation.rotate(30);
	}
}