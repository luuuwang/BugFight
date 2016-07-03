#include <iostream>
#include <cmath>
#include "bullet.h"
#include "gamedata.h"
#include "frameFactory.h"

Bullet::Bullet(const std::string& name, const Vector2f& pos, const Vector2f& vel) :
  MultiSprite(name, pos, vel),
  initPosX(pos[0]),
  initPosY(pos[1])
  {
  }

void Bullet::update(Uint32 ticks) {
  advanceFrame(ticks);
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
}
