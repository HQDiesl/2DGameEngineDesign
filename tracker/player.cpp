#include "player.h"
#include "gamedata.h"

/*
This class initializes and draws the player.
It also contains the AI for the player using the observer pattern. 
*/

Player::Player( const std::string& n, const std::string& t, bool AI) :
  player(n),
  gun("bow"),
  s(),
  type(t),
  observers(),
  strategies(),
  ai(AI),
  currentMode(EVADE),
  direction(RIGHT),
  aiSpeed(0),
  ex(0),
  ey(0),
  safeDistance(Gamedata::getInstance().getXmlInt("general/dist")),
  distanceToEnemy(0),
  lastY(getY()),
  hp(100),
  basetimer(20),
  lives(Gamedata::getInstance().getXmlInt("general/lives")),
  collision(true),
  airborne(false),
  invul(false),
  god(false),
  initialVelocity(player.getVelocity()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  weapon(t)
  {strategies.emplace_back( new RectangularCollisionStrategy );}

Player::~Player(){
std::list<Player*>::iterator it = observers.begin();
  while(it != observers.end()){
	it = observers.erase(it);
	++it;
  }
std::vector<CollisionStrategy*>::iterator it2 = strategies.begin();
  while(it2 != strategies.end()){
	delete *it2;
	++it2;
  }
}

void Player::draw() const{
	player.draw();
	int frm = 0;
	switch(direction){
		case UP:
			frm = 0;
			break;
		case RIGHT:
			frm = 1;
			break;
		case LEFT:
			frm = 2;
			break;
		case DOWN:
			frm = 3;
			break;
		case UP_RIGHT:
			frm = 4;
			break;
		case DOWN_RIGHT:
			frm = 5;
			break;
		case UP_LEFT:
			frm = 6;
			break;
		case DOWN_LEFT:
			frm = 7;
			break;
	}
	if(invul != true && hp != 1)gun.forceFrame(frm);
	weapon.draw();

}

void Player::eliminate(){
	player.explode();
	gun.explode();
}

void Player::stop() {
  player.setVelocityX( 0.85*player.getVelocityX() );
  player.setVelocityY(0);
}

void Player::right() {
   if ( player.getX() < worldWidth-getScaledWidth()) {
     if (abs(lastY-player.getY()) < 2 ) {
	player.setVelocityX(initialVelocity[0]*1.2);
     }
     else
	player.setVelocityX(initialVelocity[0]*0.9);
   }
   direction = RIGHT;
}
void Player::left()  {
   if ( player.getX() > 0) {
     if (abs(lastY-player.getY()) < 2 ) {
	player.setVelocityX(-initialVelocity[0]*1.2);
     }
     else
	player.setVelocityX(-initialVelocity[0]*0.9);
   }
   direction = LEFT;
}
void Player::up()    {
  if ( !airborne ) {
    lastY = getY();
    player.setVelocityY( -initialVelocity[1]*2);
    airborne = true;
    collision = false;
  }
  direction = UP;
}
void Player::down()  {
  if (abs(lastY-player.getY()) > 2 ) {
	direction = DOWN;
     }
}

void Player::aimDown()  {
  if (direction == RIGHT || direction == UP_RIGHT) {
	direction = DOWN_RIGHT;
     }
  else if (direction == LEFT || direction == UP_LEFT){
	direction = DOWN_LEFT;
     }
}

void Player::aimUp()  {
  if (direction == RIGHT || direction == DOWN_RIGHT) {
	direction = UP_RIGHT;
     }
  else if (direction == LEFT || direction == DOWN_LEFT){
	direction = UP_LEFT;
     }
}

void Player::shoot(){
	if(!invul){
		weapon.shoot(type, gun.getX()+gun.getScaledWidth()/4, gun.getY()+gun.getScaledHeight()/4, getDir());
		if(((int)player.getX())%2 == 0)s[0];
		else s[1];
	}

}

void Player::assessCollision(){
  if (airborne){
	if (player.getVelocityY() >= 0)
		collision = true;
  }
  else
		collision = true;
}

void Player::noCollision(){
	collision = false;
}

int Player::getHp() {
	return hp;
}

void Player::toggleAI(){
	if (ai)ai = false;
	else ai = true;
}

void Player::toggleGod(){
	if (god)god = false;
	else god = true;
}

void Player::detach(Player * p){
  std::list<Player*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == p ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}

Vector2f Player::getDir(){
	Vector2f dir;
	switch(direction){
		case UP:
			dir[0] = 0;
			dir[1] = -1;
			break;
		case DOWN:
			dir[0] = 0;
			dir[1] = 1;
			break;
		case LEFT:
			dir[0] = -1;
			dir[1] = 0;
			break;
		case RIGHT:
			dir[0] = 1;
			dir[1] = 0;
			break;
		case UP_RIGHT:
			dir[0] = 1;
			dir[1] = -1;
			break;
		case DOWN_RIGHT:
			dir[0] = 1;
			dir[1] = 1;
			break;
		case UP_LEFT:
			dir[0] = -1;
			dir[1] = -1;
			break;
		case DOWN_LEFT:
			dir[0] = -1;
			dir[1] = 1;
			break;
	}
	return dir;
}

float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

bool Player::update(Uint32 ticks) {
  player.update(ticks);
  Vector2f gunPos = player.getPosition();
  gunPos[0] = gunPos[0]+10;
  gunPos[1] = gunPos[1]+10;
  gun.setPosition(gunPos);
  weapon.update(ticks);
  std::list<Player*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    	std::list<Projectile*> temp = (*ptr)->getWeapon();
	std::list<Projectile*>::iterator it = temp.begin();
  		while ( it != temp.end() ) {
    			if(strategies[0]->execute(*getPlayer(), **it )){
				if(!invul && !god){
					damage(10);
					s[2];
					(*it)->forceCollide();
				}
			}
		++it;
 		}
	++ptr;
  }

  if(hp <= 0){
	s[3];
	player.explode();
	lives --;
	hp = 1;
	basetimer = 200;
	invul = true;
  }
  if( lives == 0 && basetimer == 0) return false;
  if(invul && basetimer == 0){invul = false; hp = 100;}
  if(hp < 100 && basetimer == 0){hp++; basetimer = 20;}
  if(basetimer > 0) {basetimer --;}
  //std::cout << direction << std::endl;
  if ( collision ) {airborne = false; lastY = getY(); stop(); player.setY(player.getY()-1);}
  else {
	airborne = true;
	if(player.getVelocityY() < initialVelocity[1] * 4) {player.setVelocityY( player.getVelocityY()+10 );}
  }
  if (aiSpeed > 0){ aiSpeed --;}
  else  {aiSpeed = Gamedata::getInstance().getXmlInt("general/aiDelay");}

  if (ai){
  	if (aiSpeed <= 0){
		float min = 9999;
 		for (auto P : observers) {
  			distanceToEnemy = ::distance( player.getX(), player.getY(), P->player.getX(), P->player.getY() );
   			 if (distanceToEnemy < min) {
      				min = distanceToEnemy;
				ex = P->player.getX();
				ey = P->player.getY();
    			}
  		}
	}
	if  ( currentMode == ATTACK ) {
   		 if(distanceToEnemy < safeDistance) currentMode = EVADE;
		else {
			if (abs(player.getX() - ex) < 80 && player.getY() > ey){
				up();
				if(!invul)shoot();
			}
			else if(abs(player.getX() - ex) < 80 && player.getY() < ey){
				up();
				down();
				if(!invul)shoot();
			}
			else if( player.getX() > ex && abs(player.getY() - ey) < 40){
				left();
				if(!invul)shoot();
			}
			else if (player.getX() > ex && player.getY() > ey){
				left();
				aimUp();
				if(!invul)shoot();
			}
			else if( player.getX() > ex && player.getY() < ey){
				left();
				aimDown();
				if(!invul)shoot();
			}
			else if( player.getX() < ex && abs(player.getY() - ey) < 40){
				right();
				if(!invul)shoot();
			}
			else if (player.getX() < ex && player.getY() > ey){
				right();
				aimUp();
				if(!invul)shoot();
			}
			else if( player.getX() < ex && player.getY() < ey){
				right();
				aimDown();
				if(!invul)shoot();
			}

    		}
  	}
  	else if  ( currentMode == EVADE ) {
   		 if(distanceToEnemy > safeDistance) currentMode=ATTACK;
    		else {
			if (abs(player.getX() - (worldWidth-getScaledWidth())) <= 30){
				left();
				up();
				down();
				shoot();
			}
			else if( player.getX() > ex && player.getVelocityY() > 0) right();
			if (player.getX() <= 30){
				right();
				up();
				down();
				shoot();
			}
			else if ( player.getX() < ex && player.getVelocityY() > 0) left();
			if ( player.getY() < ey ){
				up();
				down();
				shoot();
			}
    		}
  	}

  }
  return true;
}
