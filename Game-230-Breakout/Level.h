#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include "GameConstants.h"
#include "Player.h"
#include "Brick.h"
#include "EarthBrick.h"
#include "WaterBrick.h"
#include "WindBrick.h"
#include "FireBrick.h"
#include "ThunderBrick.h"
#include "NullBrick.h"
#include "PowerUp.h"

using namespace std;
using namespace sf;

class Level {
	friend class EarthBrick;
private:
	vector<int> brickConfig;
	vector<shared_ptr<Brick>> bricks;
	vector<vector<shared_ptr<Brick>>> brickGrid;
	vector<shared_ptr<FireBall>> fireballs;
	vector<shared_ptr<ThunderBall>> thunderballs;
	vector<shared_ptr<VertexArray>> colourShiftPanels;
	shared_ptr<NullBrick> bossBrick;
	vector<shared_ptr<PowerUp>> powerUps;
	Texture explosionTexture;
	RectangleShape explosion;
	string levelName;
	int explosionCounter;
	int explosionTextureX, explosionTextureY;
	int explosionSpeed;
	int initialNumberOfBricks;
	int numberOfBricks;
	bool bossFight;
	bool bossExploded;
	bool isWaterBrickPresent, isFireBrickPresent, isEarthBrickPresent, isWindBrickPresent, isThunderBrickPresent;
	void setEdgeExposure();
	void setPowerUps();
public:
	Level():explosion(Vector2f(Play_Area_Height*2,Play_Area_Height*2)),
	initialNumberOfBricks(0),
	numberOfBricks(0),
	explosionCounter(0),
	explosionTextureX(1),
	explosionTextureY(0),
	explosionSpeed(0),
	bossFight(false),
	bossExploded(false),
	isWaterBrickPresent(false),
	isFireBrickPresent(false), 
	isEarthBrickPresent(false), 
	isWindBrickPresent(false), 
	isThunderBrickPresent(false) {
		explosionTexture.loadFromFile(string(Texture_Folder) + '/' + Texture_Brick_Subfolder + '/' + "explosion.png");
		explosion.setTexture(&explosionTexture);
		explosion.setTextureRect(IntRect(0, 0, 256, 256));
	}
	void loadConfig(string fileName);
	void resetBricks();
	int interact(Ball &ball,Player &player);
	int act(Player &p);
	RectangleShape getExplosion() { return explosion; }
	void explode();
	bool allClear() { return numberOfBricks == 0; }
	void render(RenderWindow &window);
	string getLevelName() { return levelName; }
	void animate();
	void ceaseFire();
	bool isBossDefeated() { return bossFight&&bossBrick->isBroken(); }
	void setExplosionSpeed(int s) { explosionSpeed = s; }
	void breakAll();
	void declearDefeat() { numberOfBricks = 0; bossExploded = false; }
	bool isBossExploded() { return bossExploded; }
};

void Level::setEdgeExposure() {
	for (int i = 0; i < brickGrid.size(); i++) {
		for (int j = 0; j < brickGrid[i].size(); j++) {
			brickGrid[i][j]->setTopExposure(true);
			brickGrid[i][j]->setBottomExposure(true);
			brickGrid[i][j]->setLeftExposure(true);
			brickGrid[i][j]->setRightExposure(true);
			if (i == 0)
				brickGrid[i][j]->setTopExposure(false);
			if (j == 0)
				brickGrid[i][j]->setLeftExposure(false);
			if (j == brickGrid[i].size() - 1)
				brickGrid[i][j]->setRightExposure(false);
			if(i>0&&!brickGrid[i-1][j]->isBroken())
				brickGrid[i][j]->setTopExposure(false);
			if(i<brickGrid.size()-1&&!brickGrid[i + 1][j]->isBroken())
				brickGrid[i][j]->setBottomExposure(false);
			if(j>0&&!brickGrid[i][j-1]->isBroken())
				brickGrid[i][j]->setLeftExposure(false);
			if(j<brickGrid[i].size()-1&&!brickGrid[i][j+1]->isBroken())
				brickGrid[i][j]->setRightExposure(false);
		}
	}
}

