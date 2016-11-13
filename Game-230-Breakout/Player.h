#pragma once
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <SFML\Graphics.hpp>
#include "GameConstants.h"
#include "Paddle.h"

using namespace sf;

class Player {
private:
	Paddle paddle;
	vector<Texture> powerUpTextures;
	vector<CircleShape> acquiredPowerUps;
	vector<Element> powerUps;
	vector<Text> powerUpDescriptions;
	Font font;
	int powerUpSelected;
	int score;
	int life;
	float leftBound, rightBound;
	bool poweredUp, powerUpUsed;
public:
	Player(Vector2f position,float l,float r) :paddle(Paddle_Width, Paddle_Height, Deflection_Coefficient),
		score(0),
		life(Player_Initial_Lives),
		leftBound(l),
		rightBound(r),
		powerUpSelected(0),
		poweredUp(false),
		powerUpUsed(false){
		paddle.setOutlineColor(Color::Black);
		paddle.setPosition(position);
		paddle.setSpeed(Paddle_Speed);
		Texture texture;
		ostringstream ss;
		ss.str("");
		ss << Texture_Folder << '/' << Texture_PowerUp_Subfolder << "/Water powerup.png";
		texture.loadFromFile(ss.str());
		powerUpTextures.push_back(texture);
		ss.str("");
		ss << Texture_Folder << '/' << Texture_PowerUp_Subfolder << "/Fire powerup.png";
		texture.loadFromFile(ss.str());
		powerUpTextures.push_back(texture);
		ss.str("");
		ss << Texture_Folder << '/' << Texture_PowerUp_Subfolder << "/Earth powerup.png";
		texture.loadFromFile(ss.str());
		powerUpTextures.push_back(texture);
		ss.str("");
		ss << Texture_Folder << '/' << Texture_PowerUp_Subfolder << "/Wind powerup.png";
		texture.loadFromFile(ss.str());
		powerUpTextures.push_back(texture);
		ss.str("");
		ss << Texture_Folder << '/' << Texture_PowerUp_Subfolder << "/Thunder powerup.png";
		texture.loadFromFile(ss.str());
		powerUpTextures.push_back(texture);
		font.loadFromFile("Tinos-Regular.ttf");
		CircleShape noPowerUp(Power_Up_Display_Radius);
		noPowerUp.setPosition(Power_Up_Display_X, Power_Up_Display_Y);
		acquiredPowerUps.push_back(noPowerUp);
		powerUps.push_back(Element::Normal);
		Text t;
		t.setString("Normal"); 
		t.setFont(font); 
		t.setCharacterSize(Stat_Character_Size); 
		t.setFillColor(Color::Blue); 
		t.setPosition(Power_Up_Display_X + 70, noPowerUp.getPosition().y + 20);
		powerUpDescriptions.push_back(t);
		srand(time(NULL));
	}
	Paddle getPaddle() { return paddle; }
	void setPaddleColor(Color c) { paddle.setFillColor(c); }
	void setPaddlePosition(float x, float y) { paddle.setPosition(x, y); }
	void resetScore() { score = 0; }
	void scorePoint(int s) { score += s; }
	int getScore() { return score; }
	void lostLife() { --life; }
	int getLives() { return life; }
	void resetLives() { life = Player_Initial_Lives; }
	void moveLeft();
	void moveRight();
	void interact(Ball &ball);
	int getSelectedPowerUp() { return powerUpSelected;}
	Text getSelectedPowerUpDescription() { return powerUpDescriptions[powerUpSelected]; }
	void previousPowerUp() { if(powerUpSelected>0) --powerUpSelected; }
	void nextPowerUp() { if(powerUpSelected<acquiredPowerUps.size()-1) ++powerUpSelected; }
	void resetPowerUp() { powerUpSelected = 0; }
	void addPowerUp(Element p);
	void setPowerUp(bool p) { poweredUp = p; }
	void setPowerUpUsed(bool p) { powerUpUsed = p; }
	bool isPoweredUp() { return poweredUp; }
	bool isPowerUpUsed() { return powerUpUsed; }
	vector<CircleShape> getAcquiaredPowerUps() { return acquiredPowerUps; }
	void usePowerUp();
};

