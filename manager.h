#include <vector>
#include <SDL.h>
#include "ioManager.h"
#include "clock.h"
#include "hud.h"
#include "world.h"
#include "viewport.h"
#include "sprite.h"
#include "meteor.h"
#include "player.h"
#include "explodingPlayer.h"
#include "opponent.h"
#include "sound.h"

class Manager {
public:
  Manager ();
  ~Manager ();
  void play();
  void resetMeteor();

private:
  const bool env;
  const IOManager& io;
  Clock& clock;
  Hud& hud;

  SDL_Surface * const screen;
  World back;
  World mid;
  World fore;
  Viewport& viewport;

  std::vector<Meteor*> sprites;
  std::vector<Player*> mainSprite;
  std::vector<Opponent*> rivalSprite;

  bool makeVideo;
  int frameCount;
  const std::string username;
  const std::string title;
  const int frameMax;
  bool godMode;

  SDLSound sound;

  void draw() const;
  void update();

  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
};
