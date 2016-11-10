#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <memory>
#include <string>
#include "GameConstants.h"
#include "Brick.h"

using namespace std;
using namespace sf;

class Level {
private:
	vector<int> brickConfig;
	vector<shared_ptr<Brick>> bricks;
	vector<vector<shared_ptr<Brick>>> brickGrid;
	string levelName;
	int numberOfBricks;
	void setEdgeExposure();
public:
	//Level(){}
	void loadConfig(string fileName);
	void resetBricks();
	void setColorAfterBroken(Color c);
	int interact(Ball &ball);
	bool allClear() { return numberOfBricks == 0; }
	void render(RenderWindow &window);
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
	ifs >> levelName;
	int brickType;
	while (ifs >> brickType) {
		shared_ptr<Brick> brick;
		switch (brickType) {
		case 1:brick = make_shared<Brick>(Brick_Width, Brick_Height, Brick_Duribility, 10); break;
		}
		brick->setPosition((bricks.size() % Number_Of_Brick_Per_Row)*Brick_Width + Play_Area_X_Position, (bricks.size() / Number_Of_Brick_Per_Row)*Brick_Height);
		bricks.push_back(brick);
		++numberOfBricks;
	}
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
	for (shared_ptr<Brick> b : bricks) {
		b->setDisplay();
	}
}

void Level::setColorAfterBroken(Color c) {
	for (shared_ptr<Brick> b : bricks) {
		b->setFillColor(c);
	}
}
int Level::interact(Ball &ball) {
	float xSpeed=ball.getVelocity().x, ySpeed=ball.getVelocity().y;
	for (shared_ptr<Brick> b : bricks) {
		pair<bool,bool> velocityFlip=b->interact(ball);
		if (velocityFlip.first)
			xSpeed = -ball.getVelocity().x;
		if (velocityFlip.second)
			ySpeed = -ball.getVelocity().y;
		if (velocityFlip.first || velocityFlip.second)
			--numberOfBricks;
	}
	ball.setXSpeed(xSpeed);
	ball.setYSpeed(ySpeed);
	setEdgeExposure();
	return 1;
}

void Level::render(RenderWindow &window) {
	for (shared_ptr<Brick> b : bricks) {
		window.draw(b->getBackground());
		window.draw(*b);
	}
}