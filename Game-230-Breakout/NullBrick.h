#pragma once
#include <SFML\Graphics.hpp>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include "GameConstants.h"
#include "Brick.h"

using namespace sf;
using namespace std;

class NullBrick :public Brick {
private:
	SoundBuffer explodeBuffer;
	Sound explodeSound;
	vector<shared_ptr<Brick>> &bricks;
	const int animationRefreshRate;
	shared_ptr<VertexArray> colourShiftPanel;
	int refreshCounter;
	float colours[4][3], colourShifts[4][3];
	int cloakCounter;
	int cloakIntervalCounter;
	int cloakInterval;
	bool cloakActivated;
	bool darkening;
	void initializeColourShift();
public:
	NullBrick(float x, float y, int d, int s, vector<shared_ptr<Brick>> &b, bool e = false) :Brick(x, y, d, s),
		bricks(b),
		animationRefreshRate(Refresh_Frequency / 100),
		refreshCounter(0),
		cloakCounter(0),
		cloakIntervalCounter(0),
		cloakActivated(false),
		darkening(false){
		colourShiftPanel = make_shared<VertexArray>(Quads, 4);
		FloatRect brickBound = getGlobalBounds();
		srand(time(NULL));
		(*colourShiftPanel)[0].position = Vector2f(brickBound.left,brickBound.top);
		(*colourShiftPanel)[1].position = Vector2f(brickBound.left+brickBound.width, brickBound.top);
		(*colourShiftPanel)[2].position = Vector2f(brickBound.left + brickBound.width, brickBound.top+brickBound.height);
		(*colourShiftPanel)[3].position = Vector2f(brickBound.left, brickBound.top + brickBound.height);
		(*colourShiftPanel)[0].color = Color(rand() % 255, rand() % 255, rand() % 255);
		(*colourShiftPanel)[1].color = Color(rand() % 255, rand() % 255, rand() % 255);
		(*colourShiftPanel)[2].color = Color(rand() % 255, rand() % 255, rand() % 255);
		(*colourShiftPanel)[3].color = Color(rand() % 255, rand() % 255, rand() % 255);
		initializeColourShift();
		cloakInterval = Null_Cloak_Interval + rand() % Null_Cloak_Margin;
		explodeBuffer.loadFromFile(Audio_Folder + string("/explode.wav"));
		explodeSound.setBuffer(explodeBuffer);
	}
	virtual Interaction interact(Ball &ball);
	virtual void act(Player &p);
	virtual void upgradeBricks(bool upgrade);
	virtual void setDisplay();
	virtual bool isNormal() { return false; }
	virtual void animate();
	virtual bool isNull() { return true; }
	shared_ptr<VertexArray> getColourShiftPanel() { return colourShiftPanel; }
	virtual void setPosition(float x, float y);
	bool isCloakActivated() { return cloakActivated; }
};

void NullBrick::initializeColourShift() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			colours[i][j] = rand() % 255;
		}
	}
	for (int i = 0; i < 4; i++) {
		if ((*colourShiftPanel)[i].color.r < colours[i][0])
			colourShifts[i][0] = 1;
		else
			colourShifts[i][0] = -1;
		if ((*colourShiftPanel)[i].color.g < colours[i][1])
			colourShifts[i][1] = 1;
		else
			colourShifts[i][1] = -1;
		if ((*colourShiftPanel)[i].color.b < colours[i][2])
			colourShifts[i][2] = 1;
		else
			colourShifts[i][2] = -1;
	}
}

Interaction NullBrick::interact(Ball &ball) {
	Interaction i = bounce(ball);
	int damage = 0;
	if (i.xFlip || i.yFlip) {
		if (ball.getPowerUpType() == Element::Normal)
			damage = 1;
		else
			damage = 0;
		durability -= damage;
		if (durability > Brick_Duribility / 4 * 3)
			setTexture(&textures[0]);
		else if (durability > Brick_Duribility / 2)
			setTexture(&textures[1]);
		else if (durability > Brick_Duribility / 4)
			setTexture(&textures[2]);
		else if (durability >0)
			setTexture(&textures[3]);
		else {
			durability = 0;
			setTexture(nullptr);
			i.score = score;
		}
		if (durability > 0)
			crackSound.play();
		else
			explodeSound.play();
	}
	return i;
}

