#include "health.h"

Health::Health() :
  screen(IOManager::getInstance().getScreen()),
  start(Vector2f(Gamedata::getInstance().getXmlInt("health/startLoc/x"), 
			  Gamedata::getInstance().getXmlInt("health/startLoc/y"))), 
  totalLength(Gamedata::getInstance().getXmlInt("health/length")), 
  currentLength(totalLength), 
  thick(Gamedata::getInstance().getXmlInt("health/thick")), 
  decrements(Gamedata::getInstance().getXmlInt("health/decrements")),
  RED( SDL_MapRGB(screen->format, 0xff, 0x00, 0x00) ),
  GRAY( SDL_MapRGB(screen->format, 0xce, 0xb4, 0xb4) ),
  BLACK( SDL_MapRGB(screen->format, 0x00, 0x00, 0x00) ),
  color(RED) {
}

Health::Health(int sx, int sy, int tl, int cl,
               float t, int dec, Uint32 c):
  screen(IOManager::getInstance().getScreen()),
  start(Vector2f(sx, sy)), 
  totalLength(tl), 
  currentLength(cl), 
  thick(t), 
  decrements(dec),
  RED( SDL_MapRGB(screen->format, 0xff, 0x00, 0x00) ),
  GRAY( SDL_MapRGB(screen->format, 0xff, 0xff, 0xff) ),
  BLACK( SDL_MapRGB(screen->format, 0x00, 0x00, 0x00) ),
    color(c) {
}

void Health::drawBox() const {
  Draw_AALine(screen, start[0], start[1], 
                      start[0]+totalLength, start[1], 
                      thick, GRAY);
  // Two Horizontal lines
  Draw_AALine(screen, start[0], start[1]-(thick/2 + 1), 
                      start[0]+totalLength, start[1]-(thick/2 + 1), 
                      1.0, BLACK);
  Draw_AALine(screen, start[0], start[1]+(thick/2 + 1), 
                      start[0]+totalLength, start[1]+(thick/2 + 1), 
                      1.0, BLACK);
  // Two Vertical lines
  Draw_AALine(screen, start[0]-1, start[1]-(thick/2 + 1), 
                      start[0]-1, start[1]+(thick/2 + 1), 
                      2.0, BLACK);
  Draw_AALine(screen, start[0]+totalLength+1, start[1]-(thick/2 + 1), 
                      start[0]+totalLength+1, start[1]+(thick/2 + 1), 
                      2.0, BLACK);
}

void Health::draw() const {
  drawBox();
  Draw_AALine(screen, start[0], start[1], 
                      start[0] + currentLength, start[1], 
                      thick, color);
}
void Health::update() {
	currentLength = currentLength - decrements < 0 ? 0 : currentLength - decrements;
}
