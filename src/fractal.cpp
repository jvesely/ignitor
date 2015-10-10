#include "fractal.h"
#include "myRandom.h"
#include <QFile>
#include <QTextStream>
#include <QPainter>

fractal::fractal(const int width, const int height)
    : _width(width), _height(height) {
  picture = 0;
  engine = 0;
  _active = true;
  _running = false;
  // setBackgroundRole(QPalette::Base);
  setMinimumSize(400, 300);
  setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  // setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  setFuncCount(randomTo(10) + 1);
  genFunctions();
  setIterations(5000000);
  rendered = false;
}
fractal::~fractal() {
  stop();
  delFunc();
  delete picture;
}
void fractal::paintEvent(QPaintEvent *event) {
  if (picture) {
    QPainter painter(this);
    int width_ = picture->width();
    int height_ = picture->height();
    if (width() < width_ || height() < height_) {
      width_ = width();
      height_ = height();
    }
    QPixmap current = QPixmap::fromImage(*picture)
                          .scaled(width_, height_, Qt::KeepAspectRatio);
    int x = (width() - current.width()) / 2;
    int y = (height() - current.height()) / 2;
    painter.drawPixmap(QPoint(x, y), current);
  }
}
void fractal::delFunc() {
  _functions.clear();
  _funcCount = 0;
}
void fractal::addFunc(const Function &func) {
  _functions.append(func);
  ++_funcCount;
}
void fractal::setResolution(const int fWidth, const int fHeight) {
  _height = fHeight;
  _width = fWidth;
  delete picture;
  picture = 0;
}
void fractal::write(const QString &text) {
  // clear();
  // setText(text);
}
bool fractal::savePict(const QString &fileName) const {
  return picture->save(fileName);
}
bool fractal::loadXML(const QString &fileName) {
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return false;
  QTextStream stream(&file);
  QDomDocument frac("flamefractal");
  QString content(stream.readAll());
  if (!frac.setContent(content, false))
    return false;
  return fromXML(frac);
  // qDebug()<<"Content set.";
}
bool fractal::fromXML(const QDomDocument &frac) {
  QDomElement me = frac.documentElement();
  _width = me.attribute("width", "0").toInt();
  _height = me.attribute("height", "0").toInt();
  _iterations = me.attribute("iterations", "0").toInt();
  if (!_width || !_height || !_iterations)
    return false;
  QDomElement func = me.firstChildElement("functions");
  delFunc();
  _funcCount = func.attribute("count", 0).toInt();
  if (!_funcCount)
    return false;

  for (QDomElement i = func.firstChildElement(); !i.isNull();
       i = i.nextSiblingElement()) {
    Function f(i);
    _functions.append(f);
  }
  return true;
}
void fractal::saveXML(const QString &fileName) const {
  QDomDocument frac("flamefractal");
  frac.appendChild(toXML(frac));
  QFile file(fileName);
  file.open(QIODevice::WriteOnly | QIODevice::Text);
  QTextStream stream(&file);
  stream << frac.toString(2);

  file.close();
}
QDomElement fractal::toXML(QDomDocument &doc) const {
  QDomElement me = doc.createElement("fractal");
  me.setAttribute("width", _width);
  me.setAttribute("height", _height);
  me.setAttribute("iterations", _iterations);
  QDomElement funcs = doc.createElement("functions");
  me.appendChild(funcs);
  for (int i = 0; i < _funcCount; ++i) {
    QDomElement function = _functions[i].toXML(doc);
    funcs.appendChild(function);
  }
  funcs.setAttribute("count", _funcCount);
  QDomElement pal = doc.createElement("palette");
  return me;
}
void fractal::setFuncCount(int count) { _funcCount = count; }
void fractal::setIterations(int num) { _iterations = num; }
bool fractal::render() {
  stop();
  engine = new engineThread(_width, _height, _iterations, _functions);
  connect(engine, SIGNAL(finished()), this, SLOT(stop()));
  connect(engine, SIGNAL(nextIteration(int, int, int)), this,
          SLOT(pointIterated(int, int, int)));
  connect(engine, SIGNAL(destroyed()), this, SLOT(engineDied()));
  connect(engine, SIGNAL(instantPreview()), this, SLOT(paint()));
  _running = true;
  engine->start();
  emit changed();
  return true;
}
void fractal::engineDied() {
  _running = false;
  emit changed();
}
void fractal::pointIterated(int min, int n, int max) {
  if (_active) {
    emit setCurrent(n);
    emit setMax(max);
  }
}
void fractal::paint() {
  if (!engine)
    return;

  int width = engine->getWidth();
  int height = engine->getHeight();
  delete picture;
  picture = new QImage(width, height, QImage::Format_RGB32);

  int max = width * height;
  for (int i = 0; i < max; ++i) {
    QColor c = engine->get(i);
    int x, y;
    x = i % _width;
    y = i / _width;
    picture->setPixel(x, y, c.rgb());
  }
  rendered = true;
  emit changed();
  update();
}
void fractal::stop() {
  delete engine;
  engine = 0;
}
void fractal::genFunctions() {
  for (int i = 0; i < _funcCount; ++i) {
    const Function newFunction;
    _functions.append(newFunction);
  }
}
bool fractal::getRendered() { return rendered; }
bool fractal::isRunning() { return _running; }
int fractal::getIterations() { return _iterations; }
int fractal::getWidth() { return _width; }
int fractal::getHeight() { return _height; }
void fractal::sendStatus() { engine->sendStatus(); }
void fractal::activated(fractal *active) {
  if (_active = active == this) {
    emit setMin(0);
    if (!engine) {
      emit setMax(0);
      emit setCurrent(0);
    } else
      emit setMax(
          _iterations); // does not have to match still it is highly probable
  }
}

