#include <vector>
#include <SDL.h>
#include "renderContext.h"
#include "world.h"
#include "viewport.h"
#include "player.h"
#include "hud.h"
#include "lights.h"
#include "menuEngine.h"

/*
Class with variables for all the other high level classes to be used in the engine.
*/

class CollisionStrategy;

class Engine {
public:
  Engine ();
  ~Engine ();
  bool play();

private:
  const RenderContext* rc;
  const IOmod& io;
  Clock& clock;
  Lights lights;

  SDL_Renderer * const renderer;
  Hud hud;
  MenuEngine menuEngine;
  World background;
  Viewport& viewport;


  std::vector<Drawable*> sprites;
  std::vector<Player*> players;
  std::vector<CollisionStrategy*> strategies;

  int currentStrategy;

  bool collision;
  bool makeVideo;
  bool hudDisplaying;
  bool cont;

  void draw() const;
  void update(Uint32);

  void checkForCollisions();

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  void printScales() const;
};
