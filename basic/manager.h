#include <SDL.h>
#include <vector>
#include "ioManager.h"
#include "clock.h"
#include "world.h"
#include "player.h"
#include "health.h"
#include "hud.h"
#include "viewport.h"
#include "sound.h"

class Manager {
public:
  Manager ();
  ~Manager ();
  void play();
  void switchSprite();

private:
  const bool env;
  IOManager& io;
  Clock& clock;

  SDL_Surface * const screen;
  //World world;
  std::vector<World*> worlds;
  unsigned int worldsNumber;
  Viewport& viewport;

  //Sprite orb;
  std::vector<Drawable*> sprites;
  std::vector<Drawable*> biggerSprites;
  std::vector<Drawable*> enemies;
  std::vector<Drawable*> explodingEnemies;
  std::vector<Drawable*>::iterator currentSprite; //Lu: Current Sprite to track
  unsigned int spritesNumber; //My code to record orbs number
  unsigned int trackableNumber; //My code to record orbs number

  Player* player;
  Health* health;
  Hud* hud;
  unsigned int timeVal;
  unsigned int totalSeconds;
  unsigned int seconds;
  bool isGodMode;
  bool isEasyMode;
  bool isDead;
  SDLSound sound; //play music

  bool makeVideo;
  int frameCount;
  std::string username;
  int frameMax;
  const std::string TITLE;

  void draw() const;
  void update();

  Manager(const Manager&);
  Manager& operator=(const Manager&);

  void initSprites(); //My code to initial all the orbs
  void drawAllSprites() const; //My code to display all the orbs
  void updateAllSprites(Uint32 ticks); //My code to update positions of all the orbs
  void freeSprites(); //My code to delete all the orbs
  void resetGame();
  void toggleEasyMode();
  void updateHealthPos();
};

class Depth{
	public:
		bool operator()(const Drawable * d1, const Drawable *d2) const{
			return d1 -> Y() < d2 -> Y();
		}
};
