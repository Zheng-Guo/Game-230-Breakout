#pragma once
#include <SFML\Graphics.hpp>
#include "Ball.h"

using namespace sf;

class PowerUp:public RectangleShape{
private:
	Texture texture;
	bool present;
public:
	PowerUp():present(false){
		texture.loadFromFile("Present.png");
		setTexture(&texture);
		setSize(Vector2f(60, 60));	
	}
	int interact(Ball &ball);
	void setPresent(bool p) { present = p; }
	bool getPresent() { return present; }
};

int PowerUp::interact(Ball &ball) {//Return the player that obtains the power up by hitting the ball at it.
	FloatRect ballBound = ball.getGlobalBounds();
	FloatRect powerUpBound = getGlobalBounds();
	if (ballBound.intersects(powerUpBound)) {
		return ball.getPlayer();
	}
	return 0;
}