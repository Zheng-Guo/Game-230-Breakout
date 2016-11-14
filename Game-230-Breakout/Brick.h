#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include "GameConstants.h"
#include "Ball.h"
#include "Player.h"

using namespace sf;
using namespace std;

struct Interaction {
	bool xFlip;
	bool yFlip;
	float xPosition;
	float yPosition;
	int score;
};

class Brick :public RectangleShape {
protected:
	static Texture textures[];
	RectangleShape animation;
	vector<Texture> animationTextures;
	RectangleShape background;
	int durability;
	int score;
	bool isEmpty,waterUpgraded, earthUpgraded, windUpgraded,nullUpgraded;
	bool topExposed, bottomExposed, leftExposed, rightExposed;
	Interaction bounce(Ball &ball);
public:
	Brick(float x, float y,int d,int s,bool e=false) :RectangleShape(Vector2f(x, y)),
		animation(Vector2f(x,y)),
		background(Vector2f(x,y)),
		durability(d),
		score(s),
		isEmpty(e),
		waterUpgraded(false),
		earthUpgraded(false),
		windUpgraded(false),
		nullUpgraded(false),
		topExposed(true),
		bottomExposed(true),
		leftExposed(true),
		rightExposed(true){
		srand(time(NULL));
		setFillColor(Play_Area_Color);
	}
	virtual Interaction interact(Ball &ball);
	virtual int act(Player &p) { return 0; }
	virtual void upgradeBricks(bool b) {}
	virtual void setPosition(float x, float y);
	virtual void setDisplay();
	virtual void animate(){}
	void setBackground(Color c) { background.setFillColor(c); }
	void setAnimation(Color c) { animation.setFillColor(c); }
	RectangleShape getBackground() { return background; }
	RectangleShape getAnimation() { return animation; }
	void setTopExposure(bool b) { topExposed = b; }
	void setBottomExposure(bool b) { bottomExposed = b; }
	void setLeftExposure(bool b) { leftExposed = b; }
	void setRightExposure(bool b) { rightExposed = b; }
	void setDurability(int d) { durability = d; }
	bool isBroken() { return durability == 0; }
	bool isBrickEmpty() { return isEmpty; }
	virtual bool isNormal() { return true; }
	virtual bool isNull() { return false; }
	void setEarthUpgrade(bool b) { earthUpgraded = b; }
	bool getEarthUpgrade() { return earthUpgraded; }
	void setWaterUpgrade(bool b) { waterUpgraded = b; }
	bool getWaterUpgrade() { return waterUpgraded; }
	void setWindUpgrade(bool b) { windUpgraded = b; }
	bool getWindUpgrade() { return windUpgraded; }
	void setNullUpgrade(bool b) { nullUpgraded = b; }
	virtual void breakBrick() { durability = 0; setTexture(nullptr); }
	static void loadTextures();
};

Texture Brick::textures[4];

void Brick::loadTextures() {
	textures[0].loadFromFile(string(Texture_Folder)+'/'+string(Texture_Brick_Subfolder)+string("/Brick.png"));
	textures[1].loadFromFile(string(Texture_Folder) + '/' + string(Texture_Brick_Subfolder) + "/Brick cracked 1.png");
	textures[2].loadFromFile(string(Texture_Folder) + '/' + string(Texture_Brick_Subfolder) + "/Brick cracked 2.png");
	textures[3].loadFromFile(string(Texture_Folder) + '/' + string(Texture_Brick_Subfolder) + "/Brick cracked 3.png");
}

