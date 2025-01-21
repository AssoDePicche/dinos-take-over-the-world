#include "dino.h"

int main(void) {
  const u8 gamepad = 0;

  InitWindow(480, 360, "Dinos Take Over The World!");

  SetExitKey(KEY_Q);

  SetTargetFPS(60);

  const f64 DEFAULT_FRAME_TIME = 1.0f / 12.0f;

  InitAudioDevice();

  Music theme = LoadMusicStream("./resources/music/manlorette_party.mp3");

  SetMusicVolume(theme, 0.75f);

  PlayMusicStream(theme);

  Texture2D keyboard_texture =
      LoadTexture("./resources/ui/desktop/default.png");

  Texture2D keyboard_texture_extra =
      LoadTexture("./resources/ui/desktop/extra.png");

  const u8 KEYBOARD_TEXTURE_COMPONENTS = 4u;

  // A, D, K, Q
  const Vector2 keyboard_texture_source[] = {
      (Vector2){
          .x = 0.0f,
          .y = 32.0f,
      },
      (Vector2){
          .x = 48.0f,
          .y = 32.0f,
      },
      (Vector2){
          .x = 32.0f,
          .y = 48.0f,
      },
      (Vector2){
          .x = 0.0f,
          .y = 64.0f,
      },
  };

  Texture2D xbox_texture = LoadTexture("./resources/ui/xbox.png");

  const u8 XBOX_TEXTURE_COMPONENTS = 4u;

  // X, A, Y, B
  const Vector2 xbox_texture_source[] = {
      (Vector2){
          .x = 16.0f,
          .y = 32.0f,
      },
      (Vector2){
          .x = 16.0f,
          .y = 48.0f,
      },
      (Vector2){
          .x = 16.0f,
          .y = 64.0f,
      },
      (Vector2){
          .x = 16.0f,
          .y = 80.0f,
      },
  };

  Sprite sprite = (Sprite){
      .texture = LoadTexture("./resources/sprites/blue.png"),
      .shadow = LoadTexture("./resources/sprites/shadow.png"),
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
      .hasShadow = true,
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

  Camera2D camera = {
      .target =
          (Vector2){
              .x = sprite.box.x,
              .y = sprite.box.y,
          },
      .offset =
          (Vector2){
              .x = GetScreenWidth() / 2.0f,
              .y = GetScreenHeight() / 2.0f,
          },
      .zoom = 1.0f,
  };

  while (!WindowShouldClose()) {
    UpdateMusicStream(theme);

    if (!IsMusicStreamPlaying(theme)) {
      PlayMusicStream(theme);
    }

    if (IsCursorOnScreen()) {
      HideCursor();
    }

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

    if (IsKeyPressed(KEY_K) ||
        IsGamepadButtonPressed(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) {
      animation[SPRITE_STATE_ATTACKING].currentFrame =
          animation[SPRITE_STATE_ATTACKING].firstFrame;

      sprite.state = SPRITE_STATE_ATTACKING;
    }

    sprite.animation = &animation[sprite.state];

    camera.target = (Vector2){
        .x = sprite.box.x,
        .y = sprite.box.y,
    };

    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode2D(camera);

    UpdateSprite(&sprite, GetTime());

    DrawSprite(&sprite);

    EndMode2D();

    if (!IsGamepadAvailable(gamepad)) {
      f64 x = 10.0f;

      for (u8 index = 0u; index < KEYBOARD_TEXTURE_COMPONENTS; ++index) {
        DrawTexturePro(keyboard_texture,
                       (Rectangle){
                           .x = keyboard_texture_source[index].x,
                           .y = keyboard_texture_source[index].y,
                           .width = 16.0f,
                           .height = 16.0f,
                       },
                       (Rectangle){
                           .x = x,
                           .y = GetScreenHeight() - 32.0f - 10.0f,
                           .width = 32.0f,
                           .height = 32.0f,
                       },
                       (Vector2){
                           .x = 0.0f,
                           .y = 0.0f,
                       },
                       0, WHITE);

        x += 32.0f;
      }

      DrawTexturePro(keyboard_texture_extra,
                     (Rectangle){
                         .x = 0.0f,
                         .y = 16.0f,
                         .width = 32.0f,
                         .height = 16.0f,
                     },
                     (Rectangle){
                         .x = x,
                         .y = GetScreenHeight() - 32.0f - 10.0f,
                         .width = 64.0f,
                         .height = 32.0f,
                     },
                     (Vector2){
                         .x = 0.0f,
                         .y = 0.0f,
                     },
                     0, WHITE);
    } else {
      f64 x = 10.0f;

      for (u8 index = 0u; index < XBOX_TEXTURE_COMPONENTS; ++index) {
        DrawTexturePro(xbox_texture,
                       (Rectangle){
                           .x = xbox_texture_source[index].x,
                           .y = xbox_texture_source[index].y,
                           .width = 16.0f,
                           .height = 16.0f,
                       },
                       (Rectangle){
                           .x = x,
                           .y = GetScreenHeight() - 32.0f - 10.0f,
                           .width = 32.0f,
                           .height = 32.0f,
                       },
                       (Vector2){
                           .x = 0.0f,
                           .y = 0.0f,
                       },
                       0, WHITE);

        x += 32.0f;
      }
    }

    EndDrawing();
  }

  UnloadTexture(xbox_texture);

  UnloadTexture(keyboard_texture);

  UnloadTexture(keyboard_texture_extra);

  UnloadTexture(sprite.shadow);

  UnloadTexture(sprite.texture);

  UnloadMusicStream(theme);

  CloseAudioDevice();

  CloseWindow();

  return 0;
}
