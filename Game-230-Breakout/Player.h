#pragma once
#include <cstdlib>
#include <ctime>
#include <SFML\Graphics.hpp>
#include "Paddle.h"

using namespace sf;

class Player {
private:
	Paddle paddle1,paddle2;
	int score;
	int player;
	float upperBound, lowerBound,powerUpEnd;
	float velocityErrorCoefficient;
	bool isAI;
	bool poweredUp, powerUpUsed;
public:
	Player(int p,Vector2f position,float u,float l,float pe,bool a) :player(p),
		paddle1(p, 20, 100, 0.08),
		paddle2(p, 20, 100, 0.08),
		score(0),
		upperBound(u),
		lowerBound(l),
		powerUpEnd(pe),
		velocityErrorCoefficient(11),
		isAI(a),
		poweredUp(false),
		powerUpUsed(false){
		if (p == 1) {
			paddle1.setFillColor(Color::Red);
			paddle2.setFillColor(Color::Red);
		}
		else {
			paddle1.setFillColor(Color::Green);
			paddle2.setFillColor(Color::Green);
		}
		paddle1.setOutlineColor(Color::Black);
		paddle1.setPosition(position);
		paddle1.setSpeed(5);
		paddle2.setOutlineColor(Color::Black);
		if (isAI) {
			paddle1.setAcceleration(-0.6);
			paddle2.setAcceleration(-0.6);
			paddle2.setVelocity(Vector2f(-8, 0));
		}
		else {			
			paddle1.setAcceleration(0.6);
			paddle2.setAcceleration(0.6);
			paddle2.setVelocity(Vector2f(8, 0));
		}
		srand(time(NULL));
	}
	Paddle getPaddle1() { return paddle1; }
	Paddle getPaddle2() { return paddle2; }
	void setPaddle1Position(float x, float y) { paddle1.setPosition(x, y); }
	void resetScore() { score = 0; }
	void scorePoint() { score += 1; }
	int getScore() { return score; }
	void moveUp();
	void moveDown();
	void movePaddle2();
	void interact(Ball &ball);
	void AIMove(Ball &ball);
	void setPowerUp(bool p) { poweredUp = p; }
	void setPowerUpUsed(bool p) { powerUpUsed = p; }
	bool isPoweredUp() { return poweredUp; }
	bool isPowerUpUsed() { return powerUpUsed; }
	void usePowerUp();
};

void Player::moveUp() {
	paddle1.moveUp();
	FloatRect paddleBound = paddle1.getGlobalBounds();
	if (paddleBound.top <= upperBound)
		paddle1.setPosition(paddleBound.left, upperBound);
		
}

void Player::moveDown() {
	paddle1.moveDown();
	FloatRect paddleBound = paddle1.getGlobalBounds();
	if (paddleBound.top+paddleBound.height >= lowerBound)
		paddle1.setPosition(paddleBound.left, lowerBound-paddleBound.height);		
}

void Player::movePaddle2() {//Move the thrown paddle towards the other side of the table.
	FloatRect rect = paddle2.getGlobalBounds();
	if (paddle2.getAcceleration() > 0 && rect.left + rect.width < powerUpEnd|| paddle2.getAcceleration() < 0 && rect.left > powerUpEnd) {//The thrown paddle disappears after hitting the ball, or when it reaches the center of the table.
		paddle2.move();
	}
	else {
		powerUpUsed = false;
	}
}

void Player::interact(Ball &ball) {
	paddle1.interact(ball);
	if (powerUpUsed) {//Throw only one paddle at a time.
		powerUpUsed=!paddle2.interact(ball);
	}
}

//AI randomly picks a point(targetPoint) from the ball and a point from the paddle(hitPoint) and attempts to hit the ball by aligning the two points horizontally.
//If the targetPoint is one ball diameter above the hitPoint or one ball diameter below the hitPoint, AI moves the paddle to re-aim.
//AI computes the two points when the ball is bounced towards its paddle. No more calculation is conducted before the ball is bounced back again.
//There is a margin of error(errorMargin) when computing the hitPoint and it is related to the speed of the ball. The higher the speed the larger the margin of error.
//The margin of error increases the chance of placing the hitPoint outside the effective hitting area of the paddle. Thus the faster the ball the more likely the AI will miss it.
void Player::AIMove(Ball &ball) {
	static float targetPoint;
	static float hitPoint;
	static bool computeMove=false;
	static bool traceBall = false;
	FloatRect ballBound = ball.getGlobalBounds();
	FloatRect paddleBound = paddle1.getGlobalBounds();
	float errorMargin = (sqrt(ball.getVelocity().x*ball.getVelocity().x + ball.getVelocity().y*ball.getVelocity().y))*velocityErrorCoefficient;
	if (computeMove) {//Compute the two aligning points once, then attempt to follow the ball.
		targetPoint = rand() % (int)ballBound.height ;
		hitPoint = rand() % (int)(errorMargin*2);
		computeMove = false;
		traceBall = true;
	}
	if (ball.getVelocity().x / paddle1.getAcceleration() < 0) {//If the vertical speed of the ball is not in the same direction as the hitting direction of the paddle, the ball begins to bounce back.
		if (!traceBall && !computeMove)//Compute the two points if have not yet done so.
			computeMove = true;
		if (traceBall) {//AI follows the ball by moving the paddle along with it.
			if (targetPoint + ballBound.top< hitPoint + paddleBound.height / 2 + paddleBound.top - errorMargin -ballBound.height)
				moveUp();
			else if(targetPoint + ballBound.top> hitPoint + paddleBound.height / 2 + paddleBound.top - errorMargin + ballBound.height)
				moveDown();
			if (poweredUp&&ballBound.left + ballBound.width >= powerUpEnd)//AI uses power up ability when the ball is within half table width's reach.
				usePowerUp();
		}
	}
	else//Do not trace the ball if it is moving away from the paddle.
		traceBall = false;
}

void Player::usePowerUp() {
	if (poweredUp&&!powerUpUsed) {//If power up obtained and not yet used.
		paddle2.setPosition(paddle1.getPosition());
		powerUpUsed = true;
	}
}