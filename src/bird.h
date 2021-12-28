#pragma once

struct Bird {

  int x{20};
  int y;
  int radius{10};
  int velocity{0};

  void update(int acc) {
    velocity += acc;
    y += velocity;
  }

  bool offscreen(int screen_height) const {

    return y < 0 || (y + radius > screen_height);
  }

  void up(int force) { velocity = force; }

  static void draw(const Bird &bird, Canvas &canvas) {
    canvas.fill(80, 80, 80);

    canvas.rect(bird.x, bird.y, bird.radius, bird.radius);
  }
};