void Player::moveLeft() {
	paddle.moveLeft();
	FloatRect paddleBound = paddle.getGlobalBounds();
	if (paddleBound.left <= leftBound)
		paddle.setPosition(leftBound, paddleBound.top);
		
}

void Player::moveRight() {
	paddle.moveRight();
	FloatRect paddleBound = paddle.getGlobalBounds();
	if (paddleBound.left+paddleBound.width >= rightBound)
		paddle.setPosition(rightBound-paddleBound.width, paddleBound.top);		
}


void Player::interact(Ball &ball) {
	if(paddle.interact(ball))
		ball.setPowerUpType(powerUps[powerUpSelected]);
	if (powerUpUsed) {//Throw only one paddle at a time.

	}
}


void Player::usePowerUp() {
	if (poweredUp&&!powerUpUsed) {//If power up obtained and not yet used.
		
		powerUpUsed = true;
	}
}

void Player::addPowerUp(Element p) {
	CircleShape c;
	Text t;
	switch (p) {
	case Element::Water:c = CircleShape(Power_Up_Display_Radius); c.setPosition(Power_Up_Display_X, Power_Up_Display_Y - Power_Up_Display_Interval*acquiredPowerUps.size()); c.setTexture(&powerUpTextures[0]); acquiredPowerUps.push_back(c); powerUps.push_back(Element::Water); t.setString("Water"); t.setFont(font); t.setCharacterSize(Stat_Character_Size); t.setFillColor(Color::Blue); t.setPosition(Power_Up_Display_X + 70, c.getPosition().y + 20); powerUpDescriptions.push_back(t); break;
	case Element::Fire:c = CircleShape(Power_Up_Display_Radius); c.setPosition(Power_Up_Display_X, Power_Up_Display_Y - Power_Up_Display_Interval*acquiredPowerUps.size()); c.setTexture(&powerUpTextures[1]); acquiredPowerUps.push_back(c); powerUps.push_back(Element::Fire); t.setString("Fire"); t.setFont(font); t.setCharacterSize(Stat_Character_Size); t.setFillColor(Color::Blue); t.setPosition(Power_Up_Display_X + 70, c.getPosition().y + 20); powerUpDescriptions.push_back(t); break;
	case Element::Earth:c = CircleShape(Power_Up_Display_Radius); c.setPosition(Power_Up_Display_X, Power_Up_Display_Y - Power_Up_Display_Interval*acquiredPowerUps.size()); c.setTexture(&powerUpTextures[2]); acquiredPowerUps.push_back(c); powerUps.push_back(Element::Earth); t.setString("Earth"); t.setFont(font); t.setCharacterSize(Stat_Character_Size); t.setFillColor(Color::Blue); t.setPosition(Power_Up_Display_X + 70, c.getPosition().y + 20); powerUpDescriptions.push_back(t); break;
	case Element::Wind:c = CircleShape(Power_Up_Display_Radius); c.setPosition(Power_Up_Display_X, Power_Up_Display_Y - Power_Up_Display_Interval*acquiredPowerUps.size()); c.setTexture(&powerUpTextures[3]); acquiredPowerUps.push_back(c); powerUps.push_back(Element::Wind); t.setString("Wind"); t.setFont(font); t.setCharacterSize(Stat_Character_Size); t.setFillColor(Color::Blue); t.setPosition(Power_Up_Display_X + 70, c.getPosition().y + 20); powerUpDescriptions.push_back(t); break;
	case Element::Thunder:c = CircleShape(Power_Up_Display_Radius); c.setPosition(Power_Up_Display_X, Power_Up_Display_Y - Power_Up_Display_Interval*acquiredPowerUps.size()); c.setTexture(&powerUpTextures[4]); acquiredPowerUps.push_back(c); powerUps.push_back(Element::Thunder); t.setString("Thunder"); t.setFont(font); t.setCharacterSize(Stat_Character_Size); t.setFillColor(Color::Blue); t.setPosition(Power_Up_Display_X + 70, c.getPosition().y + 20); powerUpDescriptions.push_back(t); break;
	}
}