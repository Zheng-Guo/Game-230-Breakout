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
	Player player;
	Ball ball;
	Clock clock;
	Time time1, time2, time3;
public:
	Breakout():window(VideoMode(Window_Width,Window_Height),"Breakout",Style::Close|Style::Titlebar),
	player(Vector2f(Play_Area_X_Position+Play_Area_Width/2-Paddle_Width/2,Play_Area_Y_Position+Play_Area_Height-Paddle_Height),Play_Area_X_Position,Play_Area_X_Position+Play_Area_Width),
	ball(Ball_Radius){
		window.setPosition(Vector2i(400, 0));
	}
	void startGame();
};

void Breakout::startGame() {
	time1 = clock.getElapsedTime();
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
		}
		time2 = clock.getElapsedTime();
		time3 = time2 - time1;
		if (time3.asSeconds() >= Refresh_Interval) {
			time1 = time2;
		}

		window.clear(Color::Green);
		window.display();
	}
}