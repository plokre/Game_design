#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include "clock.h"
#include "gamedata.h"
#include "ioManager.h"

Clock& Clock::getInstance() {
  if ( SDL_WasInit(SDL_INIT_VIDEO) == 0) {
    throw std::string("Must init SDL before Clock");
  }
  static Clock clock; 
  return clock;
}

Clock::Clock() :
  frames(0),
  ticksSinceLastFrame(0),
  ticksAtLastFrame(0),
  sumOfTicks(0),
  frameCap(Gamedata::getInstance().getXmlInt("frameCap")),  
  tickIncr(0),
  frameTime(0.0),
  started(false), 
  paused(false), 
  sloMo(false), 
  fpsLoc(Gamedata::getInstance().getXmlInt("clock/fpsLoc/x"),
         Gamedata::getInstance().getXmlInt("clock/fpsLoc/y")),
  secondsLoc(Gamedata::getInstance().getXmlInt("clock/secondsLoc/x"),
         Gamedata::getInstance().getXmlInt("clock/secondsLoc/y")),
  capFrameRate(Gamedata::getInstance().getXmlBool("framesAreCapped"))
{
    start();
}

Clock::Clock(const Clock& c) :
  frames(c.frames), 
  ticksSinceLastFrame(c.ticksSinceLastFrame), 
  ticksAtLastFrame(c.ticksAtLastFrame), 
  sumOfTicks(c.sumOfTicks),
  frameCap(c.frameCap),  
  tickIncr(c.tickIncr),
  frameTime(c.frameTime),
  started(c.started), 
  paused(c.paused), 
  sloMo(c.sloMo), 
  fpsLoc(c.fpsLoc),
  secondsLoc(c.secondsLoc),
  capFrameRate(c.capFrameRate)
{
}

void Clock::draw() const { 
  IOManager::getInstance().
    printMessageValueAt("fps: ", getFps(), fpsLoc[0], fpsLoc[1]);
  IOManager::getInstance().
    printMessageValueAt("seconds: ", getSeconds(), 
                                     secondsLoc[0], secondsLoc[1]);
}

void Clock::update() { 
  if ( paused ) return;
  ++frames;
  unsigned int clockTicks = SDL_GetTicks(); 
  ticksSinceLastFrame = clockTicks - ticksAtLastFrame;
  if(capFrameRate) 
  {
    if( ticksSinceLastFrame < (1000/frameCap) )
    {
      SDL_Delay( (1000/frameCap) - ticksSinceLastFrame);
      clockTicks = SDL_GetTicks();
    }
  }
  ticksSinceLastFrame = clockTicks - ticksAtLastFrame;
  sumOfTicks += ticksSinceLastFrame;
  ticksAtLastFrame = clockTicks;
  frameTime = 0.2*ticksSinceLastFrame + (1.0-0.2)*frameTime; //used to calculate weighted average.
}

unsigned int Clock::getTicksSinceLastFrame() const {
  if (paused) return 0;
  if (sloMo) return ticksSinceLastFrame/2;
  return ticksSinceLastFrame;
}

void Clock::toggleSloMo() {
  sloMo = !sloMo;
}

void Clock::toggleFrameRateControl(){
  capFrameRate = !capFrameRate;
  frames = 0;
  sumOfTicks = 0;
}

int Clock::getFps() const { 
  if(frameTime > 0) return int(1000/frameTime);  
  return 0;
}

void Clock::start() { 
  started = true; 
  paused = false; 
  frames = 0;
  ticksAtLastFrame = SDL_GetTicks(); 
}

void Clock::pause() {
  if( started && !paused ) {
    paused = true;
  }
}

void Clock::unpause() {
  if( started && paused ) {
    ticksAtLastFrame = SDL_GetTicks();
    paused = false;
  }
}
