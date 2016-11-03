#pragma once
#include <SFML\Graphics.hpp>

using namespace sf;

class Brick :public RectangleShape {
private:
	Texture texture;
	int durability;
public:
	Brick(float x, float y,int d) :RectangleShape(Vector2f(x, y)),
	durability(d){

	}
};