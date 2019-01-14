#include <sstream>
#include <string>
#include "hud.h"
#include "ioMod.h"
#include "gamedata.h"
#include "renderContext.h"

/*
This renders the hud by using information from the XML database and SDL Render functions.
It also updates the hp bars of the players.
*/

Hud::Hud(SDL_Renderer* const rend) :
x (Gamedata::getInstance().getXmlInt("hud/xpos")),
y (Gamedata::getInstance().getXmlInt("hud/ypos")),
p1hp(100),
p2hp(100),
life1(Gamedata::getInstance().getXmlInt("general/lives")),
life2(Gamedata::getInstance().getXmlInt("general/lives")),
thick(Gamedata::getInstance().getXmlInt("hud/barthick")),
debug (Gamedata::getInstance().getXmlBool("hud/debug")),
r(rend),
text({0, 0, 0, 0}),
p1({0, 0, 0, 0}),
p2({0, 0, 0, 0})
{
  text.r =  Gamedata::getInstance().getXmlInt("hud/textcolor/red");
  text.g =  Gamedata::getInstance().getXmlInt("hud/textcolor/green");
  text.b =  Gamedata::getInstance().getXmlInt("hud/textcolor/blue");
  text.a =  Gamedata::getInstance().getXmlInt("hud/textcolor/alpha");

  p1.r =  Gamedata::getInstance().getXmlInt("hud/p1color/red");
  p1.g =  Gamedata::getInstance().getXmlInt("hud/p1color/green");
  p1.b =  Gamedata::getInstance().getXmlInt("hud/p1color/blue");
  p1.a =  Gamedata::getInstance().getXmlInt("hud/p1color/alpha");

  p2.r = Gamedata::getInstance().getXmlInt("hud/p2color/red");
  p2.g = Gamedata::getInstance().getXmlInt("hud/p2color/green");
  p2.b = Gamedata::getInstance().getXmlInt("hud/p2color/blue");
  p2.a = Gamedata::getInstance().getXmlInt("hud/p2color/alpha");
 }

void Hud::draw() const{
  if(debug){
  IOmod::getInstance().writeText("W/I = jump", text,  x, y);
  IOmod::getInstance().writeText("A/J = left", text,  x, y+30);
  IOmod::getInstance().writeText("D/L = right", text,  x, y+60);
  IOmod::getInstance().writeText("S/K = down(mid-air only)", text,  x, y+90);
  IOmod::getInstance().writeText("LSHIFT/SPACE = shoot", text,  x, y+120);
  IOmod::getInstance().writeText("Q/U = aim up", text,  x, y+150);
  IOmod::getInstance().writeText("E/O = aim down", text,  x, y+180);
  }
	IOmod::getInstance().writeText("Harrison Diesl", 300, 570);
	if(r == NULL)IOmod::getInstance().writeText("Render broke", 300, 270);
	SDL_Rect leftP = {20, 530, p1hp, thick};
	SDL_Rect leftBar = {20, 530, 100, thick};
	SDL_Rect rightP = {600, 530, p2hp, thick};
	SDL_Rect rightBar = {600, 530, 100, thick};
	SDL_SetRenderDrawColor(r, p1.r, p1.g, p1.b, p1.a);
  	SDL_RenderFillRect(r, &leftP);
	SDL_SetRenderDrawColor(r, p2.r, p2.g, p2.b, p2.a);
	SDL_RenderFillRect(r, &rightP);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 0);
	SDL_RenderDrawRect(r, &leftBar);
	SDL_RenderDrawRect(r, &rightBar);
	SDL_SetRenderDrawColor(r, 0, 225, 0, 0);
	for(int i = 0; i < life1; i++){
		SDL_Rect lCnt = {20+(i*30), (540+thick), 20, thick};
		SDL_RenderFillRect(r, &lCnt);
	}
	for(int i = 0; i < life2; i++){
		SDL_Rect lCnt = {600+(i*30), (540+thick), 20, thick};
		SDL_RenderFillRect(r, &lCnt);
	}

}

void Hud::updateHp(const int one, const int two, const int l1, const int l2){
  p1hp = one;
  p2hp = two;
  life1 = l1;
  life2 = l2;
}




///////////////////////
