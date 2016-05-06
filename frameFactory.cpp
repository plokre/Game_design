#include "frameFactory.h"
#include "extractSurface.h"
#include "ioManager.h"
#include "vector2f.h"

FrameFactory::FrameFactory() : 
  gdata( Gamedata::getInstance() ), 
  surfaces(),
  multiSurfaces(),
  frames(),
  multiFrames()
{}

FrameFactory::~FrameFactory() {
  for(std::map<std::string, SDL_Surface*>::iterator sitr = surfaces.begin(); sitr != surfaces.end(); sitr++)
  {
    if( (sitr->second) ) SDL_FreeSurface(sitr->second);
  }

  for(std::map<std::string, std::vector<SDL_Surface*> >::iterator msitr = multiSurfaces.begin(); msitr != multiSurfaces.end(); msitr++)
  {
    for(std::vector<SDL_Surface*>::iterator i = msitr->second.begin(); i != msitr->second.end(); i++)
    {
      if( (*i) )  SDL_FreeSurface(*i); 
    }
  }

  for(std::map<std::string, Frame*>::iterator fitr = frames.begin(); fitr != frames.end(); fitr++)
  {
    if(fitr->second)  delete fitr->second;
  }

  for(std::map<std::string, std::vector<Frame*> >::iterator mfitr = multiFrames.begin(); mfitr != multiFrames.end(); mfitr++)
  {
    for(std::vector<Frame*>::iterator j = mfitr->second.begin(); j != mfitr->second.end(); j++)
    {
      if( (*j) ) delete (*j);
    } 
  }
}


FrameFactory& FrameFactory::getInstance() {
  static FrameFactory factory;
  return factory;
}

Frame* FrameFactory::getFrame(const std::string& name) {
  std::map<std::string, Frame* >::const_iterator 
    posn = frames.find(name); 
  if ( posn != frames.end() ) {
    return posn->second;
  }

  SDL_Surface * const surface =
    IOManager::getInstance().loadAndSet(
        gdata.getXmlStr(name+"/file"),
        gdata.getXmlBool(name+"/transparency"));

  Frame * const frame = new Frame(name, surface);
  frames[name] = frame;
  surfaces[name] = surface;
  return frame;
}

std::vector<Frame*> FrameFactory::getFrames(const std::string& name) {
  // First search map to see if we've already made it:
  std::map<std::string, std::vector<Frame*> >::const_iterator 
    pos = multiFrames.find(name); 
  if ( pos != multiFrames.end() ) {
    return pos->second;
  }

  // It wasn't in the map, so we have to make the vector of Frames:
  SDL_Surface* surface = IOManager::
     getInstance().loadAndSet(gdata.getXmlStr(name+"/file"), true);
  unsigned numberOfFrames = gdata.getXmlInt(name+"/frames");
  std::vector<Frame*> frames;
  std::vector<SDL_Surface*> surfaces;
  frames.reserve(numberOfFrames);
  surfaces.reserve(numberOfFrames);
  Uint16 srcX = gdata.getXmlInt(name+"/srcX");
  Uint16 srcY = gdata.getXmlInt(name+"/srcY");
  Uint16 width = gdata.getXmlInt(name+"/width");
  Uint16 height = gdata.getXmlInt(name+"/height");

  SDL_Surface* surf;
  for (unsigned i = 0; i < numberOfFrames; ++i) {
    unsigned frameX = i * width + srcX;
   surf = ExtractSurface::getInstance().
               get(surface, width, height, frameX, srcY); 
    surfaces.push_back( surf );
    frames.push_back( new Frame(name, surf) );
  }
  SDL_FreeSurface(surface);
  multiSurfaces[name] = surfaces;
  multiFrames[name] = frames;
  return frames;
}
