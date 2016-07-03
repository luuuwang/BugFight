#include <iostream>
#include "bullets.h"

Bullets::Bullets(): freeList(), busyList(), cs(new PerPixelCollisionStrategy()), timeVal(9){
}

Bullets::~Bullets(){
	delete cs;
	std::list<Bullet*>::iterator iter = freeList.begin();
	while(iter != freeList.end()){
		delete *iter;
		++iter;
	}
	iter = busyList.begin();
	while(iter != busyList.end()){
		delete *iter;
		++iter;
	}

}

void Bullets::stop(){
	std::list<Bullet*>::iterator iter = busyList.begin();
	while(iter != busyList.end()){
		freeList.push_back(*iter);
		++iter;
	}
	busyList.clear();
}

bool Bullets::collisionDetect(const Drawable* d){
	std::list<Bullet*>::iterator iter = busyList.begin();
	while(iter != busyList.end()){
		if(cs -> execute(**iter, *d)){
			freeList.push_back(*iter);
			iter=busyList.erase(iter);
			return true;
		}
		++iter;
	}
	return false;
}
void Bullets::shoot(const std::string& name, const Vector2f& pos, const Vector2f& vel){
	++timeVal;
	if(timeVal > 10){
		timeVal = 0;
		if(freeList.size() != 0){
			Bullet* cur = freeList.front();
			freeList.pop_front();
			cur -> setPosition(pos);
			cur -> setInitPosX(pos[0]);
			cur -> setInitPosY(pos[1]);
			cur -> setVelocity(vel);
			busyList.push_back(cur);
		}else{
			busyList.push_back(new Bullet(name, pos, vel));
		}
	}
}


void Bullets::update(Uint32 ticks){
	std::list<Bullet*>::iterator iter = busyList.begin();
	while(iter != busyList.end()){
		(*iter) -> update(ticks);
		if((*iter) -> goneTooFar()){
			freeList.push_back(*iter);
			iter=busyList.erase(iter);
		}else{
			++iter;
		}
	}
}

void Bullets::draw() const{
	std::list<Bullet*>::const_iterator iter = busyList.begin();
	while(iter != busyList.end()){
		(*iter) -> draw();
		++iter;
	}
}
