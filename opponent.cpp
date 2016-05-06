#include <cmath>
#include "opponent.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "explodingPlayer.h"
#include "SDL/SDL_rotozoom.h"

Opponent::Opponent(const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))),
  explosion(NULL),
  frame( FrameFactory::getInstance().getFrame(name) ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  bulletName("ebullet"),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/bulletInterval")),
  timeSinceLastBullet(0),
  minBulletSpeed(Gamedata::getInstance().getXmlInt(bulletName+"/minBulletSpeed")),
  bullets(bulletName),
  hasExploded( false ),
  avoid( false ),
  Up( false ),
  Down( false )
{ 
}

Opponent::Opponent(const Opponent& s) :
  Drawable(s),
  explosion( s.explosion ),
  frame(s.frame),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  bulletName(s.bulletName),
  bulletInterval(s.bulletInterval),
  timeSinceLastBullet(s.timeSinceLastBullet),
  minBulletSpeed(s.minBulletSpeed),
  bullets(s.bullets),
  hasExploded( s.hasExploded ),
  avoid( s.avoid ),
  Up( s.Up ),
  Down( s.Down )
{ }

void Opponent::draw() const { 
  if (explosion) 
  {
    explosion->draw();
    return;
  }
  if( !hasExploded )
  {
    Uint32 x = static_cast<Uint32>(X());
    Uint32 y = static_cast<Uint32>(Y());
    frame->draw(x, y); 
    bullets.draw();
  }
}

void Opponent::explode() { 
  if ( explosion ) return;
  Sprite sprite(getName(), getPosition(), getVelocity(), getFrame());
  explosion = new ExplodingPlayer(sprite); 
}

void Opponent::reset(){
  X(Gamedata::getInstance().getXmlInt(getName()+"/startLoc/x"));
  Y(Gamedata::getInstance().getXmlInt(getName()+"/startLoc/y")); 
  if( hasExploded )
  {
    hasExploded = false;
  }
}

int Opponent::getDistance(const Opponent *obj) const { 
  return hypot(X()-obj->X(), Y()-obj->Y());
}

void Opponent::update(Uint32 ticks) {
  if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }
 
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if(avoid)
  {
    if(Down)
      velocityY(200);
    if(Up)
      velocityY(-200);
  }
  else
    velocityY(0);

  if(Y() > worldHeight-frameHeight)
    Y(0);
  if(Y() < 0)
    Y(worldWidth-frameWidth);

  timeSinceLastBullet +=ticks;
  shoot();
  bullets.update(ticks);
}

void Opponent::shoot()
{
  if(timeSinceLastBullet > 4*bulletInterval) 
  {
    Vector2f vel = getVelocity();
    float x;
    float y = Y()+ frameHeight/2;
    x=X();
    vel[0] -= minBulletSpeed;
    bullets.shoot(Vector2f(x,y),vel);
    timeSinceLastBullet = 0;
  }
}

bool Opponent::checkCollision(const Drawable* obj)
{
  if(bullets.collidedWith( obj, 10) )
  {
    return true;
  }
  return false;
}
