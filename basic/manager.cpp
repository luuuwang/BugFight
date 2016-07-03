#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <map>
#include "sprite.h"
#include "enemy.h"
#include "multisprite.h"
#include "gamedata.h"
#include "manager.h"

Manager::~Manager() { 
  // Manager made it, so Manager needs to delete it
  freeSprites();
  delete player;
  delete health;
  delete hud;
  //SDL_FreeSurface(screen);
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  //world("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  worlds(),
  worldsNumber(Gamedata::getInstance().getXmlInt("defaultWorldsNumber")),
  viewport( Viewport::getInstance() ),
  sprites(),
  biggerSprites(),
  enemies(),
	explodingEnemies(),
  currentSprite(),
  spritesNumber(Gamedata::getInstance().getXmlInt("defaultSpritsNumber")),
  trackableNumber(Gamedata::getInstance().getXmlInt("defaultEnemyNumber")),
  player(new Player("player")),
  health(new Health()),
  hud(new Hud(false)),
	timeVal(0),
	totalSeconds(0),
	seconds(0),
	isGodMode(false),
	isEasyMode(true),
	isDead(false),
	sound(),
  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  TITLE( Gamedata::getInstance().getXmlStr("screenTitle") )
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  atexit(SDL_Quit);

	initSprites(); //My code to initial all the sprits, including worlds
	currentSprite = sprites.end() - trackableNumber * 3;
	viewport.setObjectToTrack(player);
}

void Manager::switchSprite() {
  ++currentSprite;
  if ( currentSprite == sprites.end() ) {
    currentSprite = sprites.end() - trackableNumber * 3;
  }
  viewport.setObjectToTrack(*currentSprite);
}

void Manager::draw() const {
  //world.draw();
  drawAllSprites(); //Draw worlds together
  clock.draw();
  viewport.draw();
  std::map<std::string, int> info;
  info["fps"] = clock.getFps();
  info["seconds"] = clock.getSeconds();
  info["freeBullets"] = player -> getFreeCount();
  info["busyBullets"] = player -> getBusyCount();
  info["liveGhosts"] = enemies.size();
  info["deadGhosts"] = explodingEnemies.size();
  if(clock.getSeconds() <= 3 || hud -> show()){
  	hud -> draw(info);
  }
  health -> draw();
  if(isGodMode){
  	IOManager::getInstance().printMessageCenteredAt("God Mode", 30);
  }
  if(isEasyMode){
  	IOManager::getInstance().printMessageCenteredAt("Easy Mode", 50);
  }else{
  	IOManager::getInstance().printMessageCenteredAt("Hard Mode", 50);
  }
  player -> draw();
  if(enemies.size() == 0){
	hud -> drawShield();
	std::stringstream sstrm;
	sstrm << "You win in " << (totalSeconds - seconds) << " seconds";
  	IOManager::getInstance().printMessageCenteredAt(sstrm.str(), 190);
  	IOManager::getInstance().printMessageCenteredAt("Press R to restart game", 210);
  }
  SDL_Flip(screen);
}

void Manager::update() {
  clock.update();
  Uint32 ticks = clock.getTicksSinceLastFrame();
  if(ticks != 0){
	  ++timeVal;
  }
  updateAllSprites(ticks); //Also update all the worlds
  viewport.update();
  player -> update(ticks);
  if(player -> getShootting()){
		sound[0];
  }
  updateHealthPos();
  if(isDead && !player -> getIsExploding()){
  	health -> reset();
	timeVal = 0;
	isDead = false;
  }
  if(enemies.size() == 0 && totalSeconds == 0){
  	totalSeconds = clock.getSeconds();
  }

  if ( makeVideo && frameCount < frameMax ) {
    std::stringstream strm;
    strm << "frames/" << username<< '.' 
         << std::setfill('0') << std::setw(4) 
         << frameCount++ << ".bmp";
    std::string filename( strm.str() );
    std::cout << "Making frame: " << filename << std::endl;
    SDL_SaveBMP(screen, filename.c_str());
  }
}


void Manager::play() {
  SDL_Event event;

  bool done = false;
  while ( not done ) {
    
    while ( SDL_PollEvent(&event) ) {
      Uint8 *keystate = SDL_GetKeyState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
	  if(event.type == SDL_KEYUP){ 
		  //if(!(keystate[SDLK_w] || keystate[SDLK_UP] || keystate[SDLK_s] || keystate[SDLK_DOWN] || keystate[SDLK_a] || keystate[SDLK_LEFT] || keystate[SDLK_d] || keystate[SDLK_RIGHT])){
  	  	 	player -> stop();
		  //}
		  	if(!keystate[SDLK_SPACE]){
		  		player -> setShootting(false);
			}
  	  }
      //if (event.type == SDL_KEYDOWN) {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
          break;
        }

        if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
        if (keystate[SDLK_p] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
		if(keystate[SDLK_l]){
			clock.toggleSloMo();
		}
		if(keystate[SDLK_F1]){
			hud -> update();
		}
		if(keystate[SDLK_F2]){
			hud -> updatePool();
		}
		if(keystate[SDLK_w] && keystate[SDLK_s]){
			player -> stop();
		}else if(keystate[SDLK_a] && keystate[SDLK_d]){
			player -> stop();
		}else{
			if(keystate[SDLK_w]){
  	  	    	player -> goUp();
  	    	}else if(keystate[SDLK_s]){
  	  	    	player -> goDown();
  	    	}
  	    	if(keystate[SDLK_a]){
  	  	    	player -> goLeft();
  	    	}else if(keystate[SDLK_d]){
  	  	    	player -> goRight();
  	    	}
		}
		if(keystate[SDLK_SPACE]){
			player -> setShootting(true);
		}
		if(keystate[SDLK_r]){
			//if(ghosts.size() == 0){
				resetGame();
			//}
		}
		if(keystate[SDLK_g]){
			isGodMode = !isGodMode;
		}
		if(keystate[SDLK_m]){
			toggleEasyMode();
		}
      //}
    }
    update();
    draw();
  }
}

