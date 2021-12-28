#pragma once

#include <stdexcept>

///
/// \brief The Canvas struct is minimale interface to a canvas
/// allow to separate canvas usage to canvas implementation
/// not copiable
///
struct Canvas {

  Canvas() = default;

  Canvas(const Canvas &other) = delete;
  Canvas(Canvas &&other) = delete;
  Canvas &operator=(const Canvas &other) = delete;

  virtual ~Canvas() = default;

  virtual void setSize(int width, int height) = 0;
  virtual void setFramerate(int framerate) = 0;

  virtual void setBackground(int r, int g, int b) = 0;

  virtual void stroke(int r, int g, int b) = 0;
  virtual void noStroke() = 0;
  virtual void fill(int r, int g, int b) = 0;
  virtual void noFill() = 0;

  virtual void line(int x1, int y1, int x2, int y2) = 0;
  virtual void rect(int x1, int y1, int w, int h) = 0;

  virtual void text(std::string str, int x, int y) = 0;

  virtual int width() const = 0;
  virtual int height() const = 0;

  virtual void noLoop() = 0;

  virtual int mouseX() const = 0;
  virtual int mouseY() const = 0;

  virtual bool isMouseLeft() const = 0;
  virtual bool isMouseRight() const = 0;

protected:
  virtual bool isDrawing() const = 0;
  void ThrowIfNotDrawing() {
    if (!isDrawing()) {
      throw std::runtime_error("try to draw when not in draw state");
    }
  }
};

///
/// \brief The Application class is an interface to a drawing application
///
class IApplication {

public:
  virtual void setup(Canvas &canvas) {}

  virtual void draw(Canvas &canvas) = 0;

  virtual void mousePressed(Canvas &canvas) = 0;
};

#include <functional>

struct Application : IApplication {

  void Register(std::function<void(Canvas &)> setup_p,
                std::function<void(Canvas &)> draw_p) {
    setup_cb = setup_p;
    draw_cb = draw_p;
  }

  void RegisterMousePressed(std::function<void(Canvas &)> mp) {
    mousePressed_cb = mp;
  }

private:
  void setup(Canvas &canvas) override {
    if (setup_cb)
      setup_cb(canvas);
  }

  void draw(Canvas &canvas) override {
    if (draw_cb)
      draw_cb(canvas);
  }

  void mousePressed(Canvas &canvas) {
    if (mousePressed_cb)
      mousePressed_cb(canvas);
  }

  std::function<void(Canvas &)> setup_cb;
  std::function<void(Canvas &)> draw_cb;
  std::function<void(Canvas &)> mousePressed_cb;
};

// optional methods
void mousePressed(Canvas &) __attribute__((weak));

#define P5_BACKEND_QT()                                                        \
                                                                               \
  int main(int argc, char *argv[]) {                                           \
    QApplication a(argc, argv);                                                \
    Application p5_app;                                                        \
    p5_app.Register(setup, draw);                                              \
    p5_app.RegisterMousePressed(mousePressed);                                 \
    QtCanvas canvas(&p5_app);                                                  \
    canvas.show();                                                             \
    canvas.setFramerate(30);                                                   \
    return a.exec();                                                           \
  }
