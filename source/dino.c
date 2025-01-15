#include "dino.h"

void UpdateAnimation(Animation *animation) {
  const f64 CURRENT_TIME = GetTime();

  if (CURRENT_TIME <= animation->timer + animation->frameTime) {
    return;
  }

  animation->timer = CURRENT_TIME;

  ++animation->currentFrame;

  if (animation->currentFrame >= animation->endFrame) {
    animation->currentFrame = animation->startFrame;
  }
}

void DrawDinoAnimation(const Dino *dino, const Animation *animation) {
  Rectangle source =
      (Rectangle){animation->currentFrame * animation->texture.width /
                      animation->textureFrames,
                  0, animation->texture.width / animation->textureFrames,
                  animation->texture.height};

  Rectangle destination = (Rectangle){
      dino->box.x + animation->texture.width / animation->textureFrames,
      dino->box.y + animation->texture.height,
      2 * animation->texture.width / animation->textureFrames,
      animation->texture.height * 2};

  Vector2 origin = {animation->texture.width / animation->textureFrames,
                    animation->texture.height};

  DrawTexturePro(animation->texture, source, destination, origin, 0, WHITE);
}
