#ifndef __DINO__
#define __DINO__

#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX(X, Y) X > Y ? X : Y

#define MIN(X, Y) X < Y ? X : Y

typedef double f64;

typedef float f32;

typedef uint64_t u64;

typedef uint32_t u32;

typedef uint16_t u16;

typedef uint8_t u8;

typedef int64_t i64;

typedef int32_t i32;

typedef int16_t i16;

typedef int8_t i8;

typedef char *Str;

typedef struct Animation {
  f64 timer;
  f64 frameTime;
  u8 firstFrame;
  u8 lastFrame;
  u8 currentFrame;
  bool loop;
} Animation;

typedef enum SpriteState {
  SPRITE_STATE_IDLE = 0u,
  SPRITE_STATE_WALKING,
  SPRITE_STATE_RUNNING,
  SPRITE_STATE_ATTACKING,
  SPRITE_STATE_HURTING,
} SpriteState;

typedef struct Sprite {
  Animation *animation;
  Texture2D texture;
  Rectangle box;
  f32 walk_speed;
  f32 run_speed;
  u8 textureFrames;
  SpriteState state;
  bool facingRight;
  bool isRunning;
} Sprite;

void DrawSprite(const Sprite *);

void UpdateSprite(Sprite *, const f64);

#endif