void Manager::initSprites(){ //My code
	for(unsigned int i = 0; i < spritesNumber; ++i){
		sprites.push_back(new MultiSprite("fire"));
		biggerSprites.push_back(new MultiSprite("explosion"));
	}
	for(unsigned int i = 0; i < trackableNumber; ++i){
		enemies.push_back(new Enemy("enemy"));
	}

	for(unsigned int i = 1; i <= worldsNumber; ++i){
		std::ostringstream convert;
		convert << i;
		std::string name = "back" + convert.str();
		worlds.push_back(new World(name, Gamedata::getInstance().getXmlInt(name + "/factor")));	
	}
}

void Manager::drawAllSprites() const { //My code
	worlds[0] -> draw();
	for(unsigned int i = 0; i < spritesNumber; ++i){
		sprites[i] -> draw();
	}
	worlds[1] -> draw();
	for(unsigned int i = 0; i < spritesNumber; ++i){
		biggerSprites[i] -> draw();
	}
	worlds[2] -> draw();
	static int t = 0; 
	++t;
	float posX = player -> X();
	for(unsigned int i = 0; i < enemies.size(); ++i){
		enemies[i] -> draw();
		if(!isDead && t > 3 && abs(enemies[i] -> X() - posX) <= 500){
			dynamic_cast<Enemy*>(enemies[i]) -> shoot(player -> getPosition());
		}
	}
	t = t > 3 ? 0 : t;
	for(unsigned int i = 0; i < explodingEnemies.size(); ++i){
		explodingEnemies[i] -> draw();
	}
}

void Manager::updateAllSprites(Uint32 ticks){ //My code
	std::sort(sprites.begin(), sprites.end(), Depth());
	std::sort(biggerSprites.begin(), biggerSprites.end(), Depth());
	std::sort(enemies.begin(), enemies.end(), Depth());
	for(unsigned int i = 0; i < worldsNumber; ++i){
		worlds[i] -> update();
	}
	for(unsigned int i = 0; i < spritesNumber; ++i){
		sprites[i] -> update(ticks);
		biggerSprites[i] -> update(ticks);
	}
	for(unsigned int i = 0; i < enemies.size(); ++i){
		enemies[i] -> update(ticks);
	}
	for(unsigned int i = 0; i < explodingEnemies.size(); ++i){
		explodingEnemies[i] -> update(ticks);
	}
	  //If bullets collides with enemies 
  std::vector<Drawable*>::iterator iter = enemies.begin();
  while(iter != enemies.end()){
		if(player -> collidedWith(*iter)){
			(*iter) -> explode();
			if((*iter) -> getIsExploding()){
				explodingEnemies.push_back(*iter);
				iter = enemies.erase(iter);
				sound[1];
			}
		}else{
			if(!isDead && dynamic_cast<Enemy*>(*iter) -> collidedWith(player)){
				if(!isGodMode){
					health -> update();
					player -> updateHealth();
				}
				if(health -> getHealth() == 0){
					player -> explode();
					isDead = true;
					sound[1];
				}
			}
			++iter;
		}
  }
  
  //If player collides with enemy or enemys' bullets
	iter = enemies.begin();
	if(timeVal > 100 && !isDead){
		while(iter != enemies.end()){
			if(player -> collisionDetect(*iter)){
				if(!isGodMode){
					health -> update();
					player -> updateHealth();
				}
				if(health -> getHealth() == 0){
					sound[1];
					player -> explode();
					isDead = true;
				}
					(*iter) -> explode();
					if((*iter) -> getIsExploding()){
						sound[1];
						explodingEnemies.push_back(*iter);
						iter = enemies.erase(iter);
					}
				
				break;
			}
			++iter;
		}
	}
}

void Manager::freeSprites(){
	for(unsigned int i = 0; i < spritesNumber; ++i){
		delete sprites[i];
		delete biggerSprites[i];
	}
	for(unsigned int i = 0; i < enemies.size(); ++i){
		delete enemies[i];
	}
	for(unsigned int i = 0; i < explodingEnemies.size(); ++i){
		delete explodingEnemies[i];
	}
	for(unsigned int i = 0; i < worldsNumber; ++i){
		delete worlds[i];
	}
}

void Manager::resetGame(){
	std::vector<Drawable*>::iterator iter = enemies.begin(); 
	while(iter != enemies.end()){
		(*iter) -> reset();
		++iter;
	}
	iter = explodingEnemies.begin();
	while(iter != explodingEnemies.end()){
		enemies.push_back(*iter);
		(*iter) -> setIsExploding(false);
		(*iter) -> reset();
		++iter;
	}	
	explodingEnemies.clear();
	player -> reset();
	health -> reset();
	isGodMode = false;
	isEasyMode = true;
	timeVal = 0;
	seconds = clock.getSeconds();
	totalSeconds = 0;
}

void Manager::toggleEasyMode(){
	std::vector<Drawable*>::iterator iter = enemies.begin(); 
	while(iter != enemies.end()){
		dynamic_cast<Enemy*>(*iter) -> toggleEasyMode();
		++iter;
	}
	isEasyMode = !isEasyMode;
}

void Manager::updateHealthPos(){
  player -> updateHealthPos(player -> getPosition() - viewport.getPosition());
	std::vector<Drawable*>::iterator iter = enemies.begin(); 
	while(iter != enemies.end()){
		dynamic_cast<Enemy*>(*iter) -> updateHealthPos((*iter) -> getPosition() - viewport.getPosition());
		++iter;
	}
}
