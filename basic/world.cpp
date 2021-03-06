#include <iostream>
#include "world.h"

World::World(const std::string& name, int fact) : 
  io( IOManager::getInstance() ),
  frame(FrameFactory::getInstance().getFrame(name, 1.0, 1.0)),
  factor(fact),
    frameWidth( frame->getWidth() ),
    frameHeight( frame->getHeight() ),
    worldWidth( Gamedata::getInstance().getXmlInt("world/width") ),
    viewX(0.0), viewY(0.0), 
    view(Viewport::getInstance()) 
{}

void World::update() {
  viewX = static_cast<int>(view.X() / factor) % frameWidth;
  viewY = static_cast<int>(view.Y() / factor) % frameHeight;
}

void World::draw() const { 
  frame->draw(viewX, viewY, 0, 0); 
  frame->draw(0, viewY, frameWidth-viewX, 0); 
  frame->draw(viewX, 0, 0, frameHeight-viewY); 
  frame->draw(0, 0, frameWidth - viewX, frameHeight-viewY); 
}

