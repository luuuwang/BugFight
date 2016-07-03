#ifndef BULLET__H 
#define BULLET__H

#include <string>
#include <iostream>
#include "multisprite.h"

const double DISTANCE = 500;

class Bullet : public MultiSprite{
public:
  Bullet(const std::string& name, const Vector2f& pos, const Vector2f& vel);  
  virtual ~Bullet() { } 
  virtual void update(Uint32 ticks);

  int getWidth() const{
	  return frameWidth;
  }
  int getHeight() const{
	  return frameHeight;
  }
  void setWidth(int width){
	  frameWidth = width;
  }
  void setHeight(int height){
  	frameHeight = height;
  }

  bool goneTooFar(){
	  if(abs(X() - initPosX) > DISTANCE || abs(Y() - initPosY) > DISTANCE){
		  return true;
	  }else{
		  return false;
	  }
  }
  void setInitPosX(double ipx){
	  initPosX = ipx;
  }
  double getInitPosX(){
	  return initPosX;
  }
  void setInitPosY(double ipy){
	  initPosY = ipy;
  }
  double getInitPosY(){
	  return initPosY;
  }
private:
  double initPosX;
  double initPosY;

  int getDistance(const Bullet*) const;
  Bullet(const Bullet&);
  Bullet& operator=(const Bullet&);
};
#endif
