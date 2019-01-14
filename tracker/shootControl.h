#include <string>
#include <iostream>
#include <list>
#include "projectile.h"

/*
The projectiles are stored in two lists, and are only created when
a new one is requested but not free. 
*/

class Player;

class shootControl{
public:
	shootControl(const std::string& n);
	shootControl(const shootControl& s);
	~shootControl();

	void update(Uint32 ticks);
	void shoot(std::string& type, int x, int y, const Vector2f& dir);
	void draw() const;
	int getAShots(){return activeShots.size();};
	int getFShots(){return freeShots.size();};
	std::list<Projectile*> getShotList(){return activeShots;};

private:
	std::string name;
	std::list<Projectile*> freeShots;
	std::list<Projectile*> activeShots;
	float interval;
	float timelapse;
	shootControl& operator=(const shootControl&);

};
