#pragma once
#include <SFML\Graphics.hpp>
#include "Ball.h"
#include "Paddle.h"

using namespace sf;

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
	virtual int interact(Ball &ball);
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

int Brick::interact(Ball &ball) {
	FloatRect ballBound = ball.getGlobalBounds();
	FloatRect brickBound = getGlobalBounds();
	if (durability > 0) {
		if (ball.getVelocity().y > 0 && topExposed &&
			(brickBound.contains(Vector2f(ballBound.left, ballBound.top + ballBound.height)) || brickBound.contains(Vector2f(ballBound.left + ballBound.width, ballBound.top + ballBound.height)))) {
			ball.setYSpeed(-ball.getVelocity().y);
			durability = 0;
			setTexture(nullptr);
			background.setFillColor(Color::Green);
		}
		if (ball.getVelocity().y < 0 && bottomExposed &&
			(brickBound.contains(Vector2f(ballBound.left, ballBound.top)) || brickBound.contains(Vector2f(ballBound.left + ballBound.width, ballBound.top)))) {
			ball.setYSpeed(-ball.getVelocity().y);
			durability = 0;
			setTexture(nullptr);
			background.setFillColor(Color::Green);
		}
		if (ball.getVelocity().x>0 && leftExposed &&
			(brickBound.contains(Vector2f(ballBound.left + ballBound.width, ballBound.top)) || brickBound.contains(Vector2f(ballBound.left + ballBound.width, ballBound.top + ballBound.height)))) {
			ball.setXSpeed(-ball.getVelocity().x);
			durability = 0;
			setTexture(nullptr);
			background.setFillColor(Color::Green);
		}
		if (ball.getVelocity().x<0 && rightExposed &&
			(brickBound.contains(Vector2f(ballBound.left, ballBound.top)) || brickBound.contains(Vector2f(ballBound.left, ballBound.top + ballBound.height)))) {
			ball.setXSpeed(-ball.getVelocity().x);
			durability = 0;
			setTexture(nullptr);
			background.setFillColor(Color::Green);
		}
	}
	
	return 1;
}

void Brick::act(Ball &ball, Paddle &paddle) {

}

void Brick::setPosition(float x, float y) {
	RectangleShape::setPosition(x, y);
	background.setPosition(x, y);
}