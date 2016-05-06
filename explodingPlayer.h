#ifndef EXPLODINGPLAYER_H
#define EXPLODINGPLAYER_H
#include <vector>
#include <list>
#include "chunkPlayer.h"

class ExplodingPlayer : public Sprite {
public:
  ExplodingPlayer(const Sprite& s);
  ~ExplodingPlayer();
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  void makeChunks(unsigned int);
  unsigned int chunkCount() const { return chunks.size(); }
  unsigned int freeCount()  const { return freeList.size(); }
  bool done() {return isSpriteDone;}
private:
  std::list<ChunkPlayer> chunks; // An ExplodingPlayer is a list of sprite chunks
  std::list<ChunkPlayer> freeList; // When a chunk gets out of range it goes here
  std::vector<Frame*> frames; // Each chunk has a Frame
  bool isSpriteDone;
  ExplodingPlayer(const ExplodingPlayer&);  
  ExplodingPlayer& operator=(const ExplodingPlayer&); 
};
#endif
