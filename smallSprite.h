#ifndef SMALLSPRITE_H
#define SMALLSPRITE_H
#include <iostream>
#include <string>
#include "drawable.h"
#include "explodingPlayer.h"

class smallSprite : public Drawable {
public:
  smallSprite(const std::string&, const Vector2f& pos, const Vector2f& vel);
  smallSprite(const smallSprite& s);
  virtual ~smallSprite() { delete explosion; } 

  virtual const Frame* getFrame() const { return frame; }
  virtual void draw() const;
  virtual void update(Uint32 ticks);

  bool goneTooFar() const { return tooFar; }
  void changeExplode(bool isCase){ hasExploded = isCase; }

  void reset();
  void explode();
  bool getExplode() { return hasExploded; }
  float getX() const { return Xaxis; }
  int oppWidth() const { return frameWidth; }
  int oppHeight() const { return frameHeight; }

private:
  ExplodingPlayer* explosion;
  const Frame * frame;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;

  float Xaxis;
  unsigned int maxDistance;
  bool tooFar;
  bool hasExploded;

  smallSprite& operator=(const smallSprite&);
};
#endif
