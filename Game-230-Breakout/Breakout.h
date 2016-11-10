#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <memory>
#include "GameConstants.h"
#include "PlayArea.h"
#include "Player.h"
#include "Ball.h"
#include "Brick.h"
#include "Level.h"
#include "LevelManager.h"

using namespace sf;
using namespace std;

class Breakout {
private:
	RenderWindow window;
	PlayArea playArea;
	LevelManager levelManager;
	shared_ptr<Level> currentLevel;
	Player player;
	Ball ball;
	Text score, lives;
	Font font;
	Clock clock;
	Time time1, time2, time3;
	bool gameStart,gameOver,levelEnd;
	void resetPlayer();
	void resetGame();
	void nextLevel();
public:
	Breakout():window(VideoMode(Window_Width,Window_Height),"Breakout",Style::Close|Style::Titlebar),
		playArea(Play_Area_Width,Play_Area_Height),
		player(Vector2f(Play_Area_X_Position+Play_Area_Width/2-Paddle_Width/2,Play_Area_Y_Position+Play_Area_Height-Paddle_Height),Play_Area_X_Position,Play_Area_X_Position+Play_Area_Width),
		ball(Ball_Radius),
		gameStart(false),
		gameOver(false){
		srand(time(NULL));
		window.setPosition(Vector2i(400, 0));
		playArea.setPosition(Vector2f(Play_Area_X_Position, Play_Area_Y_Position));
		
		font.loadFromFile("Tinos-Regular.ttf");
		ostringstream ss;
		ss << "Score: " << player.getScore();
		score.setString(ss.str());
		score.setFont(font);
		score.setCharacterSize(Stat_Character_Size);
		score.setFillColor(Color::Yellow);
		score.setPosition(Score_X_Position, Score_Y_Position);
		ss.str("");
		ss << "Lives: " << player.getLives();
		lives.setString(ss.str());
		lives.setFont(font);
		lives.setCharacterSize(Stat_Character_Size);
		lives.setFillColor(Color::Red);
		lives.setPosition(Life_X_Position,Life_Y_Position);
		Brick::loadTextures();
		//currentLevel.setBricks(string(Config_Folder)+'/'+string("Level1.txt"));
		//currentLevel.setColorAfterBroken(Play_Area_Color);
		currentLevel = levelManager.getFirstLevel();
	}
	void startGame();
};

void Breakout::resetPlayer() {
	gameStart = false;
	player.setPaddlePosition(Play_Area_X_Position + Play_Area_Width / 2 - Paddle_Width / 2, Play_Area_Y_Position + Play_Area_Height - Paddle_Height);
	ball.setPosition(Play_Area_X_Position + Play_Area_Width / 2 - Ball_Radius, Play_Area_Height - Paddle_Height - Ball_Radius * 2);
}

void Breakout::resetGame() {
	resetPlayer();
	currentLevel = levelManager.getFirstLevel();
	player.resetScore();
	player.resetLives();
}

void Breakout::startGame() {
	bool moveLeft = false, moveRight = false;
	bool gameStart = false, gameOver = false, levelClear = false;
	time1 = clock.getElapsedTime();
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			moveLeft = false, moveRight = false;
			if (event.type == Event::Closed)
				window.close();
			if (Keyboard::isKeyPressed(Keyboard::Left)&& gameStart)
				moveLeft = true;
			if (Keyboard::isKeyPressed(Keyboard::Right)&& gameStart)
				moveRight = true;
			if (Keyboard::isKeyPressed(Keyboard::Space) && !gameStart) {
				int initialBallXSpeed = rand() % 2 == 0 ? Ball_Initial_Speed:-Ball_Initial_Speed;
				ball.setXSpeed(initialBallXSpeed);
				ball.setYSpeed(Ball_Initial_Speed);
				gameStart = true;
			}
		}
		time2 = clock.getElapsedTime();
		time3 = time2 - time1;
		if (time3.asSeconds() >= Refresh_Interval) {
			time1 = time2;
			ball.move();
			int i=playArea.interact(ball);
			if (moveLeft)
				player.moveLeft();
			if (moveRight)
				player.moveRight();
			player.interact(ball);
			currentLevel->interact(ball);
			//if(currentLevel->allClear())
		}

		window.clear(Color::Cyan);
		window.draw(playArea);
		currentLevel->render(window);
		window.draw(player.getPaddle());
		window.draw(ball);
		window.draw(score);
		window.draw(lives);
		window.display();
	}
}