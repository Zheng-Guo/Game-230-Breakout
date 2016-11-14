#pragma once
#include <SFML\Graphics.hpp>
#include <string>
#include <sstream>
#include <memory>
#include "GameConstants.h"
#include "Ball.h"
#include "Player.h"
#include "Brick.h"

using namespace sf;
using namespace std;

class PowerUp:public RectangleShape{
private:
	Texture texture;
	Element powerUpType;
	shared_ptr<Brick> brickAbove;
	bool isConsumed;
public:
	PowerUp(Element p):powerUpType(p),
	isConsumed(false){
		ostringstream ss;
		ss.str("");
		ss << Texture_Folder << '/' << Texture_PowerUp_Subfolder;
		switch (p) {
		case Element::Water:ss << "/Water powerup.png"; break;
		case Element::Fire:ss << "/Fire powerup.png"; break;
		case Element::Earth:ss << "/Earth powerup.png"; break;
		case Element::Wind:ss << "/Wind powerup.png"; break;
		case Element::Thunder:ss << "/Thunder powerup.png"; break;
		}
		texture.loadFromFile(ss.str());
		setTexture(&texture);
		setSize(Vector2f(Power_Up_Item_Width,Power_Up_Item_Height));	
	}
	void setBrickAbove(shared_ptr<Brick> b) { brickAbove = b; setPosition(b->getPosition().x+10,b->getPosition().y+10); }
	bool isExposed() { return brickAbove->isBroken()&&!isConsumed; }
	void interact(Ball &ball,Player &p);
};

void PowerUp::interact(Ball &ball,Player &player) {//Return the player that obtains the power up by hitting the ball at it.
	FloatRect ballBound = ball.getGlobalBounds();
	FloatRect powerUpBound = getGlobalBounds();
	if (!isConsumed&&ballBound.intersects(powerUpBound)) {
		isConsumed = true;
		player.addPowerUp(powerUpType);
	}
}