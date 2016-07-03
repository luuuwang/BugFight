#include "hud.h"
#include <map>

Hud::Hud(bool show): showInfo(show), showPool(false),
	screen(IOManager::getInstance().getScreen()),
  	start(Vector2f(Gamedata::getInstance().getXmlInt("hud/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt("hud/startLoc/y"))), 
	width(Gamedata::getInstance().getXmlInt("hud/width")),
	height(Gamedata::getInstance().getXmlInt("hud/height")),
	width1(Gamedata::getInstance().getXmlInt("hud/width1")),
	height1(Gamedata::getInstance().getXmlInt("hud/height1")),
	viewWidth(Gamedata::getInstance().getXmlInt("view/width")),
	viewHeight(Gamedata::getInstance().getXmlInt("view/height")),
	color(SDL_MapRGB(screen->format, Gamedata::getInstance().getXmlInt("hud/red"), 
				Gamedata::getInstance().getXmlInt("hud/green"), Gamedata::getInstance().getXmlInt("hud/blue"))),
	alpha(Gamedata::getInstance().getXmlInt("hud/alpha"))
{
	if(start[0] + width > viewWidth){
		start[0] = viewWidth - width - 5;
	}
	if(start[1] + height > viewHeight){
		start[1] = viewHeight - height - 5;
	}
}

void Hud::draw(std::map<std::string, int>& info) const {
	if(showPool){
		//Background
		Draw_AALine(screen, start[0], start[1] + height1 / 2, 
                      start[0] + width1, start[1] + height1 / 2, 
                      height1, 0xff, 0xff, 0xff, alpha);
		// Two Horizontal lines
 		Draw_AALine(screen, start[0], start[1], 
                      start[0] + width1, start[1], 
                      1.0, color);
 		Draw_AALine(screen, start[0], start[1] + height1, 
                      start[0] + width1, start[1] + height1, 
                      1.0, color);
  			// Two Vertical lines
  		Draw_AALine(screen, start[0], start[1], 
                      start[0], start[1] + height1, 
                      2.0, color);
  		Draw_AALine(screen, start[0] + width1, start[1], 
                      start[0] + width1, start[1] + height1, 
                      2.0, color);

		IOManager::getInstance().printMessageValueAt("Live Enemies: ", info["liveGhosts"], start[0] + 20, start[1] + 10);
		IOManager::getInstance().printMessageValueAt("Dead Enemies: ", info["deadGhosts"], start[0] + 20, start[1] + 30);
		IOManager::getInstance().printMessageValueAt("Free bullets: ", info["freeBullets"], start[0] + 20, start[1] + 50);
		IOManager::getInstance().printMessageValueAt("Busy bullets: ", info["busyBullets"], start[0] + 20, start[1] + 70);
		IOManager::getInstance().printMessageAt("F1 -- Toggle Help", start[0] + 20, start[1] + 110);
		IOManager::getInstance().printMessageAt("F2 -- Toggle Pool", start[0] + 20, start[1] + 130);
	}else if(showInfo || info["seconds"] <= 3){
			//Background
			Draw_AALine(screen, start[0], start[1] + height / 2, 
                      start[0] + width, start[1] + height / 2, 
                      height, 0xff, 0xff, 0xff, alpha);
			// Two Horizontal lines
 		 	Draw_AALine(screen, start[0], start[1], 
                      start[0] + width, start[1], 
                      1.0, color);
 		 	Draw_AALine(screen, start[0], start[1] + height, 
                      start[0] + width, start[1] + height, 
                      1.0, color);
  			// Two Vertical lines
  			Draw_AALine(screen, start[0], start[1], 
                      start[0], start[1] + height, 
                      2.0, color);
  			Draw_AALine(screen, start[0] + width, start[1], 
                      start[0] + width, start[1] + height, 
                      2.0, color);

			IOManager::getInstance().printMessageValueAt("Fps: ", info["fps"], start[0] + 20, start[1] + 10);
			IOManager::getInstance().printMessageValueAt("Seconds: ", info["seconds"], start[0] + 130, start[1] + 10);
			IOManager::getInstance().printMessageAt("P -- Pause", start[0] + 20, start[1] + 30);
			IOManager::getInstance().printMessageAt("R -- Restart", start[0] + 20, start[1] + 50);
			IOManager::getInstance().printMessageAt("G -- God Mode", start[0] + 20, start[1] + 70);
			IOManager::getInstance().printMessageAt("Space -- Shoot", start[0] + 20, start[1] + 90);
			IOManager::getInstance().printMessageAt("M -- Change Mode", start[0] + 20, start[1] + 110);
			IOManager::getInstance().printMessageAt("ASWD -- Move Player", start[0] + 20, start[1] + 130);
			IOManager::getInstance().printMessageAt("Q or ESA -- Quit", start[0] + 20, start[1] + 150);
			IOManager::getInstance().printMessageAt("F1 -- Toggle Help", start[0] + 20, start[1] + 190);
			IOManager::getInstance().printMessageAt("F2 -- Toggle Pool", start[0] + 20, start[1] + 210);
	}
}

void Hud::drawShield(){
	//Background
	Draw_AALine(screen, 0, viewHeight/ 2, 
              viewWidth, viewHeight/ 2, 
              viewHeight, 0xff, 0xff, 0xff, alpha);
}


void Hud::update(){
	if(showPool){
		showPool = false;
	}
	showInfo = !showInfo;
}

void Hud::updatePool(){
	if(showInfo){
		showInfo = false;
	}
	showPool = !showPool;
}

bool Hud::show() const {
	return showInfo || showPool;
}
