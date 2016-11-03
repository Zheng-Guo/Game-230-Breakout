#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "Ball.h"

using namespace sf;

class Paddle :public RectangleShape {
private:
	Vector2f velocity;
	SoundBuffer buffer;
	Sound hitBall;
	float speed;
	float acceleration;
	float deflection;//Coefficient for determining the vertical speed of the ball after hitting the paddle
	int player;
public:
	Paddle(int player, float width = 0, float height = 0, float dec = 0) :player(player),
		RectangleShape(Vector2f(width, height)),
		deflection(dec) {
		RectangleShape::setOutlineColor(Color::Black);
		buffer.loadFromFile("hit_ball.wav");
		hitBall.setBuffer(buffer);
	}
	void setSpeed(float s) { speed = s; }
	float getSpeed() { return speed; }
	void setAcceleration(float a) { acceleration = a; }
	float getAcceleration() { return acceleration; }
	void setVelocity(Vector2f v) { velocity = v; }
	void moveUp() { RectangleShape::move(0, -speed); }
	void moveDown(){ RectangleShape::move(0, speed); }
	void move() { RectangleShape::move(velocity); }
	bool interact(Ball &ball);
};

bool Paddle::interact(Ball &ball) {
	FloatRect ballBound = ball.getGlobalBounds();
	FloatRect paddleBound = RectangleShape::getGlobalBounds();
	if(ballBound.intersects(paddleBound)){
		if (paddleBound.contains(Vector2f(ballBound.left, ballBound.top)) || paddleBound.contains(Vector2f(ballBound.left, ballBound.top + ballBound.height)))
			ball.setPosition(paddleBound.left + paddleBound.width, ballBound.top);
		if (paddleBound.contains(Vector2f(ballBound.left+ballBound.width, ballBound.top)) || paddleBound.contains(Vector2f(ballBound.left+ballBound.width, ballBound.top + ballBound.height)))
			ball.setPosition(paddleBound.left-ballBound.width, ballBound.top);
		ball.setXSpeed(-ball.getVelocity().x + acceleration);
		float offset = ballBound.top - paddleBound.top + ball.getRadius() - paddleBound.height/2;//Distance between the point of contact and the center of the paddle
		ball.setYSpeed(offset*deflection);//The further away the ball is from the center of the paddle, the higher the vertical speed when bouncing back.
		ball.setPlayer(player);//Update last hitting player.
		hitBall.play();
		return true;
	}
	return false;
}