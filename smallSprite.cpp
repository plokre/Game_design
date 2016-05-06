#include <cmath>
#include "smallSprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "opponent.h"
#include "SDL/SDL_rotozoom.h"

smallSprite::smallSprite(const string& n, const Vector2f& pos, const Vector2f& vel):
  Drawable(n, pos, vel),
  explosion(NULL), 
  frame( FrameFactory::getInstance().getFrame(n) ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  Xaxis( pos[0] ),
  maxDistance(Gamedata::getInstance().getXmlInt(n+"/maxDistance")),
  tooFar( false ),
  hasExploded( false )
{ }

smallSprite::smallSprite(const smallSprite& s) :
  Drawable(s),
  explosion(s.explosion),
  frame(s.frame),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  Xaxis( s.Xaxis ),
  maxDistance( s.maxDistance ),
  tooFar( s.tooFar ),
  hasExploded( s.hasExploded )
{ }

void smallSprite::draw() const { 
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
  }
}

void smallSprite::explode() { 
  if ( explosion ) return;
  Sprite sprite(getName(), getPosition(), getVelocity(), getFrame());
  explosion = new ExplodingPlayer(sprite); 
}

void smallSprite::reset(){
  tooFar = true;
}

void smallSprite::update(Uint32 ticks) { 
  if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  velocityY(0);
  setPosition(getPosition() + incr);

  if ( Y() < 0 || Y() > worldHeight-frameHeight) {
    tooFar = true;
  }

  if ( X() < 0 || X()-Xaxis > maxDistance || Xaxis-X() > maxDistance || X() > worldWidth-frameWidth) {
    tooFar = true;
  }
}
