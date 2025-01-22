#include "sprite.h"

#define MAX(X, Y) X > Y ? X : Y

#define MIN(X, Y) X < Y ? X : Y

void Sprite::draw(void) const {
  const auto sprite_width = this->texture.width / this->textureFrames;

  const auto sprite_height = this->texture.height;

  const auto sprite_scale = this->box.width / sprite_width;

  Rectangle source(this->animation->currentFrame * sprite_width, 0.0f,
                   sprite_width, sprite_height);

  Rectangle destination(this->box.x + sprite_width, this->box.y + sprite_height,
                        sprite_scale * sprite_width,
                        sprite_scale * this->texture.height);

  Vector2 origin(sprite_width, sprite_height);

  if (!this->facingRight) {
    source.width = -source.width;
  }

  if (this->hasShadow) {
    Rectangle shadow_source(0.0f, 0.0f, 24.0f, 24.0f);

    Rectangle shadow_destination(this->box.x, this->box.y, 48.0f, 48.0f);

    Vector2 shadow_origin(0.0f, 0.0f);

    DrawTexturePro(this->shadow, shadow_source, shadow_destination,
                   shadow_origin, 0, WHITE);
  }

  DrawTexturePro(this->texture, source, destination, origin, 0, WHITE);

  DrawRectangleLinesEx(this->box, 1.0f, RED);
}

void Sprite::update(double now) {
  this->animation->update(now);

  if (this->state == SPRITE_STATE_WALKING) {
    this->box.x = this->facingRight ? MIN(this->box.x + this->walkSpeed,
                                          GetScreenWidth() - this->box.width)
                                    : MAX(this->box.x - this->walkSpeed, 0);
  }

  if (this->state == SPRITE_STATE_RUNNING) {
    this->box.x = (this->facingRight) ? MIN(this->box.x + this->runSpeed,
                                            GetScreenWidth() - this->box.width)
                                      : MAX(this->box.x - this->runSpeed, 0);
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
