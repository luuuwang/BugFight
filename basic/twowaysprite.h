#ifndef TWOWAYSPRITE_H 
#define TWOWAYSPRITE_H 
#include <string>
#include <vector>
#include "drawable.h"
#include "frameFactory.h"

class TwowaySprite: public Drawable {
public:
  TwowaySprite( const std::string& name, int dir);
  virtual ~TwowaySprite() { } 

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
	  if(direction % 2 == 1){
		return framesOne[currentFrame];
	  }else{
	  	return framesTwo[currentFrame];
	  }
  }
    virtual void zoom();
	 void resetWH(int w, int h){
	  frameWidth = w;
	  frameHeight = h;
  }

protected:
  const std::vector<Frame *> framesOne;
  const std::vector<Frame *> framesTwo;
  int direction; //1 - right 2 - left 3 - up 4 - down
  int worldWidth;
  int worldHeight;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;

  void advanceFrame(Uint32 ticks);
  TwowaySprite(const TwowaySprite&);
  TwowaySprite& operator=(const TwowaySprite&);
};
#endif
