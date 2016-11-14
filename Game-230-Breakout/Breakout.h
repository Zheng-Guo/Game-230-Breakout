#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <iostream>
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
	RectangleShape statArea;
	RectangleShape blackCurtain;
	RectangleShape redFlash;
	RectangleShape powerUpSelectionHightlight;
	PlayArea playArea;
	LevelManager levelManager;
	shared_ptr<Level> currentLevel;
	Player player;
	Ball ball;
	Text score, lives,message,levelName,restartInstruction,powerUpLabel;
	Font font;
	Clock clock;
	Time time1, time2, time3;
	float ballAcceleration;
	bool gameStart,gameOver,levelEnd;
	bool paddleStunned;
	void resetPlayer();
	void resetGame();
	void nextLevel();
public:
	Breakout() :window(VideoMode(Window_Width, Window_Height), "Breakout", Style::Close | Style::Titlebar),
		statArea(Vector2f(Window_Width - Play_Area_Width, Window_Height)),
		blackCurtain(Vector2f(Play_Area_Width, Black_Curtain_Initial_Height)),
		redFlash(Vector2f(Play_Area_Width, Play_Area_Height)),
		powerUpSelectionHightlight(Vector2f(Power_Up_Selection_Hightlight_Width, Power_Up_Selection_Hightlight_Height)),
		playArea(Play_Area_Width, Play_Area_Height),
		player(Vector2f(Play_Area_X_Position + Play_Area_Width / 2 - Paddle_Width / 2, Play_Area_Y_Position + Play_Area_Height - Paddle_Height), Play_Area_X_Position, Play_Area_X_Position + Play_Area_Width),
		ball(Ball_Radius),
		ballAcceleration(0),
		gameStart(false),
		gameOver(false),
		paddleStunned(false){
		srand(time(NULL));
		window.setPosition(Vector2i(400, 0));
		statArea.setPosition(0, 0);
		statArea.setFillColor(Color::Cyan);
		statArea.setOutlineColor(Color::Black);
		statArea.setOutlineThickness(2);
		blackCurtain.setPosition(Play_Area_X_Position, Play_Area_Height / 2 - Black_Curtain_Initial_Height / 2);
		blackCurtain.setFillColor(Color::Black);
		redFlash.setPosition(Play_Area_X_Position, Play_Area_Y_Position);
		playArea.setPosition(Vector2f(Play_Area_X_Position, Play_Area_Y_Position));
		player.setPaddleColor(Paddle_Normal_Color);
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
		message.setString("");
		message.setFont(font);
		message.setCharacterSize(Message_Character_Size);
		message.setFillColor(Color::Red);
		message.setPosition(Message_X_Position,Message_Y_Position);
		levelName.setString("");
		levelName.setFont(font);
		levelName.setCharacterSize(Level_Name_Character_Size);
		levelName.setFillColor(Color::White);
		levelName.setPosition(Level_Name_X_Position,Level_Name_Y_Position);
		restartInstruction.setString("Press Space to restart the game.");
		restartInstruction.setFont(font);
		restartInstruction.setCharacterSize(Stat_Character_Size);
		restartInstruction.setFillColor(Color::Red);
		restartInstruction.setPosition(Message_X_Position, Message_Y_Position + 100);
		powerUpLabel.setString("Power Up:");
		powerUpLabel.setFont(font);
		powerUpLabel.setCharacterSize(Stat_Character_Size);
		powerUpLabel.setFillColor(Color::Blue);
		Brick::loadTextures();
		currentLevel = levelManager.getFirstLevel();
		currentLevel->setExplosionSpeed(Explosion_Speed);
		powerUpSelectionHightlight.setFillColor(Power_Up_Selection_Hightlight_Color);
		resetPlayer();
	}
	void startGame();
};

void Breakout::resetPlayer() {
	gameStart = false;
	paddleStunned = false;
	currentLevel->ceaseFire();
	player.setPaddlePosition(Play_Area_X_Position + Play_Area_Width / 2 - Paddle_Width / 2, Play_Area_Y_Position + Play_Area_Height - Paddle_Height);
	player.resetPowerUp();
	player.setPaddleColor(Paddle_Normal_Color);
	ball.setPosition(Play_Area_X_Position + Play_Area_Width / 2 - Ball_Radius, Play_Area_Height - Paddle_Height - Ball_Radius * 2);
	ball.setXSpeed(0);
	ball.setYSpeed(0);
	ball.setPowerUpType(Element::Normal);
}

