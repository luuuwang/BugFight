#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
#include "clock.h"
#include "gamedata.h"
#include "ioManager.h"

Clock& Clock::getInstance() {
  if ( SDL_WasInit(SDL_INIT_VIDEO) == 0) {
    throw std::string("Must init SDL before Clock");
  }
  static Clock clock; 
  return clock;
}

Clock::Clock() :
  ticks(0),
	seconds(0),
	frames(0),
  started(false), 
  paused(false), 
  sloMo(false), 
	frameCap(Gamedata::getInstance().getXmlInt("frameCap")),
	totalTicks(0),
	pausedTicks(0),
  sumOfTicks(0)
  {
  start();
}

Clock::Clock(const Clock& c) :
  ticks(c.ticks), 
	seconds(c.seconds),
	frames(c.frames),
  started(c.started), 
  paused(c.paused), 
  sloMo(c.sloMo), 
	frameCap(c.frameCap),
	totalTicks(c.totalTicks),
	pausedTicks(c.pausedTicks),
  sumOfTicks(c.sumOfTicks)
  {
  start();
}

void Clock::draw() const { 
	IOManager::getInstance().printMessageCenteredAt("Lu Wang", 10);
}

void Clock::update() {
	if(!paused){
		totalTicks = SDL_GetTicks();
		ticks = totalTicks - sumOfTicks;
		if(ticks * frameCap < 1000){
			SDL_Delay(1000 / frameCap - ticks);
			//ticks = 1000 / frameCap;
		}
		totalTicks = SDL_GetTicks();
		ticks = totalTicks - sumOfTicks;
		sumOfTicks += ticks;
	}else{
		sumOfTicks = SDL_GetTicks();
	}
}

unsigned Clock::getTicksSinceInit() const { 
  if (paused) return totalTicks - pausedTicks;
  else return SDL_GetTicks() - pausedTicks; 
  
}

int Clock::getFps() const { 
	if(ticks > 0){
		return 1000/ticks;
	}
	return 0;
}

unsigned int Clock::getTicksSinceLastFrame() {
	if(paused){
 	 return 0;
	}
	if(sloMo){
		return 1;
	}
	return ticks;
}

void Clock::toggleSloMo() {
	sloMo = sloMo ? false : true;
}


void Clock::start() { 
  started = true; 
  paused = false; 
}

void Clock::pause() { 
	if( started && !paused ) {
    	paused = true;
 	}
}

void Clock::unpause() { 
  if( started && paused ) {
	pausedTicks += sumOfTicks - totalTicks;
    paused = false;
  }	
}

