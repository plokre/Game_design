#ifndef BULLET_H
#define BULLET_H
#include <list>
#include "smallSprite.h"
#include "drawable.h"
#include "explodingPlayer.h"

class Bullet {
public:
  Bullet(const std::string &);
  Bullet(const Bullet&);
  ~Bullet() {};
  void draw() const;
  void update(Uint32 ticks);
  void shoot(const Vector2f& pos, const Vector2f& objVel);
  
  unsigned int bulletCount() const {return bulletList.size();}
  bool shoot() const { return !bulletList.empty();}
  bool collidedWith(const Drawable* obj, int absDist) ;
  int getBulletlistSize();
  void reset();
  int smartCheck(const Drawable* obj);

private:
  std::string name;
  Vector2f myVelocity;
  std::vector<Frame*> bulletFrames;
  std::list<smallSprite> bulletList;
};
#endif
