#pragma once
#include <string>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "GameConstants.h"
#include "Ball.h"

using namespace sf;
using namespace std;

class Paddle :public RectangleShape {
private:
	Vector2f velocity;
	SoundBuffer buffer;
	Sound hitBall;
	float speed;
	float deflection;//Coefficient for determining the vertical speed of the ball after hitting the paddle
public:
	Paddle(float width = 0, float height = 0, float dec = 0) :RectangleShape(Vector2f(width, height)),
		deflection(dec) {
		setOutlineColor(Color::Black);
		buffer.loadFromFile(Audio_Folder+string("/hit_ball.wav"));
		hitBall.setBuffer(buffer);
	}
	void setSpeed(float s) { speed = s; }
	float getSpeed() { return speed; }
	void setVelocity(Vector2f v) { velocity = v; }
	void moveLeft() { move(-speed,0); }
	void moveRight(){ move(speed,0); }
	bool interact(Ball &ball);
};

bool Paddle::interact(Ball &ball) {
	FloatRect ballBound = ball.getGlobalBounds();
	FloatRect paddleBound = getGlobalBounds();
	if(ballBound.intersects(paddleBound)){
		ball.setPosition(ballBound.left,paddleBound.top-ballBound.height);
		ball.setYSpeed(-ball.getVelocity().y);
		float offset = ballBound.left + ball.getRadius() - paddleBound.left  - paddleBound.width / 2;//Distance between the point of contact and the center of the paddle
		ball.setXSpeed(offset*deflection);//The further away the ball is from the center of the paddle, the higher the horizontal speed when bouncing back.
		hitBall.play();
		return true;
	}
	return false;
}