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

class ThunderBall :public CircleShape {
private:
	Texture fireballTexture;
	Vector2f velocity;
	bool fired;
public:
	ThunderBall(float r) :CircleShape(r),
		velocity(0, 0),
		fired(false) {
		ostringstream ss;
		ss.str("");
		ss << Texture_Folder << '/' << Texture_Brick_Subfolder << "/thunderball.png";
		fireballTexture.loadFromFile(ss.str());
		setTexture(&fireballTexture);
	}
	void setFired(bool b) { fired = b; }
	bool isFired() { return fired; }
	void setVelocity(float x, float y) { velocity.x = x; velocity.y = y; }
	void move() { CircleShape::move(velocity); }
	int act(Player &player);
};

int ThunderBall::act(Player &player) {
	if (fired) {
		move();
		if (getPosition().y > Play_Area_Height) {
			fired = false;
		}
		else if (getGlobalBounds().intersects(player.getPaddle().getGlobalBounds())) {
			fired = false;
			return Player_Stunned;
		}
	}
	return Player_Intact;
}

class ThunderBrick :public Brick {
private:
	vector<shared_ptr<Brick>> &bricks;
	const int animationRefreshRate;
	int refreshCounter;
	vector<Texture>::iterator currentTexture;
	shared_ptr<ThunderBall> thunderball;
	Texture fireballTexture;
	int fireCounter;
	int attackInterval;
public:
	ThunderBrick(float x, float y, int d, int s, vector<shared_ptr<Brick>> &b, bool e = false) :Brick(x, y, d, s),
		bricks(b),
		animationRefreshRate(Refresh_Frequency / 10),
		refreshCounter(0),
		fireCounter(0) {
		background.setFillColor(Thunder_Brick_Background_Color);
		Texture texture;
		ostringstream ss;
		for (int i = 1; i <= Thunder_Texture_Number; i++) {
			ss.str("");
			ss << Texture_Folder << '/' << Texture_Brick_Subfolder << "/thunder" << i << ".png";
			texture.loadFromFile(ss.str());
			animationTextures.push_back(texture);
		}
		currentTexture = animationTextures.begin();
		animation.setTexture(&(*currentTexture));
		thunderball = make_shared<ThunderBall>(Thunder_Ball_Radius);
		attackInterval = Thunder_Ball_Attack_Interval + rand() % (Thunder_Ball_Attack_Margin*Refresh_Frequency);
	}
	virtual Interaction interact(Ball &ball);
	virtual void act(Player &p);
	virtual void upgradeBricks(bool upgrade) {}
	virtual void setDisplay();
	virtual bool isNormal() { return false; }
	virtual void animate();
	virtual bool isNull() { return false; }
	virtual void breakBrick();
	shared_ptr<ThunderBall> getThunderball() { return thunderball; }
};

Interaction ThunderBrick::interact(Ball &ball) {
	Interaction i = bounce(ball);
	int damage = 0;
	if (i.xFlip || i.yFlip) {
		int damage = Brick_Duribility / 4;
		if (nullUpgraded) {
			if (ball.getPowerUpType() == Element::Normal)
				damage = 1;
			else
				damage = 0;
		}
		else if (ball.getPowerUpType() == Element::Earth)
			damage /= 2;
		else if (ball.getPowerUpType() == Element::Wind)
			damage = Brick_Duribility;
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
			breakBrick();
			i.score = score;
		}
		if (durability > 0)
			crackSound.play();
		else
			breakSound.play();
	}
	return i;
}

void ThunderBrick::act(Player &p) {
	if (fireCounter < attackInterval) {
		++fireCounter;
	}
	else {
		attackInterval = Thunder_Ball_Attack_Interval + rand() % (Thunder_Ball_Attack_Margin*Refresh_Frequency);
		Paddle paddle = p.getPaddle();
		fireCounter = 0;
		thunderball->setPosition(getPosition().x + getSize().x / 2 - thunderball->getRadius(), getPosition().y + getSize().y / 2 - thunderball->getRadius());
		float yOffset = Play_Area_Height - (getPosition().y + getSize().y / 2);
		float xOffset = paddle.getPosition().x + paddle.getSize().x / 2 - getPosition().x - getSize().x / 2;
		thunderball->setVelocity(Thunder_Ball_Y_Speed / yOffset*xOffset, Fire_Ball_Y_Speed);
		thunderball->setFired(true);
	}
}

void ThunderBrick::setDisplay() {
	if (!isEmpty) {
		durability = Brick_Duribility;
		setTexture(&textures[0]);
		fireCounter = 0;
		thunderball->setFired(false);
	}
}

void ThunderBrick::animate() {
	if (refreshCounter < animationRefreshRate) {
		++refreshCounter;
	}
	else {
		refreshCounter = 0;
		currentTexture++;
		if (currentTexture == animationTextures.end())
			currentTexture = animationTextures.begin();
		animation.setTexture(&(*currentTexture));
	}
}

void ThunderBrick::breakBrick() {
	durability = 0;
	setTexture(nullptr);
}