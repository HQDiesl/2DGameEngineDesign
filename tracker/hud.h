#include <SDL_image.h>

/*
This contains info to be displayed to the HUD once populated from the database.
*/

class Hud {
public:
	Hud(SDL_Renderer* const);
	void draw() const;
	void updateHp(const int, const int, const int, const int);
private:
	int x, y, p1hp, p2hp, life1, life2, thick;
	bool debug;
	SDL_Renderer * const r;
	SDL_Color text, p1, p2;
};
