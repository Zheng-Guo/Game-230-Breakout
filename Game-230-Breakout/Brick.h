#pragma once
#include <SFML\Graphics.hpp>
#include <utility>
#include "Ball.h"
#include "Paddle.h"

using namespace sf;
using namespace std;

class Brick :public RectangleShape {
protected:
	static Texture textures[];
	RectangleShape background;
	int durability;
	int score;
	bool waterUpgraded, earthUpgraded, windUpgraded;
	bool topExposed, bottomExposed, leftExposed, rightExposed;
public:
	Brick(float x, float y,int d,int s) :RectangleShape(Vector2f(x, y)),
		background(Vector2f(x,y)),
		durability(d),
		score(s),
		waterUpgraded(false),
		earthUpgraded(false),
		windUpgraded(false),
		topExposed(true),
		bottomExposed(true),
		leftExposed(true),
		rightExposed(true){
		setTexture(&textures[0]);
		background.setFillColor(Color::White);
	}
	virtual pair<bool, bool> interact(Ball &ball);
	virtual void act(Ball &ball,Paddle &paddle);
	void setPosition(float x, float y);
	void setBackground(Color c) { background.setFillColor(c); }
	RectangleShape getBackground() { return background; }
	void setTopExposure(bool b) { topExposed = b; }
	void setBottomExposure(bool b) { bottomExposed = b; }
	void setLeftExposure(bool b) { leftExposed = b; }
	void setRightExposure(bool b) { rightExposed = b; }
	bool isBroken() { return durability == 0; }
	static void loadTextures();
};

Texture Brick::textures[4];

void Brick::loadTextures() {
	textures[0].loadFromFile("brick/Brick.png");
	textures[1].loadFromFile("brick/Brick cracked 1.png");
	textures[2].loadFromFile("brick/Brick cracked 2.png");
	textures[3].loadFromFile("brick/Brick cracked 3.png");
}

pair<bool,bool> Brick::interact(Ball &ball) {
	FloatRect ballBound = ball.getGlobalBounds();
	FloatRect brickBound = getGlobalBounds();
	bool flipX = false, flipY = false;
	if (durability > 0) {
		if (ball.getVelocity().y > 0 && topExposed &&
			(brickBound.contains(ballBound.left, ballBound.top + ballBound.height) || brickBound.contains(ballBound.left + ballBound.width, ballBound.top + ballBound.height))) {
			flipY = true;
		}
		if (ball.getVelocity().y < 0 && bottomExposed &&
			(brickBound.contains(ballBound.left, ballBound.top) || brickBound.contains(ballBound.left + ballBound.width, ballBound.top))) {
			flipY = true;
		}
		if (ball.getVelocity().x>0 && leftExposed &&
			(brickBound.contains(ballBound.left + ballBound.width, ballBound.top) || brickBound.contains(ballBound.left + ballBound.width, ballBound.top + ballBound.height))) {
			flipX = true;
		}
		if (ball.getVelocity().x<0 && rightExposed &&
			(brickBound.contains(ballBound.left, ballBound.top) || brickBound.contains(ballBound.left, ballBound.top + ballBound.height))) {
			flipX = true;
		}
		if (flipX||flipY) {
			durability = 0;
			setTexture(nullptr);
			if (flipX&&flipY) {
				flipX = !brickBound.contains(ballBound.left + ballBound.width / 2, ballBound.top) && !brickBound.contains(ballBound.left + ballBound.width / 2, ballBound.top + ballBound.height); 
				flipY = !brickBound.contains(ballBound.left + ballBound.width, ballBound.top + ballBound.height / 2) && !brickBound.contains(ballBound.left, ballBound.top + ballBound.height / 2);
			}
		}
	}
	
	return pair<bool,bool>(flipX,flipY);
}

void Brick::act(Ball &ball, Paddle &paddle) {

}

void Brick::setPosition(float x, float y) {
	RectangleShape::setPosition(x, y);
	background.setPosition(x, y);
}