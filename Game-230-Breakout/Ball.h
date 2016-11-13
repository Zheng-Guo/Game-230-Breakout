#pragma once
#include <SFML\Graphics.hpp>
#include <string>

using namespace sf;
using namespace std;

class Ball :public CircleShape{
private:
	Texture textures[5];
	Vector2f velocity;
	Element powerUpType;//The current power up applied to the ball
public:
	Ball(float r=0,float x=0,float y=0) :CircleShape(r),
		powerUpType(Element::Normal) {
		setPosition(x, y);
		setFillColor(Ball_Initial_Color);
		string texturePath = string(Texture_Folder) + '/' + string(Texture_Ball_Subfolder) + '/';
		textures[0].loadFromFile(texturePath + "water_ball.png");
		textures[1].loadFromFile(texturePath + "fire_ball.png");
		textures[2].loadFromFile(texturePath + "earth_ball.png");
		textures[3].loadFromFile(texturePath + "wind_ball.png");
		textures[4].loadFromFile(texturePath + "thunder_ball.png");
	}
	void accelerate(Vector2f v) { velocity.x += v.x; velocity.y += v.y; }
	void setXSpeed(float s) { velocity.x = s; }
	void setYSpeed(float s) { velocity.y = s; }
	Vector2f getVelocity() const { return velocity; }
	void move() { CircleShape::move(velocity); }
	int getPowerUpType() { return powerUpType; }
	void setPowerUpType(Element p);
};

void Ball::setPowerUpType(Element p) {
	powerUpType = p;
	switch (p) {
	case Element::Normal:setTexture(nullptr); break;
	case Element::Water:setTexture(&textures[0]); break;
	case Element::Fire:setTexture(&textures[1]); break;
	case Element::Earth:setTexture(&textures[2]); break;
	case Element::Wind:setTexture(&textures[3]); break;
	case Element::Thunder:setTexture(&textures[4]); break;
	}
}