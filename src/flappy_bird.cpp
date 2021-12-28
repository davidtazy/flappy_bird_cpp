
#include "p5/application.h"
#include "p5/grid.h"

#include "bird.h"
#include "pipe.h"
#include <algorithm>
#include <list>
const int Velocity = 5;
const int PipeCreation = 75;
const int PipeWidth = 50;
const int G = 1;
const int BirdAcc = -12;

std::list<Pipe> pipes;
Bird bird;
int pipe_creator_counter = 0;

void setup(Canvas &canvas) {

  pipes.emplace_back(canvas.width(), canvas.height(), PipeWidth);
}

void draw(Canvas &canvas) {

  // update positions
  bird.update(G);
  Bird::draw(bird, canvas);

  for (auto &pipe : pipes) {
    pipe.update(Velocity);
    Pipe::draw(pipe, canvas);
  }

  // check offscreen
  if (bird.offscreen(canvas.height())) {
    std::cout << "offscreen" << std::endl;
    canvas.noLoop();
  }

  // check collision
  auto pipe = pipes.front();
  if ((!(bird.x > pipe.x + pipe.width || bird.x + bird.radius < pipe.x)) &&
      (!(bird.y > pipe.top && bird.y + bird.radius < pipe.top + pipe.gate))) {
    std::cout << "collision" << std::endl;
    canvas.noLoop();
  } else {
    std::cout << "free" << std::endl;
  }

  // remove offscreen pipes
  pipes.erase(std::remove_if(pipes.begin(), pipes.end(),
                             [](const Pipe &pipe) { return pipe.offscreen(); }),
              pipes.end());

  // create new pipe periodically
  pipe_creator_counter++;
  if (pipe_creator_counter > PipeCreation) {
    pipe_creator_counter = 0;
    pipes.emplace_back(canvas.width(), canvas.height(), PipeWidth);
  }
}

void mousePressed(Canvas &canvas) { bird.up(BirdAcc); }

/// magic happens here
#include "p5/qtcanvas.h"
P5_BACKEND_QT()
