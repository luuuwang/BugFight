#ifndef HEALTH__H 
#define HEALTH__H

#include "vector2f.h"
#include "ioManager.h"
#include "aaline.h"

class Health {
public:
  Health();
  Health(int sx, int sy, int tl, int cl, 
         float t, int inc, Uint32 c);
  void draw() const ;
  void update();
  void reset() { currentLength = totalLength; }
  int getHealth(){ return currentLength; }
  void setPosition(const Vector2f& pos){
	  start[0] = pos[0] + 5;
	  start[1] = pos[1] - thick;
  }
  void setDecrements(int dec){
  	decrements = dec;
  }
  void reset(int len, int t){
	  float ratio = decrements / (float)totalLength;
  	totalLength = len - 10;
	decrements = ratio * totalLength;
	currentLength = len - 10;
	thick = t;
  }
private:
  SDL_Surface* screen;
  Vector2f start;
  int totalLength;
  int currentLength;
  int thick;
  int decrements;
  const Uint32 RED;
  const Uint32 GRAY;
  const Uint32 BLACK;
  const Uint32 color;
  void drawBox() const;
  Health(const Health&);
  Health& operator=(const Health&);
};

#endif
