#include "player.h"
#include "gamedata.h"
#include "frameFactory.h"

void Player::advanceFrame(Uint32 ticks) 
{
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame > frameInterval)
  {
    currentFrame = (currentFrame+1) % (numberOfFrames/2);
    if(velocityX() < 0) 
      currentFrame = currentFrame + (numberOfFrames/2);
    if(velocityX() == 0)
    { 
      if(direction == 0)
        currentFrame = currentFrame + (numberOfFrames/2);
    }
    timeSinceLastFrame = 0;
  }
}

Player::Player( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"), 
                    Gamedata::getInstance().getXmlInt(name+"/speedY")) 
),

  explosion(NULL),
  frames( FrameFactory::getInstance().getFrames(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  movSpeedX( Gamedata::getInstance().getXmlInt(name+"/movSpeedX") ),
  movSpeedY( Gamedata::getInstance().getXmlInt(name+"/movSpeedY") ),
  direction( 1 ),

  bulletName("bullet"),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/bulletInterval")),
  timeSinceLastBullet(0),
  minBulletSpeed(Gamedata::getInstance().getXmlInt(bulletName+"/minBulletSpeed")),
  bullets(bulletName),
  hasExploded( false )
{ 
}

Player::Player(const Player& s) :
  Drawable(s),
  explosion(s.explosion), 
  frames(s.frames),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
  movSpeedX( s.movSpeedX ),
  movSpeedY( s.movSpeedY ),
  direction( s.direction ),
  bulletName( s.bulletName ),
  bulletInterval( s.bulletInterval ),
  timeSinceLastBullet( s.timeSinceLastBullet ),
  minBulletSpeed( s.minBulletSpeed ),
  bullets( s.bullets ),
  hasExploded( s.hasExploded )
{ }

void Player::draw() const { 
  if (explosion) 
  {
    explosion->draw();
    return;
  }
  if( !hasExploded )
  {
    Uint32 x = static_cast<Uint32>(X());
    Uint32 y = static_cast<Uint32>(Y());
    frames[currentFrame]->draw(x, y);
    bullets.draw();
  }
}

void Player::explode() { 
  if ( explosion ) return;
  Sprite sprite(getName(), getPosition(), getVelocity(), getFrame());
  explosion = new ExplodingPlayer(sprite); 
}

void Player::reset()
{
  X(Gamedata::getInstance().getXmlInt(getName()+"/startLoc/x"));
  Y(Gamedata::getInstance().getXmlInt(getName()+"/startLoc/y")); 
  if( hasExploded )
    hasExploded = false;
}
 
bool Player::collidedWith(const Drawable* obj1,const Drawable* obj2)
{
  if(execute(*obj1, *obj2))
  {
    return true;
  }
  return false;
}
  
int Player::smartCheck(const Drawable* obj1)
{
  int i;
  i = bullets.smartCheck(obj1);
  return i;
}

void Player::update(Uint32 ticks) { 
  if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }

  advanceFrame(ticks);
  Uint8 *keyPressed = SDL_GetKeyState(NULL);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if(keyPressed[SDLK_h])
  {
    shoot();
  }
  //vertical movement
  if(keyPressed[SDLK_w])
  {
    if(Y() > 0)
      velocityY( -movSpeedY );
    else
      Y(0);
  }
  else if(keyPressed[SDLK_s])
  {
    if(Y() < worldHeight-frameHeight)
      velocityY( movSpeedY );
    else
      Y(worldHeight-frameHeight);
  }
  else
    velocityY(0);

  //horizontal movement
  if(keyPressed[SDLK_d])
  {
    if(X() < worldWidth-frameWidth)
      velocityX( movSpeedY );
    else
      X(worldWidth-frameWidth);
    direction = 1;   
  }
  else if(keyPressed[SDLK_a])
  {
    if(X() > 0)
      velocityX( -movSpeedY );
    else
      X(0);
    direction = 0;
  }
  else
    velocityX(0);
  
  //superspeed
  if(keyPressed[SDLK_SPACE])
    velocityX( 2*velocityX() );

  timeSinceLastBullet +=ticks;
  bullets.update(ticks);
}

void Player::shoot()
{
  //see if its time to shoot 
  if(timeSinceLastBullet > bulletInterval) 
  {
    Vector2f vel = getVelocity();
    float x;
    float y = Y()+ frameHeight/2;
    if(direction == 1) 
    {
      x = X() + frameWidth;
      vel[0] += minBulletSpeed;
      
    }
    else 
    {
      x=X();
      vel[0] -= minBulletSpeed;
    }
    bullets.shoot(Vector2f(x,y),vel);
    timeSinceLastBullet = 0;
  }
}

bool Player::checkCollision(const Drawable* obj)
{
  if(bullets.collidedWith( obj, 10) )
  {
    return true;
  }
  return false;
}

