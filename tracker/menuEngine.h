#include <vector>
#include <SDL.h>
#include "menu.h"
#include "player.h"

/*
Runs the menu and the clock.
*/

class MenuEngine {
public:
  MenuEngine (SDL_Renderer* const);
  ~MenuEngine ();
  void play(std::vector<Player*>& ps);

private:
  Clock& clock;
  SDL_Renderer * const renderer;
  Menu menu;

  void draw() const;
  void update(Uint32);

  MenuEngine(const MenuEngine&);
  MenuEngine& operator=(const MenuEngine&);
};
