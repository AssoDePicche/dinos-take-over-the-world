#include "dino.h"

void DrawRecArray(const Rectangle array[], const u8 length,
                  const Texture2D texture) {
  const f32 padding = 10.0f;

  const f32 scale_factor = 2.0f;

  f32 x = padding;

  const Vector2 origin = (Vector2){
      .x = 0.0f,
      .y = 0.0f,
  };

  for (u8 index = 0; index < length; ++index) {
    Rectangle source = array[index];

    const f32 width = scale_factor * source.width;

    const f32 height = scale_factor * source.height;

    DrawTexturePro(texture, source,
                   (Rectangle){
                       .x = x,
                       .y = GetScreenHeight() - height - padding,
                       .width = width,
                       .height = height,
                   },
                   origin, 0, WHITE);

    x += height;
  }
}

int main(void) {
  const u8 gamepad = 0;

  InitWindow(480, 360, "Dinos Take Over The World!");

  SetExitKey(KEY_Q);

  SetTargetFPS(60);

  const f64 DEFAULT_FRAME_TIME = 1.0f / 12.0f;

  InitAudioDevice();

  Music theme = LoadMusicStream("./resources/music/manlorette_party.mp3");

  SetMusicVolume(theme, 0.0f);

  PlayMusicStream(theme);

  Texture2D keyboard_texture = LoadTexture("./resources/ui/keyboard.png");

  // A, D, K, Q, SHIFT
  const Rectangle keyboard_texture_source[] = {
      (Rectangle){
          .x = 64.0f,
          .y = 64.0f,
          .width = 16.0f,
          .height = 16.0f,
      },
      (Rectangle){
          .x = 96.0f,
          .y = 64.0f,
          .width = 16.0f,
          .height = 16.0f,
      },
      (Rectangle){
          .x = 176.0f,
          .y = 64.0f,
          .width = 16.0f,
          .height = 16.0f,
      },
      (Rectangle){
          .x = 64.0f,
          .y = 48.0f,
          .width = 16.0f,
          .height = 16.0f,
      },
      (Rectangle){
          .x = 36.0f,
          .y = 80.0f,
          .width = 24.0f,
          .height = 16.0f,
      },
  };

  const u8 KEYBOARD_TEXTURE_COMPONENTS = ARRAY_LENGTH(keyboard_texture_source);

  Texture2D xbox_texture = LoadTexture("./resources/ui/xbox.png");

  // X, A, Y, B
  const Rectangle xbox_texture_source[] = {
      (Rectangle){
          .x = 16.0f,
          .y = 32.0f,
          .width = 16.0f,
          .height = 16.0f,
      },
      (Rectangle){
          .x = 16.0f,
          .y = 48.0f,
          .width = 16.0f,
          .height = 16.0f,
      },
      (Rectangle){
          .x = 16.0f,
          .y = 64.0f,
          .width = 16.0f,
          .height = 16.0f,
      },
      (Rectangle){
          .x = 16.0f,
          .y = 80.0f,
          .width = 16.0f,
          .height = 16.0f,
      },
  };

  const u8 XBOX_TEXTURE_COMPONENTS = ARRAY_LENGTH(xbox_texture_source);

  Texture2D lifebar_texture =
      LoadTexture("./resources/sprites/lifebar_32x32.png");

  Sprite sprite = (Sprite){
      .texture = LoadTexture("./resources/sprites/blue.png"),
      .shadow = LoadTexture("./resources/sprites/shadow.png"),
      .box = (Rectangle){.x = 0,
                         .y = GetScreenHeight() - 48.0f,
                         .width = 48.0f,
                         .height = 48.0f},
      .state = SPRITE_STATE_IDLE,
      .textureFrames = 24u,
      .lifes = 3u,
      .walk_speed = 5.0f,
      .run_speed = 8.0f,
      .facingRight = true,
      .isRunning = false,
      .hasShadow = true,
  };

  // IDLE, WALK, RUN, ATTACK, HURT
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
          .restart = true,
      },
      (Animation){
          .timer = 0.0f,
          .frameTime = DEFAULT_FRAME_TIME,
          .firstFrame = 14u,
          .lastFrame = 17u,
          .currentFrame = 14u,
          .loop = false,
          .restart = true,
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

    sprite.state =
        IsKeyPressed(KEY_K) ||
                IsGamepadButtonPressed(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)
            ? sprite.state = SPRITE_STATE_ATTACKING
            : sprite.state;

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

    for (u8 i = 0u; i < sprite.lifes; ++i) {
      DrawTexturePro(lifebar_texture,
                     (Rectangle){
                         .x = 0.0f,
                         .y = 0.0f,
                         .width = 32.0f,
                         .height = 32.0f,
                     },
                     (Rectangle){
                         .x = 10.0f + i * 32.0f,
                         .y = 10.0f,
                         .width = 32.0f,
                         .height = 32.0f,
                     },
                     (Vector2){
                         .x = 0.0f,
                         .y = 0.0f,
                     },
                     0, WHITE);
    }

    if (!IsGamepadAvailable(gamepad)) {
      DrawRecArray(keyboard_texture_source, KEYBOARD_TEXTURE_COMPONENTS,
                   keyboard_texture);
    } else {
      DrawRecArray(xbox_texture_source, XBOX_TEXTURE_COMPONENTS, xbox_texture);
    }

    EndDrawing();
  }

  UnloadTexture(xbox_texture);

  UnloadTexture(keyboard_texture);

  UnloadTexture(sprite.shadow);

  UnloadTexture(sprite.texture);

  UnloadTexture(lifebar_texture);

  UnloadMusicStream(theme);

  CloseAudioDevice();

  CloseWindow();

  return 0;
}
