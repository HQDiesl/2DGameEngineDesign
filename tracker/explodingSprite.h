// This is the "pointers" example in the Brian example

#include <vector>
#include <list>
#include "chunk.h"
#include "sprite.h"

/*
Handles the list of sprite chucnks. 
*/

class ExplodingSprite : public Sprite {
public:
  ExplodingSprite(const Sprite& s);
  ~ExplodingSprite();
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  void makeChunks(unsigned int);
  unsigned int chunkCount() const { return chunks.size(); }
  unsigned int freeCount()  const { return freeList.size(); }
private:
  std::list<Chunk*> chunks; // An ExplodingSprite is a list of sprite chunks
  std::list<Chunk*> freeList; // When a chunk gets out of range it goes here

  ExplodingSprite(const ExplodingSprite&); // Explicit disallow (Item 6)
  ExplodingSprite& operator=(const ExplodingSprite&); // (Item 6)
};
