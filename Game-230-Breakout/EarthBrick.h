#pragma once
#include <SFML\Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include "GameConstants.h"
#include "Brick.h"

using namespace sf;
using namespace std;

class EarthBrick :public Brick {
private:
	bool nullUpgraded;
public:
	EarthBrick(float x, float y, int d, int s, bool e = false):Brick(x,y,d,s),
		nullUpgraded(false){
		Texture texture;
		texture.loadFromFile(string(Texture_Folder)+'/'+Texture_Brick_Subfolder+'/'+"earth.png");
		animationTextures.push_back(texture);
		animation.setTexture(&animationTextures[0]);		
	}
	virtual Interaction interact(Ball &ball);
	virtual void act(vector<shared_ptr<Brick>> &bricks,Ball &ball, Paddle &paddle);
	virtual void setDisplay();
	virtual bool isNormal() { return false; }
};

Interaction EarthBrick::interact(Ball &ball) {
	Interaction i = bounce(ball);
	int damage = 0;
	if (i.xFlip || i.yFlip) {
		int damage = Brick_Duribility/4;
		if (waterUpgraded)
			damage /= 2;
		if (earthUpgraded)
			damage /= 2;
		if (windUpgraded)
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
		}
	}
	return i;
}

void EarthBrick::act(vector<shared_ptr<Brick>> &bricks,Ball &ball, Paddle &paddle) {
	for (shared_ptr<Brick> b : bricks) {
		if (b->isNormal()&& getPosition().y == b->getPosition().y) {
			if (durability > 0) {
				b->setEarthUpgrade(true);
				b->setAnimation(Earth_Upgraded_Background_Color);
			}
			else {
				b->setEarthUpgrade(false);
				b->setAnimation(Normal_Brick_Background_Color);
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