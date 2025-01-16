#include "dino.h"

int main(void) {
  const u8 gamepad = 0;

  InitWindow(480, 360, "Dinos Take Over The World!");

  SetExitKey(KEY_Q);

  SetTargetFPS(60);

  const f64 DEFAULT_FRAME_TIME = 1.0f / 12.0f;

  Sprite sprite = (Sprite){
      .texture = LoadTexture("./resources/sprites/blue.png"),
      .box = (Rectangle){.x = 0,
                         .y = GetScreenHeight() - 48.0f,
                         .width = 48.0f,
                         .height = 48.0f},
      .state = SPRITE_STATE_IDLE,
      .textureFrames = 24u,
      .walk_speed = 5.0f,
      .run_speed = 8.0f,
      .facingRight = true,
      .isRunning = false,
  };

  Animation animation[] = {
      (Animation){
          .timer = 0.0f,
          .frameTime = DEFAULT_FRAME_TIME,
          .firstFrame = 0u,
          .lastFrame = 4u,
          .currentFrame = 0u,
          .loop = true,
      },
      (Animation){
          .timer = 0.0f,
          .frameTime = DEFAULT_FRAME_TIME,
          .firstFrame = 5u,
          .lastFrame = 10u,
          .currentFrame = 5u,
          .loop = true,
      },
      (Animation){
          .timer = 0.0f,
          .frameTime = DEFAULT_FRAME_TIME,
          .firstFrame = 18u,
          .lastFrame = 24u,
          .currentFrame = 18u,
          .loop = true,
      },
      (Animation){
          .timer = 0.0f,
          .frameTime = DEFAULT_FRAME_TIME,
          .firstFrame = 10u,
          .lastFrame = 13u,
          .currentFrame = 10u,
          .loop = false,
      },
      (Animation){
          .timer = 0.0f,
          .frameTime = DEFAULT_FRAME_TIME,
          .firstFrame = 14u,
          .lastFrame = 17u,
          .currentFrame = 14u,
          .loop = false,
      },
  };

  while (!WindowShouldClose()) {
    if (sprite.state != SPRITE_STATE_ATTACKING) {
      sprite.state = SPRITE_STATE_IDLE;
    }

    sprite.isRunning =
        IsKeyDown(KEY_LEFT_SHIFT) ||
        IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);

    if (IsKeyDown(KEY_A) ||
        IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) {
      sprite.facingRight = false;

      sprite.state =
          sprite.isRunning ? SPRITE_STATE_RUNNING : SPRITE_STATE_WALKING;
    }

    if (IsKeyDown(KEY_D) ||
        IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) {
      sprite.facingRight = true;

      sprite.state =
          sprite.isRunning ? SPRITE_STATE_RUNNING : SPRITE_STATE_WALKING;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ||
        IsGamepadButtonPressed(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) {
      animation[SPRITE_STATE_ATTACKING].currentFrame =
          animation[SPRITE_STATE_ATTACKING].firstFrame;

      sprite.state = SPRITE_STATE_ATTACKING;
    }

    sprite.animation = &animation[sprite.state];

    BeginDrawing();

    ClearBackground(RAYWHITE);

    UpdateSprite(&sprite, GetTime());

    DrawSprite(&sprite);

    EndDrawing();
  }

  UnloadTexture(sprite.texture);

  CloseWindow();

  return 0;
}
