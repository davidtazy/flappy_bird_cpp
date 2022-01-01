#pragma once
#include "neuralnetwork/nn.h"
#include "pipe.h"

struct Bird {

  int x{20};
  int y;
  int radius{10};
  int velocity{0};
  int gravity{1};
  int lift{-10};
  NeuralNetwork brain{5, 8, 2};
  int score{0};
  double fitness{0};

  Bird() = default;
  Bird(const Bird &) = default;
  Bird &operator=(const Bird &) = default;
  Bird(int x, int y) : x{x}, y{y} {}

  bool think(const Pipe &pipe, int width, int height) {

    std::vector<double> input(5);

    input[0] = y / static_cast<double>(height);
    input[1] = pipe.x / static_cast<double>(width);
    input[2] = pipe.top / static_cast<double>(height);
    input[3] = (pipe.top + pipe.gate) / static_cast<double>(height);
    input[4] = velocity / 10.0;

    auto output = brain.predict(input);

    bool do_up = output[0] > output[1];
    if (do_up) {
      up();
    }
    return do_up;
  }

  void update() {
    score++;
    velocity += gravity;
    y += velocity;
  }

  bool offscreen(int screen_height) const {

    return y < 0 || (y + radius > screen_height);
  }

  void up() { velocity = lift; }

  static void draw(const Bird &bird, Canvas &canvas) {
    canvas.fill(80, 80, 80);

    canvas.rect(bird.x, bird.y, bird.radius, bird.radius);
  }
};