void Level::setPowerUps() {
	powerUps.clear();
	int brickIndices[] = { -1,-1,-1,-1,-1 }, i = 0;
	int powerUpSpawnRange = bricks.size() > Number_Of_Brick_Per_Row * 2 ? Number_Of_Brick_Per_Row * 2 : bricks.size();
	while (i < 5) {
		int brickIndex = bricks.size() - 1 - rand() % powerUpSpawnRange;
		int j = 0;
		bool isUnsuitable = false;
		while (j<5) {
			if (!bricks[brickIndex]->isNormal() || bricks[brickIndex]->isBroken() || brickIndices[j] == brickIndex)
				isUnsuitable = true;
			++j;
		}
		if (!isUnsuitable) {
			brickIndices[i] = brickIndex;
			++i;
		}
	}
	if (isWaterBrickPresent) {
		shared_ptr<PowerUp> p = make_shared<PowerUp>(Element::Earth);
		p->setBrickAbove(bricks[brickIndices[0]]);
		powerUps.push_back(p);
	}
	if (isFireBrickPresent) {
		shared_ptr<PowerUp> p = make_shared<PowerUp>(Element::Water);
		p->setBrickAbove(bricks[brickIndices[1]]);
		powerUps.push_back(p);
	}
	if (isEarthBrickPresent) {
		shared_ptr<PowerUp> p = make_shared<PowerUp>(Element::Thunder);
		p->setBrickAbove(bricks[brickIndices[2]]);
		powerUps.push_back(p);
	}
	if (isWindBrickPresent) {
		shared_ptr<PowerUp> p = make_shared<PowerUp>(Element::Fire);
		p->setBrickAbove(bricks[brickIndices[3]]);
		powerUps.push_back(p);
	}
	if (isThunderBrickPresent) {
		shared_ptr<PowerUp> p = make_shared<PowerUp>(Element::Wind);
		p->setBrickAbove(bricks[brickIndices[4]]);
		powerUps.push_back(p);
	}
}

void Level::loadConfig(string fileName) {
	ifstream ifs(fileName);
	getline(ifs,levelName);
	int brickType;
	shared_ptr<FireBrick> b;
	shared_ptr<ThunderBrick> b2;
	shared_ptr<NullBrick> b3;
	while (ifs >> brickType) {
		shared_ptr<Brick> brick;
		switch (brickType) {
		case Element::None:brick = make_shared<Brick>(Brick_Width, Brick_Height, 0, 0,true); brick->setTexture(nullptr);  break;
		case Element::Normal:brick = make_shared<Brick>(Brick_Width, Brick_Height, Brick_Duribility, Normal_Brick_Score); break;
		case Element::Water:brick = make_shared<WaterBrick>(Brick_Width, Brick_Height, Brick_Duribility, Element_Brick_Score, bricks); isWaterBrickPresent = true; break;
		case Element::Fire:b = make_shared<FireBrick>(Brick_Width, Brick_Height, Brick_Duribility, Element_Brick_Score, bricks); fireballs.push_back(b->getFireball()); brick = b; isFireBrickPresent = true; break;
		case Element::Earth:brick = make_shared<EarthBrick>(Brick_Width, Brick_Height, Brick_Duribility, Element_Brick_Score, bricks); isEarthBrickPresent = true; break;
		case Element::Wind:brick = make_shared<WindBrick>(Brick_Width, Brick_Height, Brick_Duribility, Element_Brick_Score, bricks); isWindBrickPresent = true; break;
		case Element::Thunder:b2 = make_shared<ThunderBrick>(Brick_Width, Brick_Height, Brick_Duribility, Element_Brick_Score, bricks); thunderballs.push_back(b2->getThunderball()); brick = b2; isThunderBrickPresent = true; break;
		case Element::Null:b3 = make_shared<NullBrick>(Brick_Width, Brick_Height, Brick_Duribility, Null_Brick_Score, bricks); colourShiftPanels.push_back(b3->getColourShiftPanel()); brick = b3; bossBrick = b3; bossFight = true; break;
		default:brick = make_shared<Brick>(Brick_Width, Brick_Height, 0, 0, true); brick->setTexture(nullptr);  break;
		}
		brick->setPosition((bricks.size() % Number_Of_Brick_Per_Row)*Brick_Width + Play_Area_X_Position, (bricks.size() / Number_Of_Brick_Per_Row)*Brick_Height);
		bricks.push_back(brick);
		if(!brick->isBrickEmpty())
			++numberOfBricks;
	}
	initialNumberOfBricks = numberOfBricks;
	auto ite = bricks.begin();
	for (int i = 0; ite != bricks.end(); ++i) {
		vector<shared_ptr<Brick>> brickRow;
		for (int j = 0; ite != bricks.end()&&j<Number_Of_Brick_Per_Row; ++ite, ++j) {
			brickRow.push_back(*ite);
		}
		brickGrid.push_back(brickRow);
	}
	if (bossFight) {
		explosion.setPosition(bossBrick->getPosition().x+Brick_Width/2-explosion.getSize().x/2,bossBrick->getPosition().y+Brick_Height/2 - explosion.getSize().y / 2);
	}
	setPowerUps();
	setEdgeExposure();
}

