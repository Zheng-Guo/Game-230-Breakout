#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include "GameConstants.h"
#include "Brick.h"
#include "EarthBrick.h"
#include "WaterBrick.h"
#include "WindBrick.h"

using namespace std;
using namespace sf;

class Level {
	friend class EarthBrick;
private:
	vector<int> brickConfig;
	vector<shared_ptr<Brick>> bricks;
	vector<vector<shared_ptr<Brick>>> brickGrid;
	string levelName;
	int initialNumberOfBricks;
	int numberOfBricks;
	void setEdgeExposure();
public:
	//Level(){}
	void loadConfig(string fileName);
	void resetBricks();
	int interact(Ball &ball);
	void act(Ball &ball, Paddle &paddle);
	bool allClear() { return numberOfBricks == 0; }
	void render(RenderWindow &window);
	string getLevelName() { return levelName; }
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

void Level::loadConfig(string fileName) {
	ifstream ifs(fileName);
	getline(ifs,levelName);
	int brickType;
	while (ifs >> brickType) {
		shared_ptr<Brick> brick;
		switch (brickType) {
		case Element::None:brick = make_shared<Brick>(Brick_Width, Brick_Height, 0, 0,true); brick->setTexture(nullptr);  break;
		case Element::Normal:brick = make_shared<Brick>(Brick_Width, Brick_Height, Brick_Duribility, 10); break;
		case Element::Water:brick= make_shared<WaterBrick>(Brick_Width, Brick_Height, Brick_Duribility, 20,bricks); break;
		case Element::Earth:brick = make_shared<EarthBrick>(Brick_Width, Brick_Height, Brick_Duribility, 20,bricks); break;
		case Element::Wind:brick = make_shared<WindBrick>(Brick_Width, Brick_Height, Brick_Duribility, 20, bricks); break;
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
		for (int j = 0; ite != bricks.end(); ++ite, ++j) {
			brickRow.push_back(*ite);
		}
		brickGrid.push_back(brickRow);
	}
	setEdgeExposure();
}

void Level::resetBricks() {
	numberOfBricks = initialNumberOfBricks;
	for (shared_ptr<Brick> b : bricks) {
		b->setDisplay();
	}
	for (shared_ptr<Brick> b : bricks)
		b->upgradeBricks(true);
}

int Level::interact(Ball &ball) {
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
	return score;
}

void Level::act(Ball &ball, Paddle &paddle) {
	for (shared_ptr<Brick> b : bricks) {
		b->act( ball, paddle);
	}
}

void Level::render(RenderWindow &window) {
	for (shared_ptr<Brick> b : bricks) {
		window.draw(b->getBackground());
	}
	for (shared_ptr<Brick> b : bricks) {
		if(!b->isNormal()&&!b->isBroken())
			window.draw(b->getAnimation());
	}
	for (shared_ptr<Brick> b : bricks) {
		window.draw(*b);
	}
}