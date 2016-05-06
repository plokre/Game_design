#ifndef DRAWABLE__H
#define DRAWABLE__H
#include <SDL.h>
#include <iostream>
#include <string>
#include "vector2f.h"
#include "frame.h"
#include <cmath>
 
class Drawable {
public:
  Drawable(const std::string& n, const Vector2f& pos, const Vector2f& vel): 
    name(n), position(pos), velocity(vel) {}

  Drawable(const Drawable& s) : 
    name(s.name), position(s.position), velocity(s.velocity)
    { }

  virtual ~Drawable() {}

  const std::string& getName() const { return name; }
  void setName(const std::string& n) { name = n;    }
  virtual const Frame* getFrame() const = 0;

  virtual void draw() const = 0;
  virtual void update(Uint32 ticks) = 0;

  float X() const { return position[0]; }
  void X(float x) { position[0] = x;    }

  float Y() const { return position[1]; }
  void Y(float y) { position[1] = y;    }

  float velocityX() const  { return velocity[0]; }
  void velocityX(float vx) { velocity[0] = vx;   }
  float velocityY() const  { return velocity[1]; }
  void velocityY(float vy) { velocity[1] = vy;   }

  const Vector2f& getVelocity() const   { return velocity; }
  void setVelocity(const Vector2f& vel) { velocity = vel;  }
  const Vector2f& getPosition() const   { return position; }
  void setPosition(const Vector2f& pos) { position = pos;  }

  virtual bool collidedWith(const Drawable*) const { 
    throw std::string("No collidedWith");  
  }

  virtual void explode() { throw name+std::string(" can't explode"); }

  float distance(float x1, float y1, float x2, float y2) const {
    float x = x1-x2, y = y1-y2;
    return hypot(x, y);
  }

  bool execute(const Drawable& obj1, const Drawable& obj2) const {
    int COLLISION_DISTANCE = 
      obj1.getFrame()->getWidth()/3 + obj2.getFrame()->getWidth()/2;
    float xobj1 = obj1.X()+obj1.getFrame()->getWidth()/2;
    float yobj1 = obj1.Y()+obj1.getFrame()->getHeight()/2;
    float xobj2 = obj2.X() + obj2.getFrame()->getWidth()/2;
    float yobj2 = obj2.Y() + obj2.getFrame()->getHeight()/2;
    return distance(xobj1, yobj1, xobj2, yobj2) < COLLISION_DISTANCE;
  }

private:
  std::string name;
  Vector2f position;
  Vector2f velocity;
};
#endif
