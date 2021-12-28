#include "qtcanvas.h"

QtCanvas::QtCanvas(IApplication *app) : m_app(app) {
  timer.setTimerType(Qt::PreciseTimer);
  app->setup(*this);

  // connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
  connect(&timer, &QTimer::timeout, this, [this]() { this->update(); });

  setMouseTracking(true);
}
