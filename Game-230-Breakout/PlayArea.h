#pragma once
#include <SFML\Graphics.hpp>

using namespace sf;

class PlayArea :public RectangleShape {
private:
public:
	PlayArea(float w, float h) :RectangleShape(Vector2f(w, h)) {
		setFillColor(Color::Green);
		setOutlineColor(Color::Black);
		setOutlineThickness(2);
	}
};