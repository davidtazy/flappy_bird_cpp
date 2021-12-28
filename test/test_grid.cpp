#include <QObject>
#include <QTest>

#include "p5/grid.h"

struct MinimalCell {
  Position position;
  explicit MinimalCell(Position position_p) : position(position_p) {}
};

class testGrid : public QObject {

  Q_OBJECT

private slots:

  void can_iterate_all_grid_data() {
    QTest::addColumn<int>("row");
    QTest::addColumn<int>("col");

    QTest::newRow("line") << 1 << 3;
    QTest::newRow("square") << 10 << 10;
    QTest::newRow("column") << 10 << 1;
    QTest::newRow("1 cell") << 1 << 1;
    return;
  }

  void can_iterate_all_grid() {

    QFETCH(int, row);
    QFETCH(int, col);
    Size size;
    size.row = row;
    size.column = col;

    Grid<MinimalCell> g(size);

    for (int i = 0; i < col; i++) {
      for (int j = 0; j < row; j++) {
        Position pos;
        pos.row = j;
        pos.column = i;
        auto &cell = g.at(pos);
        QVERIFY(cell.position == pos);
      }
    }
  }
};
QTEST_MAIN(testGrid)
#include "test_grid.moc"
/*
  auto &revealed_empty = grid.at({0, 0});
  revealed_empty.revealed = true;

  auto &revealed_mine = grid.at({1, 1});
  revealed_mine.revealed = true;
  revealed_mine.mine = true;

  auto &revealed_2_neighbors = grid.at({2, 2});
  revealed_2_neighbors.revealed = true;
  revealed_2_neighbors.mine_neighbor_count = 2;
  */