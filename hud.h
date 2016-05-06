#ifndef HUD_H
#define HUD_H
#include <iostream>
#include "ioManager.h"
#include "clock.h"
#include "aaline.h"

class Hud {
public:
  static Hud& getInstance();
  void toggleHudDraw();
  void drawHud(SDL_Surface* , int , int );

private:
  int hudHeight;
  int hudWidth;
  bool hudDraw;

  unsigned int limit;
  unsigned int timeWhenHudDrawn;
  Clock& clock;

  Hud();
  Hud( const Hud& );
  Hud&operator=(const Hud&);
};
#endif
