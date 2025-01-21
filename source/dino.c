#include "dino.h"

void UpdateAnimation(Animation *this, const f64 now) {
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
  UpdateAnimation(this->animation, now);

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

  if (this->state == SPRITE_STATE_HURTING && this->lifes != 0u &&
      this->animation->ended) {
    --this->lifes;
  }

  if ((this->state != SPRITE_STATE_ATTACKING &&
       this->state != SPRITE_STATE_HURTING) ||
      this->animation->ended) {
    this->state = SPRITE_STATE_IDLE;
  }
}
