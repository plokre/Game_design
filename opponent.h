#ifndef OPPONENT__H
#define OPPONENT__H
#include <string>
#include "drawable.h"
#include "explodingPlayer.h"
#include "bullet.h"

class Opponent : public Drawable {
public:
  Opponent(const std::string&);
  Opponent(const Opponent& s);
  virtual ~Opponent() { delete explosion; } 

  virtual const Frame* getFrame() const { return frame; }
  virtual void draw() const;
  void changeExplode(bool isCase){ hasExploded = isCase; }
  virtual void update(Uint32 ticks);
  void shoot();
  bool checkCollision(const Drawable* );
  void reset();
  void explode();
  bool getExplode() { return hasExploded; }
  int oppWidth() const { return frameWidth; }
  int oppHeight() const { return frameHeight; }
  void avoidBullet( bool temp) { avoid = temp; }
  void stop() { Up = false; Down = false; }
  void goUp() { Up = true; Down = false; }
  void goDown() { Up = false; Down = true; }

private:
  ExplodingPlayer* explosion;  
  const Frame * frame;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;

  std::string bulletName;
  float bulletInterval;
  float timeSinceLastBullet;
  float minBulletSpeed;
  Bullet bullets;

  bool hasExploded;
  bool avoid;
  bool Up;
  bool Down;

  int getDistance(const Opponent*) const;
  Opponent& operator=(const Opponent&);
};
#endif