void Breakout::resetGame() {
	resetPlayer();
	player.resetPowerUps();
	gameOver = false;
	currentLevel = levelManager.getFirstLevel();
	player.resetScore();
	player.resetLives();
	ostringstream ss;
	ss << "Score: " << player.getScore();
	score.setString(ss.str());
	ss.str("");
	ss << "Lives: " << player.getLives();
	lives.setString(ss.str());
	ballAcceleration = 0;
}

void Breakout::nextLevel() {
	resetPlayer();
	player.resetPowerUps();
	currentLevel = levelManager.getNextLevel();
	ballAcceleration += Ball_Next_Level_Acceleration;
}

void Breakout::startGame() {
	int startBufferTime=0,endBufferTime,redFlashDuration=255,stunnedCounter,explosionCounter=0;
	bool moveLeft = false, moveRight = false,levelStart=true,loseLife=false,stued=false,bossDefeated=false;
	float ballradius = ball.getRadius();
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
			if (Keyboard::isKeyPressed(Keyboard::Up) && gameStart)
				player.nextPowerUp();
			if (Keyboard::isKeyPressed(Keyboard::Down) && gameStart)
				player.previousPowerUp();
			if (Keyboard::isKeyPressed(Keyboard::Space) && !levelStart && !gameStart&&!loseLife) {
				int initialBallXSpeed = rand() % 2 == 0 ? Ball_Initial_Speed:-Ball_Initial_Speed;
				ball.setXSpeed(initialBallXSpeed);
				ball.setYSpeed(Ball_Initial_Speed+ballAcceleration);
				gameStart = true;
			}
			if (Keyboard::isKeyPressed(Keyboard::Space) && gameOver) {
				message.setString("");
				resetGame();
				levelStart = true;
			}
		}
		time2 = clock.getElapsedTime();
		time3 = time2 - time1;
		if (time3.asSeconds() >= Refresh_Interval&&!gameOver) {
			time1 = time2;
			currentLevel->animate();
			if (bossDefeated) {
				currentLevel->explode();
				++explosionCounter;
				if (explosionCounter == Refresh_Frequency)
					currentLevel->breakAll();
				else if (explosionCounter == Refresh_Frequency*2) {
					currentLevel->declearDefeat();
					explosionCounter = 0;
					bossDefeated = false;
				}				
			}
			else if (levelStart) {
				++startBufferTime;
				if (startBufferTime <= Refresh_Frequency / 2) {
					float y = blackCurtain.getSize().y;
					float x =Play_Area_Width / (Refresh_Frequency / 2)*startBufferTime;
					blackCurtain.setSize(Vector2f(x, y));
				}
				else if(startBufferTime<=Refresh_Frequency){
					float x = blackCurtain.getSize().x;
					float y = (Black_Curtain_Final_Height - Black_Curtain_Initial_Height) / Refresh_Frequency*(startBufferTime - Refresh_Frequency / 2) * 2 + Black_Curtain_Initial_Height;
					blackCurtain.setSize(Vector2f(x, y));
					blackCurtain.setPosition(blackCurtain.getPosition().x, Play_Area_Height / 2 - blackCurtain.getSize().y / 2);
				}
				else if (startBufferTime <= Refresh_Frequency * 3) {
					levelName.setString(currentLevel->getLevelName());
				}
				else if (startBufferTime <= Refresh_Frequency / 2 * 7) {
					levelName.setString("");
					float x = blackCurtain.getSize().x;
					float y = (Black_Curtain_Final_Height - Black_Curtain_Initial_Height) / Refresh_Frequency*(Refresh_Frequency / 2*7-startBufferTime) * 2 + Black_Curtain_Initial_Height;
					blackCurtain.setSize(Vector2f(x, y));
					blackCurtain.setPosition(blackCurtain.getPosition().x, Play_Area_Height / 2 - blackCurtain.getSize().y / 2);
				}
				else if (startBufferTime <= Refresh_Frequency * 4) {
					float y = blackCurtain.getSize().y;
					float x = Play_Area_Width / (Refresh_Frequency / 2)*(Refresh_Frequency*4-startBufferTime);
					blackCurtain.setSize(Vector2f(x, y));
				}
				else {
					startBufferTime = 0;
					levelStart = false;
				}
			}
			else if (loseLife) {
				redFlash.setFillColor(Color(255, 0, 0, redFlashDuration));
				--redFlashDuration;
				if (redFlashDuration == 0) {
					loseLife = false;
					redFlashDuration = 255;
				}
			}
			else if (!levelEnd) {
				if (ball.getRadius() != ballradius)
					cout << "Ball size changed: " << ball.getRadius() << endl;
				ball.move();
				int i = playArea.interact(ball);
				if (i == 1) {
					player.lostLife();
					moveLeft = false, moveRight = false;
					if (player.getLives() >= 1) {
						resetPlayer();
						ostringstream ss;
						ss << "Lives: " << player.getLives();
						lives.setString(ss.str());
						loseLife = true;
					}
					else {
						message.setString("Game Over");
						gameOver = true;
					}
				}
				if (paddleStunned) {
					if (stunnedCounter > 0)
						--stunnedCounter;
					else {
						paddleStunned = false;
						player.setPaddleColor(Paddle_Normal_Color);
					}
				}
				else {
					if (moveLeft)
						player.moveLeft();
					if (moveRight)
						player.moveRight();
				}
				player.interact(ball);
				int s = currentLevel->interact(ball,player);
				if (s > 0) {
					player.scorePoint(s);
					ostringstream ss;
					ss << "Score: " << player.getScore();
					score.setString(ss.str());
				}
				if (currentLevel->allClear()) {
					endBufferTime = 0;
					moveLeft = false, moveRight = false;
					gameStart = false, levelEnd = true;
				}
				if (currentLevel->isBossDefeated()) {
					bossDefeated = true;
				}
				if (gameStart) {
					i = currentLevel->act(player);
					if (i == 1) {
						player.lostLife();
						moveLeft = false, moveRight = false;
						if (player.getLives() >= 1) {
							resetPlayer();
							ostringstream ss;
							ss << "Lives: " << player.getLives();
							lives.setString(ss.str());
							loseLife = true;
						}
						else {
							message.setString("Game Over");
							gameOver = true;
						}
					}
					if (i == 2) {
						paddleStunned = true;
						stunnedCounter = Player_Stunned_Duration;
						player.setPaddleColor(Paddle_Stunned_Color);
					}
				}
			}
			else {
				++endBufferTime;
				if (endBufferTime <= Refresh_Frequency * 2) {
					message.setString("All Clear");
				}
				else if (endBufferTime <= Refresh_Frequency / 2 * 5) {
					message.setString("");
					float y = blackCurtain.getSize().y;
					float x = Play_Area_Width / (Refresh_Frequency / 2)*(endBufferTime-Refresh_Frequency*2);
					blackCurtain.setSize(Vector2f(x, y));
				}
				else if (endBufferTime <= Refresh_Frequency * 3) {
					float x = blackCurtain.getSize().x;
					float y = (Play_Area_Height - Black_Curtain_Initial_Height) / Refresh_Frequency*(endBufferTime - Refresh_Frequency /2*5) * 2 + Black_Curtain_Initial_Height;
					blackCurtain.setSize(Vector2f(x, y));
					blackCurtain.setPosition(blackCurtain.getPosition().x, Play_Area_Height / 2 - blackCurtain.getSize().y / 2);
				}
				else if (endBufferTime <= Refresh_Frequency * 4) {
					float x= Play_Area_Width / Refresh_Frequency *(Refresh_Frequency * 4-endBufferTime);
					float y = blackCurtain.getSize().y;
					blackCurtain.setSize(Vector2f(x, y));
				}
				if (endBufferTime == Refresh_Frequency * 3) {
					nextLevel();
				}
				if (endBufferTime == Refresh_Frequency * 4) {
					levelEnd = false;
					levelStart = true;
					blackCurtain.setPosition(Play_Area_X_Position, Play_Area_Height / 2 - Black_Curtain_Initial_Height / 2);
					blackCurtain.setSize(Vector2f(0, Black_Curtain_Initial_Height));
				}
			}
		}
		window.clear();
		window.draw(playArea);
		currentLevel->render(window);
		window.draw(player.getPaddle());
		window.draw(ball);
		window.draw(message);
		if (levelStart) {
			window.draw(blackCurtain);
			window.draw(levelName);
		}
		if (levelEnd)
			window.draw(blackCurtain);
		if (gameOver) 
			window.draw(restartInstruction);
		if (loseLife)
			window.draw(redFlash);
		if (bossDefeated&&!currentLevel->isBossExploded())
			window.draw(currentLevel->getExplosion());
		window.draw(statArea);
		window.draw(score);
		window.draw(lives);
		powerUpSelectionHightlight.setPosition(player.getAcquiaredPowerUps()[player.getSelectedPowerUp()].getPosition().x - 10, player.getAcquiaredPowerUps()[player.getSelectedPowerUp()].getPosition().y - 10);
		window.draw(powerUpSelectionHightlight);
		for (CircleShape c :player.getAcquiaredPowerUps()) {
			window.draw(c);
		}
		window.draw(player.getSelectedPowerUpDescription());
		powerUpLabel.setPosition(Power_Up_Label_X, Play_Area_Height- player.getAcquiaredPowerUps().size()*Power_Up_Display_Interval - 50);
		window.draw(powerUpLabel);
		window.display();
	}
}