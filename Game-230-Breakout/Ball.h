#pragma once
#include <SFML\Graphics.hpp>

using namespace sf;

class Ball :public CircleShape{
private:
	Vector2f velocity;
	int player;//The last player hitting the ball
public:
	Ball(float r=0,float x=0,float y=0) :CircleShape(r),player(0) {
		CircleShape::setPosition(x, y);
		CircleShape::setFillColor(Color::Yellow);
		CircleShape::setOutlineColor(Color::Black);
	}
	void accelerate(Vector2f v) { velocity.x += v.x; velocity.y += v.y; }
	void setXSpeed(float s) { velocity.x = s; }
	void setYSpeed(float s) { velocity.y = s; }
	Vector2f getVelocity() const { return velocity; }
	void move() { CircleShape::move(velocity); }
	int getPlayer() { return player; }
	void setPlayer(int p) { player = p; }
};
