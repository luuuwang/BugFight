#ifndef PLAYER_H 
#define PLAYER_H 

#include "drawable.h"
#include "bullets.h"
#include "collisionStrategy.h"
#include "explodingSprite.h"
#include "health.h"
#include <vector>

class Player: public Drawable{
public:
  Player(const std::string& name);  
  virtual ~Player() { 
	  delete cs; 
	  if(explosion != NULL){
	  	delete explosion;	
	  }
  }  // Don't delete frame; factory will reuse it 
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  void reset();
  virtual const Frame* getFrame() const { 
	  return (*currentFrames)[currentFrame];
  }
  
  void goUp(){
	  setVelocity(Vector2f(getVelocity()[0], -ySpeed));
	  //currentFrames = &upFrames;
  }
  void goDown(){
	  setVelocity(Vector2f(getVelocity()[0], ySpeed));
	  //currentFrames = &downFrames;
  }
  void goLeft(){
	  setVelocity(Vector2f(-xSpeed, getVelocity()[1]));
	  currentFrames = &leftFrames;
  }
  void goRight(){
	  setVelocity(Vector2f(xSpeed, getVelocity()[1]));
	  currentFrames = &rightFrames;
  }
  void stop(){
	  setVelocity(Vector2f(0, 0));
	  currentFrame = 0;
  }
  bool collidedWith(const Drawable* d){
    return rightBullets.collisionDetect(d) || leftBullets.collisionDetect(d); 
  }
  bool collisionDetect(const Drawable* d){
  	return cs->execute(*this, *d);
  }
  void shoot();
  void setShootting(bool isShoot){
  	shootting = isShoot;
  }
  bool getShootting(){
	  return shootting;
  }
  int getFreeCount(){
	  return rightBullets.getFreeCount() + leftBullets.getFreeCount();
  }
  int getBusyCount(){
	  return rightBullets.getBusyCount() + leftBullets.getBusyCount();
  }

  virtual void zoom();
  virtual void explode();
   void resetWH(int w, int h){
	  frameWidth = w;
	  frameHeight = h;
  }
   virtual void setIsExploding(bool isExploding){
	   this -> isExploding = isExploding;
   }
   virtual bool getIsExploding(){
	   return isExploding;
   }
   
   void updateHealthPos(const Vector2f& pos){
   	health.setPosition(pos);
   }
   void updateHealth(){
   	health.update();
   }
  
  void setBlood(int b) { blood = b; }
  void setMagic(int m) { magic = m; }
  int getBlood() const { return blood; }
  int getMagic() const { return magic; }

protected:
  ExplodingSprite* explosion;
  bool isExploding;
  const std::vector<Frame *> leftFrames;
  const std::vector<Frame *> rightFrames;
  const std::vector<Frame *> *currentFrames;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  int frameWidth;
  int frameHeight;

  double xSpeed;
  double ySpeed;
  int xRange;
  int yRange;
  int blood;
  int magic;
  Bullets rightBullets;
  Bullets leftBullets;
  bool shootting;
  CollisionStrategy* cs; 
  Health health;

  void advanceFrame(Uint32 ticks);
  Player(const Player&);
  Player& operator=(const Player&);
};
#endif
