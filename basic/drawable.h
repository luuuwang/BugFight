#ifndef DRAWABLE__H
#define DRAWABLE__H

#include <SDL.h>
#include <string>

#include "vector2f.h"
#include "frame.h"
#include "gamedata.h"
 
// Drawable is an Abstract Base Class (ABC) that specifies
// the methods that derived classes may or must have.
class Drawable {
public:
  Drawable(const std::string& n, const Vector2f& pos, const Vector2f& vel): 
    name(n), position(pos), velocity(vel), 
	zoomX(Gamedata::getInstance().getXmlFloat(n + "/zx")),
 	zoomY(Gamedata::getInstance().getXmlFloat(n + "/zy"))	
	{
	}

  Drawable(const Drawable& s) : 
    name(s.name), position(s.position), velocity(s.velocity), zoomX(s.zoomX), zoomY(s.zoomY)
    { }

  virtual ~Drawable() {}

  const std::string& getName() const { return name; }
  void setName(const std::string& n) { name = n;    }
  virtual const Frame* getFrame() const = 0;

  virtual void draw() const = 0;
  virtual void update(Uint32 ticks) = 0;
  virtual void zoom() = 0;
  virtual void explode(){}
  virtual void reset(){}
  
  virtual void setIsExploding(bool isExploding){
  	std::cout << isExploding << std::endl;
  }
  virtual bool getIsExploding(){
  	return false;
  }

 

  void setZoom(double zx, double zy){
	  if(velocity[1] > 0){ //Go down
		  zx += 0.01;
		  zy += 0.01;
	  }else if(velocity[1] < 0){//Go up
		  zx -= 0.01;
		  zy -= 0.01;
	  }
	  if(zx < 0.3){
		  zoomX = 0.3;
	  }else if(zx > 1.0){
	      zoomX = 1.0;
	  }else{
		  zoomX = zx;
	  }
	  if(zy < 0.3){
		  zoomY = 0.3; 
	  }else if(zy > 1.0){
	      zoomY = 1.0;
	  }else{
		  zoomY = zy;
	  }
  }
  double getZoomX() const {
	  return zoomX;
  }
  double getZoomY() const {
	  return zoomY;
  }

  float X() const { return position[0]; }
  void X(float x) { position[0] = x;    }

  float Y() const { return position[1]; }
  void Y(float y) { position[1] = y;    }

  float velocityX() const  { return velocity[0]; }
  void velocityX(float vx) { velocity[0] = vx;   }
  float velocityY() const  { return velocity[1]; }
  void velocityY(float vy) { velocity[1] = vy;   }

  const Vector2f& getVelocity() const   { return velocity; }
  void setVelocity(const Vector2f& vel) { velocity = vel;  }
  const Vector2f& getPosition() const   { return position; }
  void setPosition(const Vector2f& pos) { position = pos;  }

protected:
  std::string name;
  Vector2f position;
  Vector2f velocity;
  double zoomX; 
  double zoomY;
};
#endif
