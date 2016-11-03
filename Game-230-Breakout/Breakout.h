#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include "GameConstants.h"
#include "PlayArea.h"
#include "Player.h"
#include "Ball.h"

using namespace sf;
using namespace std;

class Breakout {
private:
	RenderWindow window;
	PlayArea playArea;
	Player player;
	Ball ball;
	Text score, lives;
	Font font;
	Clock clock;
	Time time1, time2, time3;
public:
	Breakout():window(VideoMode(Window_Width,Window_Height),"Breakout",Style::Close|Style::Titlebar),
		playArea(Play_Area_Width,Play_Area_Height),
		player(Vector2f(Play_Area_X_Position+Play_Area_Width/2-Paddle_Width/2,Play_Area_Y_Position+Play_Area_Height-Paddle_Height),Play_Area_X_Position,Play_Area_X_Position+Play_Area_Width),
		ball(Ball_Radius){
		srand(time(NULL));
		window.setPosition(Vector2i(400, 0));
		playArea.setPosition(Vector2f(Play_Area_X_Position, Play_Area_Y_Position));
		player.setPaddleColor(Color::Blue);
		int initialBallXPosition = rand() % (int)(Paddle_Width - Ball_Radius * 2)+Play_Area_X_Position+Play_Area_Width/2-Paddle_Width/2;
		ball.setPosition(initialBallXPosition, Play_Area_Height - Paddle_Height - Ball_Radius * 2);
		font.loadFromFile("Tinos-Regular.ttf");
		stringstream ss;
		string s;
		ss << "Score: " << player.getScore();
		ss >> s;
		score.setString(s);
		score.setFont(font);
		score.setCharacterSize(Stat_Character_Size);
		score.setFillColor(Color::Yellow);
		score.setPosition(Score_X_Position, Score_Y_Position);
		ss.clear();
		ss << "Lives: " << player.getLives();
		ss >> s;
		lives.setString(s);
		lives.setFont(font);
		lives.setCharacterSize(Stat_Character_Size);
		lives.setFillColor(Color::Red);
		lives.setPosition(Life_X_Position,Life_Y_Position);
	}
	void startGame();
};

void Breakout::startGame() {
	bool moveLeft, moveRight;
	time1 = clock.getElapsedTime();
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			moveLeft = false, moveRight = false;
			if (event.type == Event::Closed)
				window.close();
			if (Keyboard::isKeyPressed(Keyboard::Left))
				moveLeft = true;
			if (Keyboard::isKeyPressed(Keyboard::Right))
				moveRight = true;
			if (Keyboard::isKeyPressed(Keyboard::Space))
				ball.setYSpeed(Ball_Initial_Y_Speed);
		}
		time2 = clock.getElapsedTime();
		time3 = time2 - time1;
		if (time3.asSeconds() >= Refresh_Interval) {
			time1 = time2;
			ball.move();
			if (moveLeft)
				player.moveLeft();
			if (moveRight)
				player.moveRight();
		}

		window.clear(Color::Cyan);
		window.draw(playArea);
		window.draw(player.getPaddle());
		window.draw(ball);
		window.draw(score);
		window.draw(lives);
		window.display();
	}
}