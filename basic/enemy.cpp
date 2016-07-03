#include "enemy.h"
#include "gamedata.h"

void Enemy::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

Enemy::Enemy( const std::string& name ): 
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
	explosion(NULL),
	isExploding(false),
	easyMode(true),
  leftFrames( FrameFactory::getInstance().getFrames(name + "Left", zoomX, zoomY) ),
  rightFrames(FrameFactory::getInstance().getFrames(name + "Right", zoomX, zoomY)),
  currentFrames(&leftFrames),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth((*currentFrames)[0]->getWidth()),
  frameHeight((*currentFrames)[0]->getHeight()),
	upLeftBullets(),
	upRightBullets(),
	downLeftBullets(),
	downRightBullets(),
	shootting(false),
	cs(new PerPixelCollisionStrategy()),
   health(),
   safeTimeVal(0)	
{ 
	setPosition(Vector2f(rand()%(worldWidth - frameWidth) + 700, rand()%(worldHeight - frameHeight)));
	int flag = (rand() % 2) == 0 ? 1 : -1;
	velocityY(flag * velocityY());

	health.setPosition(getPosition());
	health.reset(frameWidth, 7);
	health.setDecrements((frameWidth - 10) / 2);
}

void Enemy::shoot(const Vector2f& playerPos){
	if(easyMode){
	  if(velocityX() < 0 && velocityY() < 0){//Up left
		upLeftBullets.shoot("enemyBulletUpLeft", getPosition() + Vector2f(-frameWidth / 3, frameHeight * 1 / 5), Vector2f(velocityX() - 200, velocityY() - 200));
	  }else if(velocityX() > 0 && velocityY() < 0){//Up right
		upRightBullets.shoot("enemyBulletUpRight", getPosition() + Vector2f(frameWidth * 3 / 4, frameHeight * 1 / 5), Vector2f(velocityX() + 200, velocityY() - 200));
	  }else if(velocityX() < 0 && velocityY() > 0){//Down left
		downLeftBullets.shoot("enemyBulletDownLeft", getPosition() + Vector2f(-frameWidth / 3, frameHeight * 2 / 5), Vector2f(velocityX() - 200, velocityY() + 200));
	  }else if(velocityX() > 0 && velocityY() > 0){//Down right
		downRightBullets.shoot("enemyBulletDownRight", getPosition() + Vector2f(frameWidth * 3 / 4, frameHeight * 2 / 5), Vector2f(velocityX() + 200, velocityY() + 200));
	  }
	}else{
		if(playerPos[0] < X() && playerPos[1] < Y()){//Player is at up left of enemy
			upLeftBullets.shoot("enemyBulletUpLeft", getPosition() + Vector2f(-frameWidth / 3, frameHeight * 1 / 5), Vector2f(velocityX() - 200, velocityY() - 200));
			if(velocityX() > 0){
				velocityX(-velocityX());
				currentFrames = &leftFrames;
				currentFrame = 0;
			}
			if(velocityY() > 0){
				velocityY(-velocityY());
			}
		}else if(playerPos[0] > X() && playerPos[1] < Y()){//Up right
			upRightBullets.shoot("enemyBulletUpRight", getPosition() + Vector2f(frameWidth * 3 / 4, frameHeight * 1 / 5), Vector2f(velocityX() + 200, velocityY() - 200));
			if(velocityX() < 0){
				velocityX(-velocityX());
				currentFrames = &rightFrames;
				currentFrame = 0;
			}
			if(velocityY() > 0){
				velocityY(-velocityY());
			}
		}else if(playerPos[0] < X() && playerPos[1] > Y()){//Down left
			downLeftBullets.shoot("enemyBulletDownLeft", getPosition() + Vector2f(-frameWidth / 3, frameHeight * 2 / 5), Vector2f(velocityX() - 200, velocityY() + 200));
			if(velocityX() > 0){
				velocityX(-velocityX());
				currentFrames = &leftFrames;
				currentFrame = 0;
			}
			if(velocityY() < 0){
				velocityY(-velocityY());
			}
		}else if(playerPos[0] > X() && playerPos[1] > Y()){//Down right
			downRightBullets.shoot("enemyBulletDownRight", getPosition() + Vector2f(frameWidth * 3 / 4, frameHeight * 2 / 5), Vector2f(velocityX() + 200, velocityY() + 200));
			if(velocityX() < 0){
				velocityX(-velocityX());
				currentFrames = &rightFrames;
				currentFrame = 0;
			}
			if(velocityY() < 0){
				velocityY(-velocityY());
			}
		}
	}
}

void Enemy::explode(){
  if ( isExploding ) {
	  return;
  }
  if(safeTimeVal > 0){
  	health.update();
  	if(health.getHealth() == 0){
  		isExploding = true;
  		if(explosion == NULL){
  			Sprite sprite(getName(), getPosition(), getVelocity(), getFrame());
  			explosion = new ExplodingSprite(sprite);  
  		}else{
  			explosion -> reset(getPosition());
  		}
  	}
  }
}

void Enemy::reset(){
	setPosition(Vector2f(rand()%(worldWidth - frameWidth) + 700, rand()%(worldHeight - frameHeight)));
	int flag = (rand() % 2) == 0 ? 1 : -1;
	velocityY(flag * velocityY());
	upLeftBullets.stop();
	upRightBullets.stop();
	downLeftBullets.stop();
	downRightBullets.stop();
	easyMode = true;
	health.reset(frameWidth, 7);
	safeTimeVal = 0;
}

void Enemy::draw() const {  
  if (isExploding) {
    explosion->draw();
  }else{
  	Uint32 x = static_cast<Uint32>(X());
  	Uint32 y = static_cast<Uint32>(Y());
  	(*currentFrames)[currentFrame]->draw(x, y);
		health.draw();
  }
	upLeftBullets.draw();
	upRightBullets.draw();
	downLeftBullets.draw();
	downRightBullets.draw();
}

void Enemy::update(Uint32 ticks) { 
	if(ticks != 0){
		++safeTimeVal;
	}
	upLeftBullets.update(ticks);
	upRightBullets.update(ticks);
	downLeftBullets.update(ticks);
	downRightBullets.update(ticks);
  if ( isExploding ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
		//isExploding = false;
		//health.reset(frameWidth, 7);
    }
    return;
  }
  advanceFrame(ticks);
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() - 15 < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
	currentFrames = &rightFrames;
	currentFrame = 0;
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
	currentFrames = &leftFrames;
	currentFrame = 0;
  }  
	//health.setPosition(getPosition());
}

void Enemy::zoom(){
	 std::size_t size = numberOfFrames;
	  while(size > 0){
		  --size;
  	      const_cast<Frame*>((*currentFrames)[size]) -> zoom(zoomX, zoomY);
	  }
	  resetWH((*currentFrames)[0] -> getWidth(), (*currentFrames)[0] -> getHeight());
	  if(Y() + frameHeight > worldHeight){
	      Y( worldHeight - frameHeight); 
	  }else if(X() + frameWidth > worldWidth){
		  X(worldWidth - frameWidth);
	  } 
}
