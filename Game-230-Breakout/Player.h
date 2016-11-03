#pragma once
#include <cstdlib>
#include <ctime>
#include <SFML\Graphics.hpp>
#include "GameConstants.h"
#include "Paddle.h"

using namespace sf;

class Player {
private:
	Paddle paddle;
	int score;
	int life;
	int powerUpType;
	float leftBound, rightBound;
	bool poweredUp, powerUpUsed;
public:
	Player(Vector2f position,float l,float r) :paddle(Paddle_Width, Paddle_Height, Deflection_Coefficient),
		score(0),
		life(Player_Initial_Lives),
		leftBound(l),
		rightBound(r),
		poweredUp(false),
		powerUpUsed(false){
		paddle.setOutlineColor(Color::Black);
		paddle.setPosition(position);
		paddle.setSpeed(Paddle_Speed);
		srand(time(NULL));
	}
	Paddle getPaddle() { return paddle; }
	void setPaddleColor(Color c) { paddle.setFillColor(c); }
	void setPaddlePosition(float x, float y) { paddle.setPosition(x, y); }
	void resetScore() { score = 0; }
	void scorePoint() { score += 1; }
	int getScore() { return score; }
	int getLives() { return life; }
	void moveLeft();
	void moveRight();
	void interact(Ball &ball);
	void setPowerUpType(int p) { powerUpType = p; }
	int getPowerUpType() { return poweredUp; }
	void setPowerUp(bool p) { poweredUp = p; }
	void setPowerUpUsed(bool p) { powerUpUsed = p; }
	bool isPoweredUp() { return poweredUp; }
	bool isPowerUpUsed() { return powerUpUsed; }
	void usePowerUp();
};

void Player::moveLeft() {
	paddle.moveLeft();
	FloatRect paddleBound = paddle.getGlobalBounds();
	if (paddleBound.left <= leftBound)
		paddle.setPosition(leftBound, paddleBound.top);
		
}

void Player::moveRight() {
	paddle.moveRight();
	FloatRect paddleBound = paddle.getGlobalBounds();
	if (paddleBound.left+paddleBound.width >= rightBound)
		paddle.setPosition(rightBound-paddleBound.width, paddleBound.top);		
}


void Player::interact(Ball &ball) {
	paddle.interact(ball);
	if (powerUpUsed) {//Throw only one paddle at a time.

	}
}


void Player::usePowerUp() {
	if (poweredUp&&!powerUpUsed) {//If power up obtained and not yet used.
		
		powerUpUsed = true;
	}
}