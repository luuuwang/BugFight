#include "SDL/SDL_rotozoom.h"
#include "drawable.h"
#include "frame.h"
#include "ioManager.h"
#include "viewport.h"

Frame::Frame( const std::string& name, SDL_Surface* surf ) : 
	name(name),
  screen(IOManager::getInstance().getScreen()),
  originalSurface(surf),
  surface( surf ),
  width(surf -> w), 
  height(surf -> h),
  sourceX(0),
  sourceY(0)
{ }

Frame::Frame( SDL_Surface* spr, Uint16 w, Uint16 h,
              Sint16 src_x, Sint16 src_y) :
	name(),
  screen(IOManager::getInstance().getScreen()),
  originalSurface(spr),
  surface(spr), 
  width(w), height(h),
  sourceX(src_x), sourceY(src_y) {
}

Frame::Frame( const Frame& frame ) :
	name(frame.name),
  screen(frame.screen),
  originalSurface(frame.originalSurface),
  surface(frame.surface), 
  width(frame.width), 
  height(frame.height),
  sourceX(frame.sourceX), 
  sourceY(frame.sourceY) 
  { }


Frame& Frame::operator=(const Frame& rhs) {
	name = rhs.name;
  originalSurface = rhs.originalSurface;
  surface = rhs.surface;
  screen = rhs.screen;
  width = rhs.width;
  height = rhs.height;
  sourceX = rhs.sourceX;
  sourceY = rhs.sourceY;
  return *this;
}

void Frame::draw(Sint16 x, Sint16 y) const {
  SDL_Rect src = { sourceX, sourceY, width, height };    
  x -= Viewport::getInstance().X();
  y -= Viewport::getInstance().Y();
  SDL_Rect dest = {x, y, width, height };
  SDL_BlitSurface(surface, &src, screen, &dest);
}

void Frame::draw(Sint16 sx, Sint16 sy, Sint16 dx, Sint16 dy) const {
  SDL_Rect src = { sx, sy, width, height };    
  SDL_Rect dest = {dx, dy, width, height };
  SDL_BlitSurface(surface, &src, screen, &dest);
}

void Frame::draw(Sint16 x, Sint16 y, double angle) const {
  SDL_Surface* tmp = rotozoomSurface(surface, angle, 1, 1);
  Sint16 zero = 0;
  Uint16 width = tmp->w;
  Uint16 height = tmp->h;
  SDL_Rect src = { zero, zero, width, height };    
  x -= Viewport::getInstance().X();
  y -= Viewport::getInstance().Y();
  SDL_Rect dest = {x, y, 0, 0 };
  SDL_BlitSurface(tmp, &src, screen, &dest);
  SDL_FreeSurface( tmp );
}

