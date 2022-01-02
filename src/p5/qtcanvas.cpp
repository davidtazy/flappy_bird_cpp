#include "qtcanvas.h"

QtCanvas::QtCanvas(IApplication *app) : m_app(app) {
  timer.setTimerType(Qt::PreciseTimer);

  setMouseTracking(true);
  setBackgroundRole(QPalette::Base);
  setAutoFillBackground(false);
  setAttribute(Qt::WA_OpaquePaintEvent);

  app->setup(*this);

  // connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
  connect(&timer, &QTimer::timeout, this, [this]() { this->update(); });
}
