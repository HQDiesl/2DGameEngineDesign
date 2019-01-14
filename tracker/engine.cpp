#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
//#include "sound.h"
/*
This is the main class that handles the gameplay loop, as well as initializing and clearing data.
Accepts key inputs as well as calling functions for updating the frame and checking collision.
*/

Engine::~Engine() {
  std::cout << "Beginning Engine deletion" << std::endl;
  std::vector<Drawable*>::iterator it = sprites.begin();
  while(it != sprites.end()){
	delete *it;
	++it;
  }
  std::vector<Player*>::iterator it2 = players.begin();
  while(it2 != players.end()){
	delete *it2;
	++it2;
  }
  std::vector<CollisionStrategy*>::iterator it3 = strategies.begin();
  while(it3 != strategies.end()){
	delete *it3;
	++it3;
  }
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  clock( Clock::getInstance() ),
  lights(),
  renderer( rc->getRenderer() ),
  hud(renderer),
  menuEngine(renderer),
  background("background", Gamedata::getInstance().getXmlInt("background/factor") ),
  viewport( Viewport::getInstance() ),
  sprites(),
  players(),
  strategies(),
  currentStrategy(0),
  collision(false),
  makeVideo( false ),
  hudDisplaying( true ),
  cont(true)
{
  int platCount = Gamedata::getInstance().getXmlInt("general/platCnt");

  sprites.reserve(platCount+1);
  int i = 0;
  while (i < platCount){
    std::stringstream ss;
    ss << i;
    string id = ss.str();
    sprites.emplace_back(new Sprite("platform"));
    sprites[i]->setVelocityX(0);
    sprites[i]->setVelocityY(0);
    sprites[i]->setX(Gamedata::getInstance().getXmlInt("general/platPos/x"+id));
    sprites[i]->setY(Gamedata::getInstance().getXmlInt("general/platPos/y"+id));
    i++;
  }
  sprites.emplace_back(new Sprite("center"));
  sprites[i]->setVelocityX(0);
  sprites[i]->setVelocityY(0);
  sprites[i]->setX(400);
  sprites[i]->setY(400);

  strategies.reserve(3);
  strategies.emplace_back( new RectangularCollisionStrategy );
  strategies.emplace_back( new PerPixelCollisionStrategy );
  strategies.emplace_back( new MidPointCollisionStrategy );

  players.reserve(3);
  players.emplace_back(new Player("Player1", "blue", false));
  players.emplace_back(new Player("Player2", "red", false));
  for (auto& ptr : players){
  	for (auto& ptr2 : players){
    		if(ptr != ptr2)
		ptr -> attach(ptr2);
  	}
  }


  Viewport::getInstance().setObjectToTrack(sprites[i]);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
  unsigned i = 0;
  background.draw();
  while (i < sprites.size()-1){
    sprites[i] ->draw();
     ++i;
  }

  std::vector<Player*>::const_iterator ptr2 = players.begin();
  for (auto& ptr2 : players){
    ptr2 ->draw();
  }
  lights.draw();

  if(hudDisplaying){
  	hud.draw();
  }

  if(!cont){
	io.writeText("Game Over! Press R to Restart.", 250, 250);
	clock.pause();
  }

  SDL_RenderPresent(renderer);
}

void Engine::checkForCollisions() {
collision = false;
  for ( unsigned it = 0; it < players.size(); it++ ) {
     for(unsigned it2 = 0; it2 < sprites.size()-1; it2++) {
   	if ( strategies[currentStrategy]->execute(*players[it]->getPlayer(), *sprites[it2]) ) {
	players[it]->assessCollision();
	collision = true;
    	}
    }
    if (!collision){
    	players[it] ->noCollision();
    }
    collision = false;
  }
}


void Engine::update(Uint32 ticks) {
  checkForCollisions();
  std::vector<Drawable*>::const_iterator ptr = sprites.begin();
  for (auto& ptr : sprites){
    ptr ->update(ticks);
  }
  std::vector<Player*>::const_iterator ptr2 = players.begin();
  for (auto& ptr2 : players){
    if(!(ptr2 ->update(ticks))){
	cont = false;
    }
  }
  background.update();
  lights.update();
  viewport.update(); // always update viewport last
}

bool Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  SDLSound sound;
  sound.startMusic();


  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Y]) {
          done = true;
          break;
        }
	if ( keystate[SDL_SCANCODE_M] ) {
	  sound.toggleMusic();
          clock.pause();
	  menuEngine.play(players);
          clock.unpause();
	  sound.toggleMusic();
	}
        if ( keystate[SDL_SCANCODE_R] ) {
          clock.unpause();
	  return true;
        }
	if ( keystate[SDL_SCANCODE_F1] ) {
          if ( hudDisplaying ) hudDisplaying = false;
          else hudDisplaying = true;
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      /////////////////////////Player1 controls here
	if ( keystate[SDL_SCANCODE_W] ) {
          players[0]->up();
        }
	if ( keystate[SDL_SCANCODE_A] ) {
          players[0]->left();
        }
	if ( keystate[SDL_SCANCODE_D] ) {
          players[0]->right();
        }
	if ( keystate[SDL_SCANCODE_S] ) {
          players[0]->down();
        }
	if ( keystate[SDL_SCANCODE_Q] ) {
          players[0]->aimUp();
        }
	if ( keystate[SDL_SCANCODE_E] ) {
          players[0]->aimDown();
        }
	if ( keystate[SDL_SCANCODE_LSHIFT] ) {
          players[0]->shoot();
        }
      ///////////////////////////Player2 controls here
	if ( keystate[SDL_SCANCODE_I] ) {
          players[1]->up();
        }
	if ( keystate[SDL_SCANCODE_J] ) {
          players[1]->left();
        }
	if ( keystate[SDL_SCANCODE_L] ) {
          players[1]->right();
        }
	if ( keystate[SDL_SCANCODE_K] ) {
          players[1]->down();
        }
	if ( keystate[SDL_SCANCODE_U] ) {
          players[1]->aimUp();
        }
	if ( keystate[SDL_SCANCODE_O] ) {
          players[1]->aimDown();
        }
	if ( keystate[SDL_SCANCODE_SPACE] ) {
          players[1]->shoot();
        }
	//////////////////////////////
      hud.updateHp(players[0]->getHp(), players[1]->getHp(), players[0]->getL(), players[1]->getL());
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
  return false;
}
