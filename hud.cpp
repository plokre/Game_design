#include <string>
#include <sstream>
#include <iostream>
#include "hud.h"
#include "gamedata.h"
#include "ioManager.h"

Hud& Hud::getInstance() {
  if ( SDL_WasInit(SDL_INIT_VIDEO) == 0) {
    throw std::string("Must init SDL before HUD");
  }
  static Hud hud; 
  return hud;
}

Hud::Hud() :
  hudHeight(Gamedata::getInstance().getXmlInt("hud/Height")),
  hudWidth(Gamedata::getInstance().getXmlInt("hud/Width")),
  hudDraw(Gamedata::getInstance().getXmlBool("hud/Start")),
  limit(Gamedata::getInstance().getXmlInt("hud/Limit")),
  timeWhenHudDrawn( SDL_GetTicks() ),
  clock( Clock::getInstance() )
{

}

void Hud::toggleHudDraw() {
  hudDraw = !hudDraw;
  if(hudDraw)
    timeWhenHudDrawn = SDL_GetTicks();
}

void Hud::drawHud(SDL_Surface* screen, int x, int y)
{
  if( (SDL_GetTicks()-timeWhenHudDrawn < (limit*1000) ) )
  {
    const Uint32 RED = SDL_MapRGB(screen->format, 0xff, 0, 0);
    Draw_AALine(screen, x, y+hudHeight/2, 
                        x+hudWidth,y+hudHeight/2, 
                        hudHeight, 0xff, 0xff, 0xff, 0xff/2);
    Draw_AALine(screen, x,y, x+hudWidth,y, RED);
    clock.draw();
    unsigned int textX = Gamedata::getInstance().getXmlInt("hud/x");
    unsigned int textY = Gamedata::getInstance().getXmlInt("hud/y");
    IOManager::getInstance().printMessageAt("Press W for moving up", textX, textY);
    IOManager::getInstance().printMessageAt("Press A for moving left", textX, textY+20);
    IOManager::getInstance().printMessageAt("Press S for moving down", textX, textY+40);
    IOManager::getInstance().printMessageAt("Press D for moving right", textX, textY+60);
    IOManager::getInstance().printMessageAt("Press SpaceBar for moving faster", textX, textY+80);    
    IOManager::getInstance().printMessageAt("Press P for Pause", textX, textY+130);    
    IOManager::getInstance().printMessageAt("Press F3 for Slow motion", textX, textY+150);    
    IOManager::getInstance().printMessageAt("Press H for shooting bullet", textX, textY+100);
  }
  else
  {
    hudDraw = false;
  }
}
