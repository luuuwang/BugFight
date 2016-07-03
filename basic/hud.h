#ifndef HUD__H
#define HUD__H 
#include <map>
#include "vector2f.h"
#include "ioManager.h"
#include "aaline.h"

class Hud{
public:
	Hud(bool);
	void draw(std::map<std::string, int>&) const;  
  	void update();  
	void updatePool();
	void drawShield();
	bool show() const;

private:
	bool showInfo;	
	bool showPool;
	SDL_Surface* screen;
  	Vector2f start;
	const int width;
	const int height;
	const int width1;
	const int height1;
	const int viewWidth;
	const int viewHeight;
	const Uint32 color;
	const int alpha;
	
	Hud(const Hud&);
	Hud& operator=(const Hud&);
};

#endif



