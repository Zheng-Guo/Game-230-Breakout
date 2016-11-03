#pragma once
#include <SFML\Graphics.hpp>

using namespace sf;

class Ball :public CircleShape{
private:
	Texture texture;
	Vector2f velocity;
	int powerUpType;//The current power up applied to the ball
public:
	Ball(float r=0,float x=0,float y=0) :CircleShape(r),powerUpType(0) {
		CircleShape::setPosition(x, y);
		CircleShape::setFillColor(Color::White);
		CircleShape::setOutlineColor(Color::Black);
	}
	void accelerate(Vector2f v) { velocity.x += v.x; velocity.y += v.y; }
	void setXSpeed(float s) { velocity.x = s; }
	void setYSpeed(float s) { velocity.y = s; }
	Vector2f getVelocity() const { return velocity; }
	void move() { CircleShape::move(velocity); }
	int getPowerUpType() { return powerUpType; }
	void setPowerUpType(int p) { powerUpType = p; }
};