void Level::resetBricks() {
	numberOfBricks = initialNumberOfBricks;
	for (shared_ptr<Brick> b : bricks) {
		b->setDisplay();
	}
	for (shared_ptr<Brick> b : bricks)
		if(!b->isNull())
			b->upgradeBricks(true);
	setPowerUps();
	setEdgeExposure();
}

int Level::interact(Ball &ball,Player &player) {
	float xSpeed=ball.getVelocity().x, ySpeed=ball.getVelocity().y,xPosition=ball.getPosition().x,yPosition=ball.getPosition().y;
	int score=0;
	for (shared_ptr<Brick> b : bricks) {
		Interaction i=b->interact(ball);
		if (i.xFlip) {
			xSpeed = -ball.getVelocity().x;
			xPosition = i.xPosition;
		}
		if (i.yFlip) {
			ySpeed = -ball.getVelocity().y;
			yPosition = i.yPosition;
		}
		if (i.score > 0) {
			score += i.score;
			--numberOfBricks;
		}			
	}
	ball.setXSpeed(xSpeed);
	ball.setYSpeed(ySpeed);
	ball.setPosition(xPosition, yPosition);
	setEdgeExposure();
	for (shared_ptr<PowerUp> p : powerUps) {
		p->interact(ball, player);
	}
	return score;
}

int Level::act(Player &p) {
	bool killed=false, stunned=false;
	for (shared_ptr<Brick> b : bricks) {
		if (!b->isBroken()) {
			b->act(p);
		}	
	}
	for (shared_ptr<FireBall> f : fireballs) {
		if (f->isFired() && f->act(p) == Player_Killed)
			killed = true;
	}
	for (shared_ptr<ThunderBall> t : thunderballs) {
		if (t->isFired() && t->act(p) == Player_Stunned)
			stunned = true;
	}
	int returnValue = Player_Intact;
	if (stunned)
		returnValue = Player_Stunned;
	if (killed)
		returnValue = Player_Killed;
	return returnValue;
}

void Level::render(RenderWindow &window) {
	for (shared_ptr<Brick> b : bricks) {
		window.draw(b->getBackground());
	}
	for (shared_ptr<VertexArray> c : colourShiftPanels) {
		window.draw(*c);
	}
	for (shared_ptr<Brick> b : bricks) {
		if(!b->isNormal()&&!b->isNull()&&!b->isBroken())
			window.draw(b->getAnimation());
	}
	for (shared_ptr<Brick> b : bricks) {
		window.draw(*b);
	}
	for (shared_ptr<PowerUp> p : powerUps) {
		if(p->isExposed())
			window.draw(*p);
	}
	for (shared_ptr<FireBall> f : fireballs) {
		if(f->isFired())
			window.draw(*f);
	}
	for (shared_ptr<ThunderBall> t : thunderballs) {
		if(t->isFired())
			window.draw(*t);
	}
}

void Level::animate() {
	for (shared_ptr<Brick> b : bricks) {
		b->animate();
	}
}

void Level::ceaseFire() {
	for (shared_ptr<FireBall> f : fireballs) {
		if (f->isFired())
			f->setFired(false);
	}
	for (shared_ptr<ThunderBall> t : thunderballs) {
		if (t->isFired())
			t->setFired(false);
	}
}

void Level::breakAll() {
	for (shared_ptr<Brick> b : bricks) {
		if (!b->isBrickEmpty())
			b->breakBrick();
	}
}

void Level::explode() {
	if (!bossExploded) {
		if (explosionCounter < explosionSpeed) {
			explosionCounter++;
		}
		else {
			explosionCounter = 0;
			explosion.setTextureRect(IntRect(explosionTextureX * 256, explosionTextureY * 256, 256, 256));
			++explosionTextureX;
			if (explosionTextureX >= Explosion_Texture_Column_Number) {
				++explosionTextureY;
				explosionTextureX = 0;
			}
			if (explosionTextureY >= Explosion_Texture_Row_Number) {
				explosionTextureY = 0;
				bossExploded = true;
			}
		}
	}
}