void NullBrick::act(Player &p) {
	if (cloakActivated) {
		if (cloakCounter == 255) {
			darkening = false;
			(*colourShiftPanel)[0].position = Vector2f(Play_Area_X_Position, Play_Area_Y_Position);
			(*colourShiftPanel)[1].position = Vector2f(Play_Area_X_Position+Play_Area_Width, Play_Area_Y_Position);
			(*colourShiftPanel)[2].position = Vector2f(Play_Area_X_Position + Play_Area_Width,bricks[bricks.size()-1]->getPosition().y+Brick_Height);
			(*colourShiftPanel)[3].position = Vector2f(Play_Area_X_Position, bricks[bricks.size() - 1]->getPosition().y + Brick_Height);
			initializeColourShift();
			upgradeBricks(true);
		}
		else if (cloakCounter == 255 + Null_Cloak_Duration) {
			darkening = true;
		}
		else if (cloakCounter == 255 * 2 + Null_Cloak_Duration) {
			darkening = false;
			FloatRect brickBound = getGlobalBounds();
			(*colourShiftPanel)[0].position = Vector2f(brickBound.left, brickBound.top);
			(*colourShiftPanel)[1].position = Vector2f(brickBound.left + brickBound.width, brickBound.top);
			(*colourShiftPanel)[2].position = Vector2f(brickBound.left + brickBound.width, brickBound.top + brickBound.height);
			(*colourShiftPanel)[3].position = Vector2f(brickBound.left, brickBound.top + brickBound.height);
			initializeColourShift();
			upgradeBricks(false);
			cloakActivated = false;
		}
		++cloakCounter;
	}
	else {
		if (cloakIntervalCounter < cloakInterval) {
			++cloakIntervalCounter;
		}
		else {
			cloakInterval = Null_Cloak_Interval + rand() % Null_Cloak_Margin;
			cloakIntervalCounter = 0;
			cloakCounter = 0;
			darkening = true;
			cloakActivated = true;
		}
	}
}

void NullBrick::upgradeBricks(bool upgrade) {
	for (shared_ptr<Brick> b : bricks) {
		if (!b->isBroken()&&!b->isNull())
			b->setNullUpgrade(upgrade);
	}
}

void NullBrick::setDisplay() {
	if (!isEmpty) {
		durability = Brick_Duribility;
		setTexture(&textures[0]);
		initializeColourShift();
	}
}

void NullBrick::animate() {
	if (darkening) {
		for (int i = 0; i < 4; i++) {
			if ((*colourShiftPanel)[i].color.r > 0)
				--(*colourShiftPanel)[i].color.r;
			if ((*colourShiftPanel)[i].color.g > 0)
				--(*colourShiftPanel)[i].color.g;
			if ((*colourShiftPanel)[i].color.b > 0)
				--(*colourShiftPanel)[i].color.b;
		}
	}
	else {
		if (refreshCounter < animationRefreshRate) {
			++refreshCounter;
		}
		else {
			refreshCounter = 0;
			for (int i = 0; i < 4; i++) {
				if (colourShifts[i][0] > 0 && colours[i][0] <= (*colourShiftPanel)[i].color.r || colourShifts[i][0] < 0 && colours[i][0] >= (*colourShiftPanel)[i].color.r) {
					colours[i][0] = rand() % 255;
					if (colours[i][0] < (*colourShiftPanel)[i].color.r)
						colourShifts[i][0] = -1;
					else
						colourShifts[i][0] = 1;
				}
				else
					(*colourShiftPanel)[i].color.r += colourShifts[i][0];
				if (colourShifts[i][1] > 0 && colours[i][1] <= (*colourShiftPanel)[i].color.g || colourShifts[i][1] < 0 && colours[i][1] >= (*colourShiftPanel)[i].color.g) {
					colours[i][1] = rand() % 255;
					if (colours[i][1] < (*colourShiftPanel)[i].color.g)
						colourShifts[i][1] = -1;
					else
						colourShifts[i][1] = 1;
				}
				else
					(*colourShiftPanel)[i].color.g += colourShifts[i][1];
				if (colourShifts[i][2] > 0 && colours[i][2] <= (*colourShiftPanel)[i].color.b || colourShifts[i][2] < 0 && colours[i][2] >= (*colourShiftPanel)[i].color.b) {
					colours[i][2] = rand() % 255;
					if (colours[i][2] < (*colourShiftPanel)[i].color.b)
						colourShifts[i][2] = -1;
					else
						colourShifts[i][2] = 1;
				}
				else
					(*colourShiftPanel)[i].color.b += colourShifts[i][2];
			}
		}
	}
}

void NullBrick::setPosition(float x, float y) {
	Brick::setPosition(x, y);
	FloatRect brickBound = getGlobalBounds();
	(*colourShiftPanel)[0].position = Vector2f(x, y);
	(*colourShiftPanel)[1].position = Vector2f(x+brickBound.width, y);
	(*colourShiftPanel)[2].position = Vector2f(x + brickBound.width, y+brickBound.height);
	(*colourShiftPanel)[3].position = Vector2f(x, y + brickBound.height);
}