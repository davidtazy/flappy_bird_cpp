#pragma once

#include "p5/application.h"
#include "p5/grid.h"

struct Pipe {

  int x{0};
  int width{20};
  int gate{100};
  int top{-1};
  bool closest = false;

  Pipe(int screen_width, int screen_height, int pipe_width) {
    x = screen_width;
    top = random(screen_height - gate);
    width = pipe_width;
  }

  void update(int velocity) { x -= velocity; }

  bool offscreen() const { return x + width < 0; }

  static void draw(const Pipe &pipe, Canvas &canvas) {

    canvas.fill((pipe.closest) ? 50 : 0, 0, 0);

    canvas.rect(pipe.x, 0, pipe.width, pipe.top);
    canvas.rect(pipe.x, pipe.top + pipe.gate, pipe.width, canvas.height());
  }
};