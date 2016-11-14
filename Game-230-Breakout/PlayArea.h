#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "Ball.h"

using namespace sf;

class PlayArea :public RectangleShape {
private:
	SoundBuffer hitWallBuffer;
	Sound hitWallSound;
public:
	PlayArea(float w, float h) :RectangleShape(Vector2f(w, h)) {
		setFillColor(Play_Area_Color);
		hitWallBuffer.loadFromFile(Audio_Folder + string("/hit_wall.wav"));
		hitWallSound.setBuffer(hitWallBuffer);
	}

	int interact(Ball &ball);
};

int PlayArea::interact(Ball &ball) {
	FloatRect ballBound = ball.getGlobalBounds();
	if (ballBound.top + ballBound.height >= Play_Area_Height) {
		return 1;
	}
	if (ballBound.left <= Play_Area_X_Position) {
		ball.setPosition(Play_Area_X_Position, ballBound.top);
		ball.setXSpeed(-ball.getVelocity().x);
		hitWallSound.play();
	}
	if (ballBound.left + ballBound.width >= Play_Area_X_Position + Play_Area_Width) {
		ball.setPosition(Play_Area_X_Position + Play_Area_Width - ballBound.width, ballBound.top);
		ball.setXSpeed(-ball.getVelocity().x);
		hitWallSound.play();
	}
	if (ballBound.top <= Play_Area_Y_Position) {
		ball.setPosition(ballBound.left, Play_Area_Y_Position);
		ball.setYSpeed(-ball.getVelocity().y);
		hitWallSound.play();
	}
	return 0;
}