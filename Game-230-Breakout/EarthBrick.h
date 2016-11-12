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
	virtual void act(vector<vector<shared_ptr<Brick>>> &brickGrid,Ball &ball, Paddle &paddle);
	virtual void setDisplay();
};

Interaction EarthBrick::interact(Ball &ball) {
	Interaction i = bounce(ball);
	int damage = 0;
	if (i.xFlip || i.yFlip) {
		int damage = Brick_Duribility/2;
		if (waterUpgraded)
			damage /= 2;
		if (earthUpgraded)
			damage /= 2;
		if (windUpgraded)
			damage /= 2;
		durability -= damage;
		if (durability <= 0) {
			durability = 0;
			setTexture(nullptr);
			i.score = score;
		}
	}
	return i;
}

void EarthBrick::act(vector<vector<shared_ptr<Brick>>> &brickGrid,Ball &ball, Paddle &paddle) {

}

void EarthBrick::setDisplay() {
	if (!isEmpty) {
		durability = Brick_Duribility;
		setTexture(&textures[0]);
	}
}