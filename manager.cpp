#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include "sprite.h"
#include "gamedata.h"
#include "manager.h"
#include "meteor.h"
//#include "player.h"

struct Compare
{
  bool operator()(const Meteor* left, const Meteor* right) const 
  {
    return ( left->getScale() < right->getScale() );
  }
};

Manager::~Manager() { 
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
  for (unsigned i = 0; i < sprites.size(); ++i) {
    delete sprites[i];
  }
  for (unsigned i = 0; i < mainSprite.size(); ++i) {
    delete mainSprite[i];
  }
  for (unsigned i = 0; i < rivalSprite.size(); ++i) {
    delete rivalSprite[i];
  }
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  hud( Hud::getInstance() ),

  screen( io.getScreen() ),
  back("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  mid("mid", Gamedata::getInstance().getXmlInt("mid/factor") ),
  fore("fore", Gamedata::getInstance().getXmlInt("fore/factor") ),
  viewport( Viewport::getInstance() ),
  sprites(),
  mainSprite(),
  rivalSprite(),

  makeVideo( false ),
  frameCount( 0 ),
  username( Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  godMode( false ),
  sound()
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);
  int meteor_num = Gamedata::getInstance().getXmlInt("meteor/number");
  for(int i = 0; i < meteor_num; i++)
  {
    sprites.push_back( new Meteor("meteor") );
  }
  mainSprite.push_back( new Player("robot") );
  rivalSprite.push_back( new Opponent("spaceship") );
  viewport.setObjectToTrack(mainSprite[0]);
  std::sort(sprites.begin(), sprites.end(), Compare());
}

void Manager::draw() const {
  back.draw();
  for (unsigned i = 0; i < sprites.size()/3; ++i) {
    sprites[i]->draw();
  }
  mid.draw();
  for (unsigned i = sprites.size()/3; i < 2*sprites.size()/3; ++i) {
    sprites[i]->draw();
  }
  fore.draw();
  for (unsigned i = 2*sprites.size()/3; i < sprites.size(); ++i) {
    sprites[i]->draw();
  }

  mainSprite[0]->draw();
  rivalSprite[0]->draw();

  if(!rivalSprite[0]->getExplode())
  {
    if(mainSprite[0]->getExplode())
      io.printMessageAt( "Game Over! Press R for Restart.", 320, 240);
  }
  else
      io.printMessageAt( "YOU WIN!!.", 360, 240);
  io.printMessageAt(title, 10, 450);
  viewport.draw();
  hud.drawHud(screen, 20, 40);
  SDL_Flip(screen);
}

void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}

void Manager::update() {
  clock.update();
  Uint32 ticks = clock.getTicksSinceLastFrame();

  back.update();
  for (unsigned int i = 0; i < sprites.size()/3; ++i) {
    sprites[i]->update(ticks);
  }
  mid.update();
  for (unsigned int i = sprites.size()/3; i < 2*sprites.size()/3; ++i) {
    sprites[i]->update(ticks);
  }
  fore.update();

  //checking meteor collision
  for (unsigned int i = 2*sprites.size()/3; i < sprites.size(); ++i) 
  {
    //with main player
    if( (mainSprite[0]->collidedWith(mainSprite[0],sprites[i])) 
          && (!sprites[i]->getExplode()) && !godMode 
          &&  !mainSprite[0]->getExplode() )
    {
      sound[0];

      mainSprite[0]->explode();
      mainSprite[0]->changeExplode(true);

      sprites[i]->explode();
      sprites[i]->changeExplode(true);
    }
      
    //with bullets
    if( mainSprite[0]->checkCollision(sprites[i]) && 
        !sprites[i]->getExplode() )
    {
      sound[0];
      sprites[i]->explode();
      sprites[i]->changeExplode(true);
    }
  }

  //checking player collision with opponent ship 
  if( (mainSprite[0]->collidedWith(mainSprite[0],rivalSprite[0])) && 
      !rivalSprite[0]->getExplode() && !godMode && !mainSprite[0]->getExplode() )
  {
    sound[0];

    mainSprite[0]->explode();
    mainSprite[0]->changeExplode(true);

    rivalSprite[0]->explode();
    rivalSprite[0]->changeExplode(true);
  }

  //checking player collision with bullets fired from opponent ship
  if( rivalSprite[0]->checkCollision(mainSprite[0]) && 
      !mainSprite[0]->getExplode() && !godMode && !rivalSprite[0]->getExplode() )
  {
    sound[0];
    mainSprite[0]->explode();
    mainSprite[0]->changeExplode(true);
  }

  //checking opponent ship collision with bullets
  if( mainSprite[0]->checkCollision(rivalSprite[0]) && 
      !rivalSprite[0]->getExplode() && !mainSprite[0]->getExplode() )
  {
    sound[0];
    rivalSprite[0]->explode();
    rivalSprite[0]->changeExplode(true);
  }

  //AI check
  int j = mainSprite[0]->smartCheck(rivalSprite[0]);
  if(!j)
  {
    rivalSprite[0]->avoidBullet(false);    
  }
  else
  {
    //8 in 10 chance of hitting target
    int i = rand()%10;
    if(i < 2)
    {
      rivalSprite[0]->avoidBullet(true);
      if(j == 2)
        rivalSprite[0]->goDown();
      if(j == 1)
        rivalSprite[0]->goUp();
    }
  }

  for (unsigned int i = 2*sprites.size()/3; i < sprites.size(); ++i) {
      sprites[i]->update(ticks);
  }

  mainSprite[0]->update(ticks);
  rivalSprite[0]->update(ticks);

  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }
  viewport.update(); // always update viewport last
}

void Manager::resetMeteor(){
  for (unsigned int i = 2*sprites.size()/3; i < sprites.size(); ++i) {
      sprites[i]->reset();
  }
}
void Manager::play() {
  SDL_Event event;
  bool done = false;
  clock.start();

  while ( not done ) {
    while ( SDL_PollEvent(&event) ) {
      Uint8 *keystate = SDL_GetKeyState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
          break;
        }
        if ( keystate[SDLK_p] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDLK_r] ) {
          mainSprite[0]->reset();
          rivalSprite[0]->reset();
          resetMeteor();
        }
        if (keystate[SDLK_F1]) {
          hud.toggleHudDraw();
        }
        if (keystate[SDLK_F3]) {
          clock.toggleSloMo();
        }
        if (keystate[SDLK_F2]) {
          clock.toggleFrameRateControl();
        }
        if (keystate[SDLK_g]) {
          if(!godMode)
          {
            godMode=true;
	  }
          else
          {
            godMode=false;
          }
        }
        if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
      }
    }
    draw();
    update();
  }
}
