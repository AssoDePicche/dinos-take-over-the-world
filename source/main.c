#include "dino.h"

int main(void) {
  const u8 gamepad = 0;

  InitWindow(480, 360, "Dinos Take Over The World!");

  SetExitKey(KEY_Q);

  SetTargetFPS(60);

  Dino dino = (Dino){
      .box = (Rectangle){.x = 0, .y = 0, .width = 48.0f, .height = 48.0f},
      .velocity = (Vector2){.x = 10.0f, .y = 200.0f},
      .state = DINO_STATE_IDLE};

  Animation animation =
      (Animation){.texture = LoadTexture("./resources/sprites/blue.png"),
                  .timer = 0.0f,
                  .frameTime = 0.12f,
                  .startFrame = 0u,
                  .endFrame = 4u,
                  .currentFrame = 0u,
                  .textureFrames = 24u};

  while (!WindowShouldClose()) {
    dino.state = DINO_STATE_IDLE;

    if (IsKeyDown(KEY_A) ||
        IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) {
      dino.state = DINO_STATE_MOVING_LEFT;

      dino.box.x = MAX(dino.box.x - 10, 0);
    }

    if (IsKeyDown(KEY_D) ||
        IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) {
      dino.state = DINO_STATE_MOVING_RIGHT;

      dino.box.x = MIN(dino.box.x + 10, GetScreenWidth() - dino.box.width);
    }

    UpdateAnimation(&animation);

    dino.box.y = MIN(dino.box.y + dino.velocity.y * GetFrameTime(),
                     GetScreenHeight() - dino.box.height);

    dino.velocity.y = dino.velocity.y + WORLD_GRAVITY;

    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawRectangleLinesEx(dino.box, 1.0f, RED);

    DrawDinoAnimation(&dino, &animation);

    EndDrawing();
  }

  UnloadTexture(animation.texture);

  CloseWindow();

  return 0;
}
