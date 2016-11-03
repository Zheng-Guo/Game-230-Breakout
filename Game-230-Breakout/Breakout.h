#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "GameConstants.h"
#include "PlayArea.h"
#include "Player.h"
#include "Ball.h"

using namespace sf;

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
		window.setPosition(Vector2i(400, 0));
		playArea.setPosition(Vector2f(Play_Area_X_Position, Play_Area_Y_Position));
		player.setPaddleColor(Color::Blue);
		font.loadFromFile("Tinos-Regular.ttf");
		score.setString("Score: 0");
		score.setFont(font);
		score.setCharacterSize(Score_Character_Size);
		score.setFillColor(Color::Yellow);
		score.setPosition(Score_X_Position, Score_Y_Position);
		lives.setString("Lives: " + player.getLives());
		lives.setFont(font);
		score.setCharacterSize();
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
		}
		time2 = clock.getElapsedTime();
		time3 = time2 - time1;
		if (time3.asSeconds() >= Refresh_Interval) {
			time1 = time2;
			if (moveLeft)
				player.moveLeft();
			if (moveRight)
				player.moveRight();
		}

		window.clear(Color::Cyan);
		window.draw(playArea);
		window.draw(player.getPaddle());
		window.draw(score);
		window.display();
	}
}