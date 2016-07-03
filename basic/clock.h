#include <SDL.h>
#include <string>
#include <deque>

class Manager;

class Clock {
public:
  static Clock& getInstance();  // This class is a Singleton
  unsigned int getTicksSinceLastFrame() ;
  unsigned int getTicksSinceInit() const; 

  void toggleSloMo();
  bool isStarted() const { return started; }
  bool isPaused() const  { return paused;  }
  unsigned getSeconds() const { return getTicksSinceInit()/1000;  }
  int getFps() const;

  void start();
  void pause();
  void unpause();
  void draw() const;  // For debugging
  void update();  // Increments time/ticks

private:
  unsigned int ticks;
  unsigned int seconds;
  unsigned int frames;
  bool started;
  bool paused;
  bool sloMo;
  unsigned int frameCap;
  unsigned int totalTicks;
  unsigned int pausedTicks;
  unsigned int sumOfTicks;

  Clock();
  Clock(const Clock&);
  Clock&operator=(const Clock&);
};
