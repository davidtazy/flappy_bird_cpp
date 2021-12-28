#pragma once

#include "application.h"
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>
#include <QWidget>
///
/// \brief The QtCanvas class implements canvas with Qt
/// QPainter for Canvas drawing primitives
/// QTimer to refresh canvas
///
class QtCanvas : public QWidget, public Canvas {

  Q_OBJECT
  QPainter *m_painter{};
  QTimer timer;
  IApplication *m_app{};
  QPoint mouse;
  Qt::MouseButton mouseButton{Qt::NoButton};

  // Canvas interface
public:
  QtCanvas(IApplication *app);

  void setSize(int width, int height) override {
    QWidget::resize(width, height);
  }
  void setFramerate(int framerate) override {
    if (framerate > 0) {
      timer.setInterval(1000 / framerate);
      timer.start();
    } else {
      timer.stop();
    }
  }

  void setBackground(int r, int g, int b) override {
    ThrowIfNotDrawing();
    m_painter->fillRect(0, 0, QWidget::width(), QWidget::height(),
                        QColor(r, g, b));
  }

  void line(int x1, int y1, int x2, int y2) override {
    ThrowIfNotDrawing();
    m_painter->drawLine(x1, y1, x2, y2);
  }

  void stroke(int r, int g, int b) override {
    ThrowIfNotDrawing();
    QPen pen{QColor{r, g, b}};
    m_painter->setPen(pen);
  }

  void noStroke() override {
    ThrowIfNotDrawing();
    QPen pen{Qt::NoPen};
    m_painter->setPen(pen);
  }

  int width() const override { return QWidget::width(); }
  int height() const override { return QWidget::height(); }

  void fill(int r, int g, int b) override {
    ThrowIfNotDrawing();
    m_painter->setBrush(QBrush{QColor{r, g, b}});
  }
  void noFill() override {
    ThrowIfNotDrawing();
    m_painter->setBrush(QBrush{});
  }
  void rect(int x, int y, int w, int h) override {
    m_painter->fillRect(x, y, w, h, m_painter->brush());
    m_painter->drawRect(x, y, w, h);
  }

  void text(std::string str, int x, int y) override {
    m_painter->drawText(x, y, QString::fromStdString(str));
  }

  void noLoop() override { timer.stop(); }

  int mouseX() const override { return mouse.x(); }
  int mouseY() const override { return mouse.y(); }

  bool isMouseLeft() const override { return mouseButton == Qt::LeftButton; }
  bool isMouseRight() const override { return mouseButton == Qt::RightButton; }

protected:
  void paintEvent(QPaintEvent *) override {
    QPainter painter(this);
    m_painter = &painter;

    m_app->draw(*this);

    m_painter = nullptr;
  }
  void mousePressEvent(QMouseEvent *event) override {
    mouseButton = event->button();
    m_app->mousePressed(*this);
  }

  void mouseReleaseEvent(QMouseEvent *event) override {
    mouseButton = Qt::NoButton;
  }

  void mouseMoveEvent(QMouseEvent *event) override { mouse = event->pos(); }
  bool isDrawing() const override { return m_painter; }
};
