#include "multisprite.h"
#include "gamedata.h"

void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

MultiSprite::MultiSprite(const std::string& name, const Vector2f& pos, const Vector2f& vel):
	Drawable(name, pos, vel),
	frames( FrameFactory::getInstance().getFrames(name, zoomX, zoomY) ),
  	worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  	worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  	currentFrame(rand() % frames.size()),
  	numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  	frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  	timeSinceLastFrame( 0 ),
  	frameWidth(frames[0]->getWidth()),
  	frameHeight(frames[0]->getHeight())
{ 
}

MultiSprite::MultiSprite( const std::string& name ): 
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  frames( FrameFactory::getInstance().getFrames(name, zoomX, zoomY) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  currentFrame(rand() % frames.size()),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight())
{ 
	setPosition(Vector2f(rand()%(worldWidth - frameWidth), rand()%(worldHeight - frameHeight)));
}

void MultiSprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y);
}

void MultiSprite::update(Uint32 ticks) { 
  advanceFrame(ticks);
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  }  
}

void MultiSprite::zoom(){
	 std::size_t size = numberOfFrames;
	  while(size > 0){
		  --size;
  	      const_cast<Frame*>(frames[size]) -> zoom(zoomX, zoomY);
	  }
	  resetWH(frames[0] -> getWidth(), frames[0] -> getHeight());
	  if(Y() + frameHeight > worldHeight){
	      Y( worldHeight - frameHeight); 
	  }else if(X() + frameWidth > worldWidth){
		  X(worldWidth - frameWidth);
	  } 
}
