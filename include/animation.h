#pragma once

#include <raylib.h>

struct Sfx final {
  Sound sound;
  float volume;
};

struct Animation final {
  double timer{0.0f};
  double frameTime;
  unsigned firstFrame;
  unsigned lastFrame;
  unsigned currentFrame;
  bool loop{false};
  bool ended{false};
  bool restart{false};

  Animation(double, unsigned, unsigned, bool, bool);

  void draw(void) const;

  void update(double);
};
