#ifndef ENEMY__H 
#define ENEMY__H 
#include <string>
#include <vector>
#include "drawable.h"
#include "frameFactory.h"
#include "explodingSprite.h"
#include "collisionStrategy.h"
#include "bullets.h"
#include "vector2f.h"
#include "health.h"

class Enemy : public Drawable {
public:
  Enemy(const std::string&);
  virtual ~Enemy() { 
	  if(explosion != NULL){
	  	delete explosion; 
	  }
	  delete cs;
  } 

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return (*currentFrames)[currentFrame]; 
  }

  void explode();
  void shoot(const Vector2f&);
  void reset();
  int getFreeCount(){
	  return upLeftBullets.getFreeCount();
  }
  int getBusyCount(){
	  return upLeftBullets.getBusyCount();
  }
  bool collidedWith(const Drawable* d){
    return upLeftBullets.collisionDetect(d) || upRightBullets.collisionDetect(d) 
	   || downLeftBullets.collisionDetect(d) || downRightBullets.collisionDetect(d);	
  }
  bool collisionDetect(const Drawable* d){
  	return cs->execute(*this, *d);
  }
  virtual void zoom();
   void resetWH(int w, int h){
	  frameWidth = w;
	  frameHeight = h;
  }
   virtual void setIsExploding(bool isExploding){
	   explosion -> done();
	   this -> isExploding = isExploding;
   }
	virtual bool getIsExploding(){
		return isExploding;
	}

   int getWidth(){
	   return frameWidth;
   }
   int getHeight(){
   	return frameHeight;
   }
   int getWorldWidth(){
   	return worldWidth;
   }
   int getWorldHeight(){
   	return worldHeight;
   }
   void toggleEasyMode(){
   	easyMode = !easyMode;
   }
   void updateHealthPos(const Vector2f& pos){
   	health.setPosition(pos);
   }
   void updateHealth(){
   	health.update();
   }

protected:
   ExplodingSprite* explosion;
   bool isExploding;
   bool easyMode;
  const std::vector<Frame *> leftFrames;
  const std::vector<Frame *> rightFrames;
  const std::vector<Frame *> *currentFrames;
  int worldWidth;
  int worldHeight;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;
  Bullets upLeftBullets;
  Bullets upRightBullets;
  Bullets downLeftBullets;
  Bullets downRightBullets;
  bool shootting;
  CollisionStrategy* cs;
  Health health;
  int safeTimeVal;

  void advanceFrame(Uint32 ticks);
  Enemy(const Enemy&);
  Enemy& operator=(const Enemy&);
};
#endif
