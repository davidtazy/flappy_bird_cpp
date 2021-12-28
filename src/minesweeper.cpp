#include <QApplication>

#include "p5/application.h"
#include "p5/grid.h"

#include <set>

const Size grid_size{10, 10};
const int MineCount = 10;

struct Cell {
  Position position;
  bool revealed{false};
  bool mine{false};
  int mine_neighbor_count{0};
  bool marker{false};

  explicit Cell(Position position_p) : position(position_p) {}

  void mark() { marker = !marker; }

  void draw(Canvas &canvas) const;
};

Grid<Cell> grid(grid_size);
bool game_over = false;
int go_anim = 0;
bool win = false;
int revealed = 0;

bool isFinished() { return win || game_over; }

void setup(Canvas &canvas) {

  // fill the grid with mines
  std::set<Position> mines;
  while (mines.size() < MineCount) {
    int col = random(grid_size.column);
    int row = random(grid_size.row);
    Position pos{col, row};

    if (auto [it, inserted] = mines.insert(pos); inserted) {
      grid.at(pos).mine = true;
    }
  }

  // fill other cells with number of neighbors mines
  grid.forEach([](Cell &cell) {
    if (cell.mine == false) {
      int count = 0;
      grid.forEachNeighbor(cell, [&count](Cell &neighbor) {
        if (neighbor.mine) {
          count++;
        }
      });
      cell.mine_neighbor_count = count;
    }
  });
}

void draw(Canvas &canvas) {

  // Render all cells
  grid.forEach([&canvas](const Cell &cell) { cell.draw(canvas); });

  if (isFinished()) {
    if (game_over) {
      canvas.text("GAME OVER", go_anim, go_anim);
    }
    if (win) {
      canvas.text("WIN", go_anim, go_anim);
    }
    go_anim++;
    if (go_anim > canvas.width())
      go_anim = 0;
  }
}

void reveal(Cell &cell) {

  if (cell.revealed == false) {
    cell.revealed = true;
    revealed++;
    if (cell.mine_neighbor_count == 0) {
      grid.forEachNeighbor(cell, [](Cell &cell) { reveal(cell); });
    }
  }
}

Cell &cellFromWindow(Canvas &canvas, int x, int y) {
  int col = grid_size.column * x / canvas.width();
  int row = grid_size.row * y / canvas.height();
  Position pos{col, row};
  return grid.at(pos);
}

void mousePressed(Canvas &canvas) {

  // do not accept any events if game is finished
  if (isFinished()) {
    return;
  }

  auto &cell = cellFromWindow(canvas, canvas.mouseX(), canvas.mouseY());

  if (canvas.isMouseLeft()) {

    if (cell.mine == true) {
      // game over
      grid.forEach([&canvas](Cell &cell) { cell.revealed = true; });
      game_over = true;
    } else {
      reveal(cell);
      if (grid_size.column * grid_size.row == revealed + MineCount) {
        win = true;
      }
    }
  }

  if (canvas.isMouseRight() && cell.revealed == false) {
    cell.mark();
  }
}

void Cell::draw(Canvas &canvas) const {

  int x_min = canvas.width() * position.column / grid_size.column;
  int y_min = canvas.height() * position.row / grid_size.row;
  int width = canvas.width() / grid_size.column;
  int height = canvas.height() / grid_size.row;

  canvas.stroke(80, 80, 80);
  canvas.fill(255, 255, 255);
  if (revealed == false) {
    if (!marker)
      canvas.fill(200, 200, 200);
    else
      canvas.fill(255, 0, 0);
    canvas.rect(x_min, y_min, width, height);

  } else if (mine == true) {
    canvas.line(x_min, y_min, x_min + width, y_min + height);
    canvas.line(x_min + width, y_min, x_min, y_min + height);
  } else if (mine_neighbor_count > 0) {
    canvas.text(std::to_string(mine_neighbor_count), x_min + width / 2,
                y_min + height / 2);
  }
  canvas.noFill();
  canvas.rect(x_min, y_min, width, height);
}

/// magic happens here
#include "p5/qtcanvas.h"
P5_BACKEND_QT()
