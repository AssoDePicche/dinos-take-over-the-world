#include "dino.h"

void DrawSprite(const Sprite *this) {
  const u64 sprite_width = this->texture.width / this->textureFrames;

  const u64 sprite_height = this->texture.height;

  const u64 sprite_scale = this->box.width / sprite_width;

  Rectangle source = (Rectangle){
      this->animation->currentFrame * sprite_width,
      0.0f,
      sprite_width,
      sprite_height,
  };

  Rectangle destination = (Rectangle){
      this->box.x + sprite_width,
      this->box.y + sprite_height,
      sprite_scale * sprite_width,
      sprite_scale * this->texture.height,
  };

  Vector2 origin = (Vector2){sprite_width, sprite_height};

  if (!this->facingRight) {
    source.width = -source.width;
  }

  if (this->hasShadow) {
    DrawTexturePro(this->shadow,
                   (Rectangle){
                       .x = 0.0f,
                       .y = 0.0f,
                       .width = 24.0f,
                       .height = 24.0f,
                   },
                   (Rectangle){
                       .x = this->box.x,
                       .y = this->box.y,
                       .width = 48.0f,
                       .height = 48.0f,
                   },
                   (Vector2){
                       .x = 0.0f,
                       .y = 0.0f,
                   },
                   0, WHITE);
  }

  DrawTexturePro(this->texture, source, destination, origin, 0, WHITE);

  DrawRectangleLinesEx(this->box, 1.0f, RED);
}

void UpdateSprite(Sprite *this, const f64 now) {
  if (this->state == SPRITE_STATE_WALKING) {
    this->box.x = this->facingRight ? MIN(this->box.x + this->walk_speed,
                                          GetScreenWidth() - this->box.width)
                                    : MAX(this->box.x - this->walk_speed, 0);
  }

  if (this->state == SPRITE_STATE_RUNNING) {
    this->box.x = (this->facingRight) ? MIN(this->box.x + this->run_speed,
                                            GetScreenWidth() - this->box.width)
                                      : MAX(this->box.x - this->run_speed, 0);
  }

  if (now <= this->animation->timer + this->animation->frameTime) {
    return;
  }

  this->animation->timer = now;

  ++this->animation->currentFrame;

  if (this->animation->currentFrame < this->animation->lastFrame) {
    return;
  }

  this->animation->currentFrame = this->animation->loop
                                      ? this->animation->firstFrame
                                      : this->animation->lastFrame;
}
