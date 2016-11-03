#pragma once
#include <SFML\Graphics.hpp>
#include "Ball.h"

using namespace sf;

class PlayArea :public RectangleShape {
private:
public:
	PlayArea(float w, float h) :RectangleShape(Vector2f(w, h)) {
		setFillColor(Color::Green);
		setOutlineColor(Color::Black);
		setOutlineThickness(2);
	}

	int interact(Ball &ball);
};

int PlayArea::interact(Ball &ball) {
	FloatRect ballBound = ball.getGlobalBounds();
	if (ballBound.top + ballBound.height >= Play_Area_Height)
		return 1;
	if (ballBound.left <= Play_Area_X_Position) {
		ball.setPosition(Play_Area_X_Position, ballBound.top);
		ball.setXSpeed(-ball.getVelocity().x);
	}
	if (ballBound.left + ballBound.width >= Play_Area_X_Position + Play_Area_Width) {
		ball.setPosition(Play_Area_X_Position + Play_Area_Width - ballBound.width, ballBound.top);
		ball.setXSpeed(-ball.getVelocity().x);
	}
	if (ballBound.top <= Play_Area_Y_Position) {
		ball.setPosition(ballBound.left, Play_Area_Y_Position);
		ball.setYSpeed(-ball.getVelocity().y);
	}
	return 0;
}