#pragma once
#include <array>
#include <functional>
#include <iostream>
#include <tuple>
#include <vector>

struct Size {
  int row;
  int column;
};

struct Position {
  int column;
  int row;

  bool valid(Size size) const {
    return (column >= 0 && row >= 0 && column < size.column && row < size.row);
  }

  bool operator==(const Position &other_p) const {
    return column == other_p.column && row == other_p.row;
  }

  bool operator!=(const Position &other_p) const {
    return column != other_p.column || row != other_p.row;
  }

  bool operator<(const Position &other_p) const {

    return std::make_tuple(column, row) <
           std::make_tuple(other_p.column, other_p.row);
  }

  std::vector<Position> Neighbors() const {

    std::vector<Position> ret(8);

    for (int col = -1; col <= 1; col++) {
      for (int r = -1; r <= 1; r++) {
        Position pos{column + col, row + r};
        if (pos != *this) {
          ret.push_back(pos);
        }
      }
    }

    return ret;
  }
};

template <typename T> class Grid {
  Size m_size;
  std::vector<T> m_cells;

  std::vector<T> make_grid() const {
    std::vector<T> grid;
    for (int i = 0; i < m_size.column; i++) {
      for (int j = 0; j < m_size.row; j++) {
        Position pos;
        pos.column = i;
        pos.row = j;
        grid.emplace_back(pos);
      }
    }
    return grid;
  }

public:
  Grid(Size size) : m_size(size), m_cells(make_grid()) {}

  void forEach(std::function<void(T &)> cb) {
    for (auto &cell : m_cells) {
      cb(cell);
    }
  }

  void forEachNeighbor(const T &cell_p, std::function<void(T &)> cb) {

    auto pos = cell_p.position;

    for (auto neighbor : pos.Neighbors()) {
      if (neighbor.valid(m_size)) {
        cb(at(neighbor));
      }
    }
  }

  T &at(Position pos_p) {
    const int id = pos_p.column * (m_size.row) + (pos_p.row);
    return m_cells.at(id);
  }
};

int random(int max);
