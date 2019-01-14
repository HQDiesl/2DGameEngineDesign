#ifndef PROJECTILE__H
#define PROJECTILE__H
#include <iostream>
#include <vector>
#include <string>
#include "multisprite.h"
#include "gamedata.h"

/*
This class handles projectiles fired by player objects.
It uses a multisprite to hold the different images and assign direction and velocity.
*/

class Player;

class Projectile : public MultiSprite{
public:
	explicit Projectile(const std::string& name, int xPos, int yPos, const Vector2f& vec);

	virtual void update(Uint32 ticks);
	virtual void draw() const;


	void changeDir(int x, int y, const Vector2f& dir);
	void setFrame();
	void forceCollide(){collision = true;};
	void reset(){ collision = false;}
	bool hit(){return collision;}
	bool isStuck(){return stuck;}

private:
	int speed;
	bool collision, stuck;
	Vector2f dirMod;
};
#endif
