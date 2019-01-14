#include <iostream>
#include <cmath>
#include "projectile.h"
#include "player.h"

/*
This class handles projectiles fired by player objects.
It uses a multisprite to hold the different images and assign direction and velocity.
*/

Projectile::Projectile(const std::string& name, int xPos, int yPos, const Vector2f& vec):
    MultiSprite(name),
    speed(Gamedata::getInstance().getXmlInt(name+"/speed")),
    collision(false),
    stuck(false),
    dirMod(vec)
  {
	setX(xPos);
	setY(yPos);
  }

void Projectile::update(Uint32 ticks){
  	Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
 	 setPosition(getPosition() + incr);

  	if ( getY() < 0) {
	 stuck = true;
	 collision = true;
  	}
  	else if ( getY() > worldHeight-getScaledHeight()-80) {
	 stuck = true;
	 collision = true;
  	}
  	else if ( getX() < 0) {
	 stuck = true;
	 collision = true;
  	}
 	else if ( getX() > worldWidth-getScaledWidth()) {
	 stuck = true;
	 collision = true;
 	}
	else
	stuck = false;
  	setFrame();
}

void Projectile::changeDir(int x, int y, const Vector2f& vec){
	setX(x);
	setY(y);
	stuck = false;
	dirMod[0] = vec[0];
	dirMod[1] = vec[1];
	setVelocityX((100*speed)*dirMod[0]);
	setVelocityY((100*speed)*dirMod[1]);
}

void Projectile::draw() const {
  images[currentFrame]->draw(getX(), getY(), getScale());
}

void Projectile::setFrame(){
	if (getVelocityX() == 0 && getVelocityY() < 0){
		currentFrame = 0; //up
	}
	else if (getVelocityX() > 0 && getVelocityY() == 0){
		currentFrame = 1; //right
	}
	else if (getVelocityX() < 0 && getVelocityY() == 0){
		currentFrame = 2; //left
	}
	else if (getVelocityX() == 0 && getVelocityY() > 0){
		currentFrame = 3; //down
	}
	else if (getVelocityX() > 0 && getVelocityY() < 0){
		currentFrame = 4; //right-up
	}
	else if (getVelocityX() > 0 && getVelocityY() > 0){
		currentFrame = 5; //right-down
	}
	else if (getVelocityX() < 0 && getVelocityY() < 0){
		currentFrame = 6; //left-up
	}
	else
		currentFrame = 7; //left-down

}
