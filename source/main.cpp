#include <raylib.h>

#include <vector>

#include "animation.h"
#include "serialization.h"
#include "sprite.h"

void DrawRecArray(const std::vector<Rectangle> array, const Texture2D texture) {
  const auto padding = 10.0f;

  const auto scale_factor = 2.0f;

  auto x = padding;

  const Vector2 origin(0.0f, 0.0f);

  for (const auto& source : array) {
    const auto width = scale_factor * source.width;

    const auto height = scale_factor * source.height;

    const Rectangle destination(x, GetScreenHeight() - height - padding, width,
                                height);

    DrawTexturePro(texture, source, destination, origin, 0, WHITE);

    x += height;
  }
}

auto main(void) -> int {
  const auto gamepad = 0;

  const auto DEFAULT_FRAME_TIME = 1.0f / 12.0f;

  auto rows = 8;

  auto columns = 8;

  auto map = load_map("./resources/map.txt");

  if (map.empty()) {
    return 1;
  }

  InitWindow(480, 360, "Dinos Take Over The World!");

  SetExitKey(KEY_Q);

  SetTargetFPS(60);

  InitAudioDevice();

  auto theme = LoadMusicStream("./resources/music/manlorette_party.mp3");

  SetMusicVolume(theme, 0.75f);

  PlayMusicStream(theme);

  auto map_texture = LoadTexture("./resources/sprites/palm_tree_32x32.png");

  const std::vector<Rectangle> map_texture_sources = {
      {0.0f, 0.0f, 0.0f, 0.0f},     {0.0f, 0.0f, 32.0f, 32.0f},
      {32.0f, 0.0f, 32.0f, 32.0f},  {64.0f, 0.0f, 32.0f, 32.0f},
      {0.0f, 32.0f, 32.0f, 32.0f},  {32.0f, 32.0f, 32.0f, 32.0f},
      {64.0f, 32.0f, 32.0f, 32.0f}, {0.0f, 64.0f, 32.0f, 32.0f},
      {32.0f, 64.0f, 32.0f, 32.0f}, {64.0f, 64.0f, 32.0f, 32.0f}};

  auto music_icon = LoadTexture("./resources/ui/icons/music.png");

  auto controller_icon = LoadTexture("./resources/ui/icons/controller.png");

  auto keyboard_icon = LoadTexture("./resources/ui/icons/keyboard.png");

  auto keyboard_texture = LoadTexture("./resources/ui/keyboard.png");

  // A, D, K, Q, SHIFT
  const std::vector<Rectangle> keyboard_texture_source{
      {64.0f, 64.0f, 16.0f, 16.0f},  {96.0f, 64.0f, 16.0f, 16.0f},
      {176.0f, 64.0f, 16.0f, 16.0f}, {64.0f, 48.0f, 16.0f, 16.0f},
      {36.0f, 80.0f, 24.0f, 16.0f},
  };

  auto xbox_texture = LoadTexture("./resources/ui/xbox.png");

  // X, A, Y, B
  const std::vector<Rectangle> xbox_texture_source{
      {16.0f, 32.0f, 16.0f, 16.0f},
      {16.0f, 48.0f, 16.0f, 16.0f},
      {16.0f, 64.0f, 16.0f, 16.0f},
      {16.0f, 80.0f, 16.0f, 16.0f}};

  auto lifebar_texture = LoadTexture("./resources/sprites/lifebar_32x32.png");

  Sprite sprite{
      .texture = LoadTexture("./resources/sprites/blue.png"),
      .shadow = LoadTexture("./resources/sprites/shadow.png"),
      .box = {GetScreenWidth() / 2.0f,
              (GetScreenHeight() - 48.0f) / 2.0f - 10.0f, 48.0f, 48.0f},
      .animation = nullptr,
      .walkSpeed = 5.0f,
      .runSpeed = 8.0f,
      .textureFrames = 24u,
      .lifes = 3u,
      .state = SPRITE_STATE_IDLE,
      .facingRight = true,
      .isRunning = false,
      .hasShadow = true,
  };

  // IDLE, WALK, RUN, ATTACK, HURT
  std::vector<std::shared_ptr<Animation>> animation{
      std::make_shared<Animation>(DEFAULT_FRAME_TIME, 0u, 4u, true, false),
      std::make_shared<Animation>(DEFAULT_FRAME_TIME, 5u, 10u, true, false),
      std::make_shared<Animation>(DEFAULT_FRAME_TIME, 18u, 24u, true, false),
      std::make_shared<Animation>(DEFAULT_FRAME_TIME, 10u, 13u, false, true),
      std::make_shared<Animation>(DEFAULT_FRAME_TIME, 14u, 17u, false, true)};

  Camera2D camera({GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f},
                  {sprite.box.x, sprite.box.y}, 0.0f, 1.0f);

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

    if (IsKeyPressed(KEY_K) ||
        IsGamepadButtonPressed(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) {
      sprite.state = SPRITE_STATE_ATTACKING;
    }

    sprite.animation = animation[sprite.state];

    camera.target = {
        sprite.box.x,
        sprite.box.y,
    };

    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode2D(camera);

    sprite.update(GetTime());

    auto x = 0.0f;

    auto y = (GetScreenHeight() - 8 * 32.0f) / 2.0f;

    for (auto i = 0; i < rows; ++i) {
      for (auto j = 0; j < columns; ++j) {
        const auto index = i * columns + j;

        const int tile = map[index];

        DrawTexturePro(map_texture, map_texture_sources[tile],
                       {x, y, 32.0f, 32.0f}, {0.0f, 0.0f}, 0, WHITE);

        x += 32.0f;
      }

      x = 0.0f;

      y += 32.0f;
    }

    sprite.draw();

    EndMode2D();

    for (auto i = 0u; i < sprite.lifes; ++i) {
      DrawTexturePro(lifebar_texture, {0.0f, 0.0f, 32.0f, 32.0f},
                     {10.0f + i * 32.0f, 10.0f, 32.0f, 32.0f}, {0.0f, 0.0f}, 0,
                     WHITE);
    }

    DrawTexturePro(
        music_icon, {0.0f, 0.0f, 24.0f, 24.0f},
        {GetScreenWidth() - 2.0f * 32.0f - 2.0f * 10.0f, 10.0f, 32.0f, 32.0f},
        {0.0f, 0.0f}, 0, WHITE);

    if (!IsGamepadAvailable(gamepad)) {
      DrawTexturePro(keyboard_icon, {0.0f, 0.0f, 24.0f, 24.0f},
                     {GetScreenWidth() - 32.0f - 10.0f, 10.0f, 32.0f, 32.0f},
                     {0.0f, 0.0f}, 0, WHITE);

      DrawRecArray(keyboard_texture_source, keyboard_texture);
    } else {
      DrawTexturePro(controller_icon, {0.0f, 0.0f, 24.0f, 24.0f},
                     {GetScreenWidth() - 32.0f - 10.0f, 10.0f, 32.0f, 32.0f},
                     {0.0f, 0.0f}, 0, WHITE);

      DrawRecArray(xbox_texture_source, xbox_texture);
    }

    EndDrawing();
  }

  UnloadTexture(map_texture);

  UnloadTexture(xbox_texture);

  UnloadTexture(keyboard_texture);

  UnloadTexture(keyboard_icon);

  UnloadTexture(controller_icon);

  UnloadTexture(music_icon);

  UnloadTexture(sprite.shadow);

  UnloadTexture(sprite.texture);

  UnloadTexture(lifebar_texture);

  UnloadMusicStream(theme);

  CloseAudioDevice();

  CloseWindow();

  return 0;
}