Interaction Brick::bounce(Ball &ball) {
	FloatRect ballBound = ball.getGlobalBounds();
	FloatRect brickBound = getGlobalBounds();
	Interaction i;
	i.xFlip = false;
	i.yFlip = false;
	i.xPosition = ballBound.left;
	i.yPosition = ballBound.top;
	i.score = 0;
	if (durability > 0) {
		if (ball.getVelocity().y > 0 && topExposed &&
			(brickBound.contains(ballBound.left, ballBound.top + ballBound.height) || brickBound.contains(ballBound.left + ballBound.width, ballBound.top + ballBound.height))) {
			i.yFlip = true;
			i.yPosition = brickBound.top - ballBound.height;
		}
		if (ball.getVelocity().y < 0 && bottomExposed &&
			(brickBound.contains(ballBound.left, ballBound.top) || brickBound.contains(ballBound.left + ballBound.width, ballBound.top))) {
			i.yFlip = true;
			i.yPosition = brickBound.top + brickBound.height;
		}
		if (ball.getVelocity().x>0 && leftExposed &&
			(brickBound.contains(ballBound.left + ballBound.width, ballBound.top) || brickBound.contains(ballBound.left + ballBound.width, ballBound.top + ballBound.height))) {
			i.xFlip = true;
			i.xPosition = brickBound.left - ballBound.width;
		}
		if (ball.getVelocity().x<0 && rightExposed &&
			(brickBound.contains(ballBound.left, ballBound.top) || brickBound.contains(ballBound.left, ballBound.top + ballBound.height))) {
			i.xFlip = true;
			i.xPosition = brickBound.left + brickBound.width;
		}
		if (i.xFlip&&i.yFlip) {
			i.xFlip = !brickBound.contains(ballBound.left + ballBound.width / 2, ballBound.top) && !brickBound.contains(ballBound.left + ballBound.width / 2, ballBound.top + ballBound.height);
			i.yFlip = !brickBound.contains(ballBound.left + ballBound.width, ballBound.top + ballBound.height / 2) && !brickBound.contains(ballBound.left, ballBound.top + ballBound.height / 2);
			if (!i.xFlip)
				i.xPosition = ballBound.left;
			if (!i.yFlip)
				i.yPosition = ballBound.top;
		}
	}
	return i;
}
Interaction Brick::interact(Ball &ball) {
 	Interaction i = bounce(ball);
	if (windUpgraded) {
		float newXSpeed, newYSpeed;
		float deltaSpeed = (rand() % 3 + 1) / 50.0;
		newXSpeed = ball.getVelocity().x > 0 ? ball.getVelocity().x + deltaSpeed : ball.getVelocity().x - deltaSpeed;
		deltaSpeed = (rand() % 3 + 1) / 50.0;
		newYSpeed = ball.getVelocity().y > 0 ? ball.getVelocity().y + deltaSpeed : ball.getVelocity().y - deltaSpeed;
		ball.setXSpeed(newXSpeed);
		ball.setYSpeed(newYSpeed);			
	}
	int damage=0;
	if (i.xFlip || i.yFlip) {
		int damage = Brick_Duribility;
		bool instantKill = false;
		if (nullUpgraded) {
			if (ball.getPowerUpType() == Element::Normal)
				damage = 1;
			else
				damage = 0;
		}
		else {
			if (waterUpgraded) {
				if (ball.getPowerUpType() == Element::Fire)
					damage /= 4;
				else if (ball.getPowerUpType() == Element::Earth)
					instantKill = true;
				else
					damage /= 2;
			}
			if (earthUpgraded) {
				if (ball.getPowerUpType() == Element::Water)
					damage /= 4;
				else if (ball.getPowerUpType() == Element::Thunder)
					instantKill = true;
				else
					damage /= 2;
			}
			if (windUpgraded) {
				if (ball.getPowerUpType() == Element::Thunder)
					damage /= 4;
				else if (ball.getPowerUpType() == Element::Fire)
					instantKill = true;
				else
					damage /= 2;
			}
		}
		if (!instantKill)
			durability -= damage;
		else
			durability = 0;
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

void Brick::setPosition(float x, float y) {
	RectangleShape::setPosition(x, y);
	animation.setPosition(x, y);
	background.setPosition(x, y);
}

void Brick::setDisplay() {
	if (!isEmpty) {
		durability = Brick_Duribility;
		setTexture(&textures[0]);
		background.setFillColor(Normal_Brick_Background_Color);
	}
}