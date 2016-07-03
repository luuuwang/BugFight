#include "twowaysprite.h"
#include "gamedata.h"

void TwowaySprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

TwowaySprite::TwowaySprite( const std::string& name, int dir):
	Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
	framesOne(FrameFactory::getInstance().getFrames(name + "One", zoomX, zoomY)),
	framesTwo(FrameFactory::getInstance().getFrames(name + "Two", zoomX, zoomY)),
	direction(dir),
	worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(framesOne[0]->getWidth()),
  frameHeight(framesOne[0]->getHeight())
{
	setPosition(Vector2f(rand()%(worldWidth - frameWidth), rand()%(worldHeight - frameHeight)));
}


void TwowaySprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  //frames[currentFrame]->draw(x, y);
  if(direction % 2 != 0){ //Go right or go up
	  framesOne[currentFrame] -> draw(x, y);
  }else{ //Go left or go down
	  framesTwo[currentFrame] -> draw(x, y);
  }
}

void TwowaySprite::update(Uint32 ticks) { 
  advanceFrame(ticks);
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
	direction = 4; //Go down
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
	direction = 3; //Go up
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
	direction = 1; //Go right
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
	direction = 2; //Go left
  }  
}

  void TwowaySprite::zoom(){
	  std::size_t size = framesOne.size();
	  while(size > 0){
		  --size;
  	      const_cast<Frame*>(framesOne[size]) -> zoom(zoomX, zoomY);
  	      const_cast<Frame*>(framesTwo[size]) -> zoom(zoomX, zoomY);
	  }
	  resetWH(framesOne[0] -> getWidth(), framesOne[0] -> getHeight());
	  if(Y() + frameHeight > worldHeight){
	      Y( worldHeight - frameHeight); 
	  }else if(X() + frameWidth > worldWidth){
		  X(worldWidth - frameWidth);
	  }
  }
