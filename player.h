#ifndef PLAYER__H
#define PLAYER__H
#include <string>
#include <vector>
#include "drawable.h"
#include "sprite.h"
#include "bullet.h"

class Player : public Drawable {
public:
  Player(const std::string&);
  Player(const Player&);
  virtual ~Player() { delete explosion; } 

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }
  void changeExplode(bool isCase){ hasExploded = isCase; }
  void reset();
  virtual bool collidedWith(const Drawable* ,const Drawable* );
  void shoot();
  bool checkCollision(const Drawable* );
  void explode();
  bool getExplode() { return hasExploded; }
  int smartCheck(const Drawable* );

protected:
  ExplodingPlayer* explosion;  
  const std::vector<Frame *> frames;
  int worldWidth;
  int worldHeight;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;

  int movSpeedX;
  int movSpeedY;
  int direction;

  std::string bulletName;
  float bulletInterval;
  float timeSinceLastBullet;
  float minBulletSpeed;
  Bullet bullets;

  bool hasExploded;

  void advanceFrame(Uint32 ticks);
  Player& operator=(const Player&);
};
#endif
