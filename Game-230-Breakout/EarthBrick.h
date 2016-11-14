#pragma once
#include <SFML\Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include "GameConstants.h"
#include "Brick.h"
#include "Player.h"

using namespace sf;
using namespace std;

class EarthBrick :public Brick {
private:
	vector<shared_ptr<Brick>> &bricks;
public:
	EarthBrick(float x, float y, int d, int s, vector<shared_ptr<Brick>> &b,bool e = false):Brick(x,y,d,s),
	bricks(b){
		Texture texture;
		texture.loadFromFile(string(Texture_Folder)+'/'+Texture_Brick_Subfolder+'/'+"earth.png");
		animationTextures.push_back(texture);
		animation.setTexture(&animationTextures[0]);		
	}
	virtual Interaction interact(Ball &ball);
	virtual void act(Player &p) { }
	virtual void upgradeBricks(bool upgrade);
	virtual void setDisplay();
	virtual bool isNormal() { return false; }
	virtual void animate() {}
	virtual bool isNull() { return false; }
	virtual void breakBrick();
};

Interaction EarthBrick::interact(Ball &ball) {
 	Interaction i = bounce(ball);
	int damage = 0;
	if (i.xFlip || i.yFlip) {
		int damage = Brick_Duribility/4;
		if (nullUpgraded) {
			if (ball.getPowerUpType() == Element::Normal)
				damage = 1;
			else
				damage = 0;
		}
		else if (ball.getPowerUpType() == Element::Water)
			damage /= 2;
		else if (ball.getPowerUpType() == Element::Thunder)
			damage = Brick_Duribility;
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
			breakBrick();
			i.score = score;
		}
	}
	return i;
}

void EarthBrick::upgradeBricks(bool upgrade) {
	for (shared_ptr<Brick> b : bricks) {
		if (b->isNormal() && getPosition().y == b->getPosition().y) {
			if (upgrade) {
				b->setEarthUpgrade(true);
				if (b->getWaterUpgrade())
					b->setBackground(Due_Upgraded_Background_Color);
				else
					b->setBackground(Earth_Upgraded_Background_Color);
			}
			else {
				b->setEarthUpgrade(false);
				if (b->getWaterUpgrade())
					b->setBackground(Water_Upgraded_Background_Color);
				else
					b->setBackground(Normal_Brick_Background_Color);
			}
		}
	}
}
void EarthBrick::setDisplay() {
	if (!isEmpty) {
		durability = Brick_Duribility;
		setTexture(&textures[0]);
	}
}

void EarthBrick::breakBrick() {
	durability = 0;
	setTexture(nullptr);
	upgradeBricks(false);
}