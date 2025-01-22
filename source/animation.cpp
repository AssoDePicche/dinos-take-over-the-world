#include "animation.h"

Animation::Animation(double frameTime, unsigned first, unsigned last, bool loop,
                     bool restart)
    : frameTime{frameTime},
      firstFrame{first},
      lastFrame{last},
      currentFrame{firstFrame},
      loop{loop},
      restart{restart} {}

void Animation::draw(void) const {}

void Animation::update(double now) {
  if (now <= this->timer + this->frameTime) {
    this->ended = false;

    return;
  }

  this->timer = now;

  ++this->currentFrame;

  if (this->currentFrame < this->lastFrame) {
    this->ended = false;

    return;
  }

  if (this->loop || (this->restart && this->ended)) {
    this->currentFrame = this->firstFrame;

    this->ended = false;

    return;
  }

  this->currentFrame = this->lastFrame;

  this->ended = true;
}
