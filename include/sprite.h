#pragma once

#include <raylib.h>

#include <memory>

#include "animation.h"

enum State {
  SPRITE_STATE_IDLE = 0u,
  SPRITE_STATE_WALKING,
  SPRITE_STATE_RUNNING,
  SPRITE_STATE_ATTACKING,
  SPRITE_STATE_HURTING,
};

struct Sprite final {
  Texture2D texture;
  Texture2D shadow;
  Rectangle box;
  std::shared_ptr<Animation> animation;
  float walkSpeed;
  float runSpeed;
  unsigned textureFrames;
  unsigned lifes;
  State state;
  bool facingRight;
  bool isRunning;
  bool hasShadow;

  void draw(void) const;

  void update(double);
};
