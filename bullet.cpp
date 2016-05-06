#include "bullet.h"
#include "gamedata.h"
#include "frameFactory.h"

Bullet::Bullet(const std::string & str) :
  name(str),
  myVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX"),
             Gamedata::getInstance().getXmlInt(name+"/speedY")
  ),
  bulletFrames(FrameFactory::getInstance().getFrames(name)),
  bulletList()
{}

Bullet::Bullet(const Bullet& b) :
  name(b.name),
  myVelocity(b.myVelocity),
  bulletFrames(b.bulletFrames),
  bulletList(b.bulletList)
{}

void Bullet::shoot(const Vector2f& pos, const Vector2f& objVel){
  smallSprite b(name,pos, objVel);
  bulletList.push_back(b);
}

void Bullet::draw() const {
  std::list<smallSprite>::const_iterator bitr = bulletList.begin();
  while(bitr!=bulletList.end()){
     bitr->draw();
      ++bitr;
  }
}

void Bullet::reset(){
  std::list<smallSprite>::iterator bitr = bulletList.begin();
  while(bitr!=bulletList.end()){
    bitr->reset();
 }
}

int Bullet::getBulletlistSize(){
   return bulletList.size();
}

int Bullet::smartCheck(const Drawable* obj)
{
  std::list<smallSprite>::const_iterator bitr = bulletList.begin();
  while(bitr!=bulletList.end()){
     Vector2f pos= bitr->getPosition();
     float localHeight = bitr->getFrame()->getHeight();
     float objHeight = obj->getFrame()->getWidth();
     if(obj->X()-pos[0] < 100)
     {
       if( pos[1]+localHeight > obj->Y() && 
           pos[1]+localHeight/2< obj->Y()+objHeight/2 ) 
       {
         return 2; //go down
       }
       else if( (pos[1]+localHeight/2 >= obj->Y()+objHeight/2 && 
           pos[1] < obj->Y()+objHeight )  )      
       {
         return 1; //go up
       }
     }
     ++bitr;
  }  
  return 0;
}

bool Bullet::collidedWith(const Drawable * obj, int absDist)  
{
  std::list<smallSprite>::iterator bitr = bulletList.begin();
  while(bitr!=bulletList.end())
  {
    if(!bitr->getExplode())
    {
      if(bitr->execute(*bitr, *obj)&& absDist <15)
      {
        bitr->explode();
        bitr->changeExplode(true);
        bitr = bulletList.erase(bitr);
	return true;
      }
    }
    ++bitr;
  }
  return false;
}
 
void Bullet::update(Uint32 ticks)  {
  std::list<smallSprite>::iterator bitr = bulletList.begin();
  while(bitr!=bulletList.end()){
     bitr->update(ticks);
     if(bitr->goneTooFar()){
        bitr = bulletList.erase(bitr);
     }
     else ++bitr;
  }
} 
