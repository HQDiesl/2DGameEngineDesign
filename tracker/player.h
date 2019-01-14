#ifndef PLAYER__H
#define PLAYER__H
#include <SDL.h>
#include <string>
#include <vector>
#include <list>
#include <cmath>
#include "multisprite.h"
#include "shootControl.h"
#include "collisionStrategy.h"
#include "sound.h"

/*
Contains all the information for the player class, including the AI features
such as the observer list and AI states. 
*/

class Player  {
public:
  Player(const std::string&, const std::string& t, bool);
  Player(const Player&);
  ~Player();

  void draw() const;
  bool update(Uint32 ticks);
  const MultiSprite* getPlayer() const { return &player; }
  void eliminate();

  const std::string& getName() const { return player.getName(); }
  int getX() const { return player.getX(); }
  int getY() const { return player.getY(); }
  int getL() const { return lives;}
  const Image* getImage() const {
    return player.getImage();
  }
  int getScaledWidth()  const {
    return player.getScaledWidth();
  }
  int getScaledHeight()  const {
    return player.getScaledHeight();
  }
  const SDL_Surface* getSurface() const {
    return player.getSurface();
  }

  void right();
  void left();
  void up();
  void down();
  void aimDown();
  void aimUp();
  void stop();
  void shoot();
  int getHp();
  void damage(int d){hp -= d;};
  int getActiveShots(){return weapon.getAShots();};
  int getFreeShots(){return weapon.getFShots();};
  void assessCollision();
  void noCollision();
  void toggleAI();
  void toggleGod();
  void attach(Player* p) {observers.emplace_back(p);};
  void detach(Player* p);
  Vector2f getDir();
  std::list<Projectile*> getWeapon(){return weapon.getShotList();};

  Player& operator=(const Player&) = delete;

private:
  MultiSprite player, gun;
  SDLSound s;
  std::string type;
  std::list<Player*> observers;
  std::vector<CollisionStrategy*> strategies;
  bool ai;
  enum MODE {ATTACK, EVADE};
  MODE currentMode;
  enum AIM {UP, DOWN, LEFT, RIGHT, UP_RIGHT, UP_LEFT, DOWN_RIGHT, DOWN_LEFT};
  AIM  direction;
  int aiSpeed, ex, ey, safeDistance;
  float distanceToEnemy, lastY;
  int hp, basetimer, lives;
  bool collision;
  bool airborne;
  bool invul, god;
  Vector2f initialVelocity;
  int worldWidth, worldHeight;
  shootControl weapon;
};
#endif
