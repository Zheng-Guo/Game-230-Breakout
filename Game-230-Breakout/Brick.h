#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include "GameConstants.h"
#include "Ball.h"
#include "Paddle.h"

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
		setFillColor(Play_Area_Color);
	}
	virtual Interaction interact(Ball &ball);
	virtual void act(Ball &ball, Paddle &paddle) {}
	void setPosition(float x, float y);
	virtual void setDisplay();
	void setBackground(Color c) { background.setFillColor(c); }
	RectangleShape getBackground() { return background; }
	void setTopExposure(bool b) { topExposed = b; }
	void setBottomExposure(bool b) { bottomExposed = b; }
	void setLeftExposure(bool b) { leftExposed = b; }
	void setRightExposure(bool b) { rightExposed = b; }
	void setDurability(int d) { durability = d; }
	bool isBroken() { return durability == 0; }
	bool isBrickEmpty() { return isEmpty; }
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
		if (i.xFlip || i.yFlip) {
			if (i.xFlip&&i.yFlip) {
				i.xFlip = !brickBound.contains(ballBound.left + ballBound.width / 2, ballBound.top) && !brickBound.contains(ballBound.left + ballBound.width / 2, ballBound.top + ballBound.height);
				i.yFlip = !brickBound.contains(ballBound.left + ballBound.width, ballBound.top + ballBound.height / 2) && !brickBound.contains(ballBound.left, ballBound.top + ballBound.height / 2);
				if (!i.xFlip)
					i.xPosition = ballBound.left;
				if (!i.yFlip)
					i.yPosition = ballBound.top;
			}
		}
	}
	return i;
}
Interaction Brick::interact(Ball &ball) {
 	Interaction i = bounce(ball);
	int damage=0;
	if (i.xFlip || i.yFlip) {
		int damage = durability;
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

void Brick::setPosition(float x, float y) {
	RectangleShape::setPosition(x, y);
	background.setPosition(x, y);
}

void Brick::setDisplay() {
	if (!isEmpty) {
		durability = Brick_Duribility;
		setTexture(&textures[0]);
		background.setFillColor(Normal_Brick_Background_Color);
	}
}