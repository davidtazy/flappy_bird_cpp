
#include "p5/application.h"
#include "p5/grid.h"

#include "bird.h"
#include "pipe.h"
#include <algorithm>
#include <list>
const int Population = 300;
const int Velocity = 5;
const int PipeCreation = 75;
const int PipeWidth = 50;
const int BirdAcc = -12;
const int bird_pos = 20;
int cycle = 10;

std::list<Pipe> pipes;
std::list<Bird> birds;
std::list<Bird> failed_birds;
int pipe_creator_counter = 0;
int generation_count = 0;

Bird best_bird;

std::list<Bird> nextGeneration(Canvas &canvas);

void setup(Canvas &canvas) {

  for (int i = 0; i < Population; i++) {
    birds.emplace_back(bird_pos, canvas.height() / 2);
  }

  pipes.emplace_back(canvas.width(), canvas.height(), PipeWidth);
}

const Pipe &closestPipe(std::list<Pipe> &pipes) {

  auto it = std::find_if(pipes.begin(), pipes.end(), [](const Pipe &pipe) {
    return pipe.x + pipe.width > bird_pos;
  });

  it->closest = true;

  return *it;
}
#include <chrono>
void draw(Canvas &canvas) {

  auto start = std::chrono::steady_clock::now();

  for (int c = 0; c < cycle; c++) {
    // update positions
    for (auto &pipe : pipes) {
      pipe.update(Velocity);
    }
    auto &closest_pipe = closestPipe(pipes);

    int cpt = 0;
    for (auto &bird : birds) {
      cpt += bird.think(closest_pipe, canvas.width(), canvas.height());
      bird.update();
    }
    // std::cout << "do up= " << cpt << "/" << birds.size() << std::endl;

    auto failed_it = std::remove_if(
        birds.begin(), birds.end(), [&canvas, &closest_pipe](Bird &bird) {
          // check offscreen
          if (bird.offscreen(canvas.height())) {
            return true;
          }
          // check collision
          if ((!(bird.x > closest_pipe.x + closest_pipe.width ||
                 bird.x + bird.radius < closest_pipe.x)) &&
              (!(bird.y > closest_pipe.top &&
                 bird.y + bird.radius <
                     closest_pipe.top + closest_pipe.gate))) {
            return true;
          }
          return false;
        });

    failed_birds.splice(failed_birds.begin(), birds, failed_it, birds.end());

    // std::cout << "failed birds = " << failed_birds.size() << std::endl;

    // remove offscreen pipes
    pipes.erase(
        std::remove_if(pipes.begin(), pipes.end(),
                       [](const Pipe &pipe) { return pipe.offscreen(); }),
        pipes.end());

    if (birds.empty()) {
      birds = nextGeneration(canvas);
      pipe_creator_counter = PipeCreation + 1;
      pipes.clear();
    }

    // create new pipe periodically
    pipe_creator_counter++;
    if (pipe_creator_counter > PipeCreation) {
      pipe_creator_counter = 0;
      pipes.emplace_back(canvas.width(), canvas.height(), PipeWidth);
    }
  }

  // drawing stuff
  for (const auto &pipe : pipes)
    Pipe::draw(pipe, canvas);
  for (auto &bird : birds) {
    Bird::draw(bird, canvas);
  }
  auto end = std::chrono::steady_clock::now();
  /*
    std::cout << "Elapsed time in milliseconds: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                       start)
                     .count()
              << " ms" << std::endl;*/
  std::cout << std::flush;
}

void calculateFitness(std::list<Bird> &b) {

  double sum =
      std::accumulate(b.begin(), b.end(), 0.0, [](double s, const Bird &bird) {
        return bird.score + s;
      });

  for (auto &bird : b) {
    bird.fitness = bird.score / sum;
  }
}

std::random_device RandomDevice;

Bird &pickOne(std::list<Bird> &b) {
  std::uniform_real_distribution gen{0.0, 1.0};
  auto r = gen(RandomDevice);

  auto it = b.begin();

  while (r > 0) {
    r = r - it->fitness;
    it++;
  }
  it--;
  return *it;
}

Bird reproduce(std::list<Bird> &b, const Canvas &canvas) {

  auto &parent = pickOne(b);

  Bird child(bird_pos, canvas.height() / 2);
  child.brain = parent.brain;
  child.brain.mutate(0.1);

  return child;
}

std::list<Bird> nextGeneration(Canvas &canvas) {
  std::list<Bird> ret;
  generation_count++;
  std::cout << " next generation " << generation_count << "\n";

  calculateFitness(failed_birds);

  // failed_birds.sort(
  //    [](const Bird &a, const Bird &b) { return a.fitness > b.fitness; });

  std::cout << "-----------------------------------\n";
  std::cout << "best  score " << failed_birds.front().score << " /fitness "
            << failed_birds.front().fitness << '\n';
  std::cout << "worst score " << failed_birds.back().score << " /fitness "
            << failed_birds.back().fitness << '\n';

  for (int i = 0; i < Population; i++) {
    ret.push_back(std::move(reproduce(failed_birds, canvas)));
  }

  failed_birds.clear();

  return ret;
}

void mousePressed(Canvas &canvas) {
  if (cycle == 1)
    cycle = 10;
  else
    cycle = 1;
  // birds.front().up();
}

/// magic happens here
#include "p5/qtcanvas.h"
P5_BACKEND_QT()
