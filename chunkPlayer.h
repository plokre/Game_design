#ifndef CHUNKPLAYER_H
#define CHUNKPLAYER_H
#include <iostream>
#include "sprite.h"
#include "gamedata.h"

class ChunkPlayer : public Sprite {
public:
  explicit ChunkPlayer(
    const Vector2f& pos, const Vector2f vel, 
    const string& name, const Frame* fm) :
    Sprite(name, pos, vel, fm), 
    distance(0), 
    maxDistance(Gamedata::getInstance().getXmlInt(name+"/distance")), 
    tooFar(false) 
  { }
  virtual void update(Uint32 ticks);
  bool goneTooFar() const { return tooFar; }
  void reset() {
    tooFar = false;
    distance = 0;
  }
private:
  float distance;
  float maxDistance;
  bool tooFar;
};
#endif
