#include <vector>
#include "shootControl.h"
#include "gamedata.h"

/*
This class manages the projectiles within the game, maintaining a list of active
and inactive projectiles for efficiency. 
*/

shootControl::shootControl(const std::string& n) :
	name(n),
	freeShots(),
	activeShots(),
	interval(Gamedata::getInstance().getXmlInt(name+"/interval")),
	timelapse(0)
	{}

shootControl::shootControl(const shootControl& s) :
	name(s.name),
	freeShots(s.freeShots),
	activeShots(s.activeShots),
	interval(s.interval),
	timelapse(s.timelapse)
	{}

shootControl::~shootControl(){
	std::list<Projectile*>::iterator it = freeShots.begin();
  while(it != freeShots.end()){
	it = freeShots.erase(it);
	++it;
  }
	std::list<Projectile*>::iterator it2 = activeShots.begin();
  while(it2 != activeShots.end()){
	it2 = activeShots.erase(it2);
	++it2;
  }
}

void shootControl::shoot(std::string& type, int x, int y, const Vector2f& dir){
	if (timelapse < interval) return;
	if(freeShots.empty()){
		Projectile *temp = new Projectile(type, x, y, dir);
		freeShots.push_front(temp);
	}
	activeShots.push_front(freeShots.front());
	activeShots.front()->changeDir(x, y, dir);
	freeShots.pop_front();
	timelapse = 0;
}

void shootControl::draw() const{
	std::list<Projectile*>::const_iterator it = activeShots.begin();
	while(it != activeShots.end()){
		(*it)->draw();
		it++;
	}
	std::list<Projectile*>::const_iterator it2 = freeShots.begin();
	while(it2 != freeShots.end()){
		if((*it2)->isStuck() == true) {(*it2)->draw();}
		it2++;
	}
}


void shootControl::update(Uint32 ticks){
	std::list<Projectile*>::const_iterator it = activeShots.begin();
	while(it != activeShots.end()){
		(*it)->update(ticks);
		if((*it) -> hit()){
			(*it) -> reset();
			freeShots.emplace_back(*it);
			it = activeShots.erase(it);
		}
		else
		it++;
	}
	timelapse++;
}
