#include <cmath>
#include "meteor.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "explodingPlayer.h"
#include "SDL/SDL_rotozoom.h"

float getVariation(const std::string& name, const char *s, int min, int max)
{
float variation = Gamedata::getInstance().getRandInRange(min,max);
variation *= (rand()%2 == 0? -1: 1);
return Gamedata::getInstance().getXmlInt(name+s)+variation; 
}

Meteor::Meteor(const std::string& name) :
  Drawable(name,
           Vector2f(getVariation(name,"/startLoc/x", 
                                 Gamedata::getInstance().getXmlInt(name+"/minDist"), 
                                 Gamedata::getInstance().getXmlInt(name+"/maxDist")
                                ),
                   Gamedata::getInstance().getXmlInt(name+"/startLoc/y") 
                   ), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),                    
                    Gamedata::getInstance().getXmlInt(name+"/speedY")
                   ) 
           ),
  explosion(NULL),
  zoom(Gamedata::getInstance().getRandFloat( 0.2, 1.0 )),
  frame( FrameFactory::getInstance().getFrame(name) ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  hasExploded( false )
{ 
  Vector2f change = getVelocity()*zoom; //This step was done in the constructor as an assignment statement because the zoom factor which is a part of this class can
  setVelocity( change );                //only be initialized after the base class members are initialized thus preventing us from directly initializing the velocity 
                                        //according to the size of the meteor to produce the depth effect.
}

Meteor::Meteor(const Meteor& s) :
  Drawable(s),
  explosion( s.explosion ),
  zoom( s.zoom ), 
  frame(s.frame),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  hasExploded( s.hasExploded )
{ }

void Meteor::draw() const { 
  if (explosion) 
  {
    explosion->draw();
    return;
  }
  if( !hasExploded )
  {
    Uint32 x = static_cast<Uint32>(X());
    Uint32 y = static_cast<Uint32>(Y());
    SDL_Surface* surf = frame->getSurface();
    surf = zoomSurface(surf, zoom, zoom, SMOOTHING_ON);
    std::string name = "meteor";
    Frame* immediate = new Frame(name, surf);
    immediate->draw(x, y);
    delete immediate;
    SDL_FreeSurface(surf);
  }
}

void Meteor::explode() { 
  if ( explosion ) return;
  Sprite sprite(getName(), getPosition(), getVelocity(), getFrame());
  explosion = new ExplodingPlayer(sprite); 
}

void Meteor::reset(){
  if( hasExploded )
  {
    hasExploded = false;
  }
}

int Meteor::getDistance(const Meteor *obj) const { 
  return hypot(X()-obj->X(), Y()-obj->Y());
}

void Meteor::update(Uint32 ticks) {
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

  if ( X() < 0) {
    velocityX( abs( velocityY() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  }

  if ( Y() > worldHeight-frameHeight) {
    Y(0); //wraparound on X-axis
    X( Gamedata::getInstance().getRandInRange(0, worldWidth-frameWidth) );
  }  
}
