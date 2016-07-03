#ifndef BULLETS__H
#define BULLETS__H

#include <list>
#include "bullet.h"
#include "collisionStrategy.h"

class Bullets{
	public:
		Bullets();
		~Bullets();
		bool collisionDetect(const Drawable* d);
		void shoot(const std::string& name, const Vector2f& pos, const Vector2f& vel);
		bool shooting(){
			return busyList.size() > 0;
		}
		int getFreeCount(){
			return freeList.size();
		}
		int getBusyCount(){
			return busyList.size();
		}
		void update(Uint32 ticks);
		void draw() const;
		void stop();
	private:
		std::list<Bullet*> freeList;
		std::list<Bullet*> busyList;
		CollisionStrategy* cs; 
		int timeVal;

		Bullets(const Bullets&);
		Bullets& operator=(const Bullets&);
};

#endif
