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

class WaterBrick :public Brick {
private:
	vector<shared_ptr<Brick>> &bricks;
	const int animationRefreshRate;
	int refreshCounter;
	vector<Texture>::iterator currentTexture;
public:
	WaterBrick(float x, float y, int d, int s, vector<shared_ptr<Brick>> &b,bool e = false) :Brick(x, y, d, s),
		bricks(b),
	animationRefreshRate(Refresh_Frequency/10),
	refreshCounter(0){
		Texture texture;
		ostringstream ss;
		for (int i = 1; i <= Water_Texture_Number; i++) {
			ss.str("");
			ss << Texture_Folder << '/' << Texture_Brick_Subfolder << "/water" << i << ".png";
			texture.loadFromFile(ss.str());
			animationTextures.push_back(texture);
		}		
		currentTexture = animationTextures.begin();
		animation.setTexture(&(*currentTexture));
	}
	virtual Interaction interact(Ball &ball);
	virtual void act(Ball &ball, Paddle &paddle);
	virtual void upgradeBricks(bool upgrade);
	virtual void setDisplay();
	virtual bool isNormal() { return false; }
};

Interaction WaterBrick::interact(Ball &ball) {
	Interaction i = bounce(ball);
	int damage = 0;
	if (i.xFlip || i.yFlip) {
		int damage = Brick_Duribility / 4;
		if (waterUpgraded)
			damage /= 2;
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
			i.score = score;
			upgradeBricks(false);
		}
	}
	return i;
}

void WaterBrick::act(Ball &ball, Paddle &paddle) {
	if (refreshCounter < animationRefreshRate) {
		++refreshCounter;
	}
	else {
		refreshCounter = 0;
		currentTexture++;
		if (currentTexture == animationTextures.end())
			currentTexture = animationTextures.begin();
		animation.setTexture(&(*currentTexture));
	}/*
	for (shared_ptr<Brick> b : bricks) {
		if (b->isNormal() && getPosition().x == b->getPosition().x) {
			if (durability > 0) {
				b->setWaterUpgrade(true);
				b->setAnimation(Water_Upgraded_Background_Color);
			}
			else {
				b->setWaterUpgrade(false);
				b->setAnimation(Normal_Brick_Background_Color);
			}
		}
	}*/
}

void WaterBrick::upgradeBricks(bool upgrade) {
	for (shared_ptr<Brick> b : bricks) {
		if (b->isNormal() && getPosition().x == b->getPosition().x) {
			if (upgrade) {
				b->setWaterUpgrade(true);
				if (b->getEarthUpgrade())
					b->setBackground(Due_Upgraded_Background_Color);
				else
					b->setBackground(Water_Upgraded_Background_Color);
			}
			else {
				b->setWaterUpgrade(false);
				if (b->getEarthUpgrade())
					b->setBackground(Earth_Upgraded_Background_Color);
				else
					b->setBackground(Normal_Brick_Background_Color);
			}
		}
	}
}

void WaterBrick::setDisplay() {
	if (!isEmpty) {
		durability = Brick_Duribility;
		setTexture(&textures[0]);
	}
}