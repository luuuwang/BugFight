#include <iostream>
#include "player.h"
#include "frameFactory.h"

Player::Player(const std::string& name): 
	Drawable(name, 
			Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           	Vector2f(0, 0)
		   ), 
		explosion(NULL),
	isExploding(false),
	leftFrames(FrameFactory::getInstance().getFrames(name + "Left", zoomX, zoomY)),
	rightFrames(FrameFactory::getInstance().getFrames(name + "Right", zoomX, zoomY)),
	currentFrames(&rightFrames),
	currentFrame(0),
	numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  	frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  	timeSinceLastFrame( 0 ),
	worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  	worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  	frameWidth(leftFrames[0]->getWidth()),
  	frameHeight(leftFrames[0]->getHeight()),
	xSpeed(Gamedata::getInstance().getXmlInt("player/speedX")),
	ySpeed(Gamedata::getInstance().getXmlInt("player/speedY")),
	xRange(Gamedata::getInstance().getXmlInt("player/xRange")),
	yRange(Gamedata::getInstance().getXmlInt("player/yRange")),	
	blood(200), magic(200), 
	rightBullets(),
	leftBullets(),
	shootting(false),
   cs(new PerPixelCollisionStrategy()),
   health()	
{
	health.setPosition(getPosition());
	health.reset(frameWidth, 7);
} 

void Player::shoot(){
	float velY = velocityY();
	int posY = getPosition()[1];
	if(posY == 0 || posY + frameHeight == worldHeight){
		velY = 0;
	}
	  if(currentFrames == &leftFrames){
		
		leftBullets.shoot("leftBullet", getPosition() + Vector2f(-frameWidth * 1 / 8, (frameHeight - 32) * 4 / 5), Vector2f(velocityX() - 400, velY));
	  }else if(currentFrames == &rightFrames){
		rightBullets.shoot("rightBullet", getPosition() + Vector2f(frameWidth * 3 / 4, (frameHeight - 32) * 3 / 5), Vector2f(velocityX() + 400, velY));
	  }
}
void Player::explode(){
  if ( isExploding ) {
	  return;
  }
  isExploding = true;
  if(explosion == NULL){
  	Sprite sprite(getName(), getPosition(), getVelocity(), getFrame());
  	explosion = new ExplodingSprite(sprite);  
  }else{
  	explosion -> reset(getPosition());
  }
}

void Player::reset(){
  	setPosition(Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")));	
	currentFrames = &rightFrames;
	currentFrame = 0;
	rightBullets.stop();
	leftBullets.stop();
	isExploding = false;
	health.reset(frameWidth, 7);
}

void Player::draw() const{
	if (isExploding) {
    	explosion->draw();
  	}else{
  		Uint32 x;
  		Uint32 y;
  		x = static_cast<Uint32>(X());
  		y = static_cast<Uint32>(Y());
  		(*currentFrames)[currentFrame] -> draw(x, y);
		health.draw();
	}
	rightBullets.draw();
	leftBullets.draw();
}

void Player::advanceFrame(Uint32 ticks) {
	//Vector2f vec = getVelocity();
	//if(vec[0] != 0 || vec[1] != 0){
		timeSinceLastFrame += ticks;
		if (timeSinceLastFrame > frameInterval) {
    		currentFrame = (currentFrame+1) % numberOfFrames;
			timeSinceLastFrame = 0;
		}
	//}else{
	//	currentFrame = 0;
	//}
}

void Player::update(Uint32 ticks){
	rightBullets.update(ticks);
	leftBullets.update(ticks);
	if ( isExploding ) {
    	explosion->update(ticks);
    	if ( explosion->chunkCount() == 0 ) {
			isExploding = false;
			health.reset(frameWidth, 7);
    	}
    	return;
  	}
	//zoom();
  	advanceFrame(ticks);	
  	Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  	setPosition(getPosition() + incr);
  	if (Y() - 15 < yRange){
		Y(yRange + 15);
  	}else if ( Y() > worldHeight - frameHeight){
		Y(worldHeight - frameHeight);
  	}
  	if ( X() < xRange){
		X(xRange);
  	}else if ( X() > worldWidth - frameWidth){
		X(worldWidth - frameWidth);
  	}
	if(shootting){
		this -> shoot();
	}
	//health.setPosition(getPosition());
}

void Player::zoom(){
	setZoom(zoomX, zoomY);
	std::size_t size = leftFrames.size();
	  while(size > 0){
		  --size;
  	      const_cast<Frame*>(leftFrames[size]) -> zoom(zoomX, zoomY);
  	      const_cast<Frame*>(rightFrames[size]) -> zoom(zoomX, zoomY);
	  }
	  resetWH(leftFrames[0] -> getWidth(), leftFrames[0] -> getHeight());
	  if(Y() + frameHeight > worldHeight){
	      Y( worldHeight - frameHeight); 
	  }else if(X() + frameWidth > worldWidth){
		  X(worldWidth - frameWidth);
	  } 
}
