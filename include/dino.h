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

typedef enum DinoState {
  DINO_STATE_IDLE,
  DINO_STATE_MOVING_RIGHT,
  DINO_STATE_MOVING_LEFT,
} DinoState;

typedef struct Dino {
  Rectangle box;
  Vector2 velocity;
  DinoState state;
} Dino;

#define DINO_SPRITE_FRAMES (u8)24u

#define WORLD_GRAVITY (f32)9.81f

#endif