engineThread::engineThread(const int width, const int height,
                           const int iterations,
                           const QVector<Function> &functions) {
  // copy params, in case they change during rendering;
  _width = width;
  _height = height;
  _iterations = iterations;
  _functions = functions;
  _funcCount = functions.count();
  red = 0;
  green = 0;
  blue = 0;
  hits = 0;
  _abort = false;
}
engineThread::~engineThread() {
  _abort = true; // stop rendering cycle
  emit nextIteration(0, 0, 0);
  blockSignals(true); // none shall hear me cry, as I die
  wait();
  delete[] red; // get rid of stuff
  delete[] green;
  delete[] blue;
  delete[] hits;
}
void engineThread::run() {
  // qDebug()<<"Rendering...";

  int max = _width * _height;
  red = new double[max]; // red colour count array
  green = new double[max]; // green colour count array
  blue = new double[max]; // blue colour count array
  hits = new int[max]; // hit count array

  // clean arrays
  for (int i = 0; i < max; ++i) {
    hits[i] = 0;
    red[i] = 0;
    green[i] = 0;
    blue[i] = 0;
  }
  // strting positions
  double x = biunitRandom();
  double y = biunitRandom();
  double r = unitRandom();
  double g = unitRandom();
  double b = unitRandom();

  int interval = _iterations / 100; // suitable update interval

  for (_i = 0; _i < 20; ++_i) // ignore first 20 iterations
    _functions[randomTo(_funcCount)](x, y, r, g, b);

  for (_i = 20; _i < _iterations; ++_i) { // main algorythm loop
    _functions[randomTo(_funcCount)](x, y, r, g, b); // iterate
    put(x, y, r, g, b); // plot

    if (_i % interval == 0)
      sendStatus(); // change percentage
    if (_i % (interval * 10) == 0)
      emit instantPreview(); // showpicture

    if (_abort)
      return;
  }
  sendStatus();
  emit instantPreview();
}
void engineThread::sendStatus() { emit nextIteration(0, _i, _iterations); }

void engineThread::put(const double x, const double y, const double r,
                       const double g, const double b) {
  int pos =
      (int)((x + 1) * (_width / 2)) + (int)((y + 1) * (_height / 2)) * _width;
  if (pos < 0 || pos >= (_width * _height))
    return;
  red[pos] += r;
  green[pos] += g;
  blue[pos] += b;
  hits[pos] += 1;
}
QColor engineThread::get(const int pos) const {
  if (!red || !green || !blue || !hits || hits[pos] == 0)
    return QColor(0, 0, 0);
  return QColor((red[pos] / hits[pos]) * 255, (green[pos] / hits[pos]) * 255,
                (blue[pos] / hits[pos]) * 255);
}
int engineThread::getWidth() const { return _width; }
int engineThread::getHeight() const { return _height; }
