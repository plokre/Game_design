#ifndef METEOR__H
#define METEOR__H
#include <string>
#include "drawable.h"
#include "explodingPlayer.h"

class Meteor : public Drawable {
public:
  Meteor(const std::string&);
  Meteor(const Meteor& s);
  virtual ~Meteor() { delete explosion; } 

  virtual const Frame* getFrame() const { return frame; }
  virtual void draw() const;
  double getScale() const { return zoom; }
  void changeExplode(bool isCase){ hasExploded = isCase; }
  virtual void update(Uint32 ticks);
  void reset();
  void explode();
  bool getExplode() { return hasExploded; }

private:
  ExplodingPlayer* explosion;  
  double zoom;
  const Frame * frame;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
  bool hasExploded;

  int getDistance(const Meteor*) const;
  Meteor& operator=(const Meteor&);
};
#endif
