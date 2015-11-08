#include "myRandom.h"
#include "function.h"
#define _USE_MATH_DEFINES
#include <cmath>

Function::Function() {
  for (int i = 0; i < 6; ++i) {
    _coefs[i] = biunitRandom();
  }
  _red = unitRandom();
  _green = unitRandom();
  _blue = unitRandom();
  _nonLinear = randomTo(14);
}
Function::Function(const QDomElement &func) {
  double colour = func.attribute("colour", "2").toDouble();
  if (colour == 2) {
    _red = func.attribute("red", "2").toDouble();
    _green = func.attribute("green", "2").toDouble();
    _blue = func.attribute("blue", "2").toDouble();
  } else {
    _red = colour;
    _green = colour;
    _blue = colour;
  }

  _nonLinear = func.attribute("nonlinear", "-1").toInt();

  _coefs[0] = func.attribute("xx", "2").toDouble();
  _coefs[1] = func.attribute("xy", "2").toDouble();
  _coefs[2] = func.attribute("xa", "2").toDouble();
  _coefs[3] = func.attribute("yx", "2").toDouble();
  _coefs[4] = func.attribute("yy", "2").toDouble();
  _coefs[5] = func.attribute("ya", "2").toDouble();
}
Function::Function(const double coefs[5], const double red, const double green,
                   const double blue, const int nonLin) {
  for (int i = 0; i < 6; ++i) {
    _coefs[i] = coefs[i];
  }
  _red = red;
  _green = green;
  _blue = blue;
  _nonLinear = nonLin;
}
bool Function::nonLinear(double &x, double &y) {
  double phi = atan2(y, x);
  double r = sqrt(x * x + y * y);
  switch (_nonLinear) {
  case 0:
    break;
  case 1:
    x = sin(x);
    y = sin(y);
    break;
  case 2:
    x = x / pow(r, 2);
    y = y / pow(r, 2);
    break;
  case 3:
    x = r * cos(phi + r);
    y = r * sin(phi + r);
    break;
  case 4:
    x = r * cos(phi * 2);
    y = r * sin(phi * 2);
    break;
  case 5:
    x = phi / M_PI;
    y = r - 1;
    break;
  case 6:
    x = r * sin(phi + r);
    y = r * cos(phi - r);
    break;
  case 7:
    x = r * sin(phi * r);
    y = 0 - cos(phi * r);
    break;
  case 8:
    x = phi * sin(M_PI * r) / M_PI;
    y = phi * cos(M_PI * r) / M_PI;
    break;
  case 9:
    x = (cos(phi) + sin(r)) / r;
    y = (sin(phi) - cos(r)) / r;
    break;
  case 10: {
    x = sin(phi) / r;
    y = cos(phi) * r;
    break;
  }
  case 11: {
    x = sin(phi) * cos(r);
    y = cos(phi) * sin(r);
    break;
  }
  case 12: {
    x = r * pow(sin(phi + r), 3);
    y = r * pow(cos(phi - r), 3);
    break;
  }
  case 13: {
    double omega = randomTo(2) ? M_PI : 0;
    x = sqrt(r) * cos((phi / 2) + omega);
    y = sqrt(r) * sin((phi / 2) + omega);
    break;
  }
  case 14: {
    x = (2 * r / (r + 1)) * x;
    y = (2 * r / (r + 1)) * y;
    break;
  }
  case 15: {
    double nx = cos(M_PI * x) * cosh(y);
    double ny = (0 - sin(M_PI * x)) * sinh(y);
    x = nx;
    y = ny;
    break;
  }
  default:
    return false;
  }
  return true;
}
bool Function::operator()(double &x, double &y, double &r, double &g,
                          double &b) {
  double ox = x;
  double oy = y;
  x = (_coefs[0] * ox) + (_coefs[1] * oy) + _coefs[2];
  y = (_coefs[3] * ox) + (_coefs[4] * oy) + _coefs[5];
  r = (r + _red) / 2;
  g = (g + _green) / 2;
  b = (b + _blue) / 2;
  return nonLinear(x, y);
}
QDomElement Function::toXML(QDomDocument &doc) const {
  QDomElement me = doc.createElement("function");
  me.setAttribute("xx", _coefs[0]);
  me.setAttribute("xy", _coefs[1]);
  me.setAttribute("xa", _coefs[2]);
  me.setAttribute("yx", _coefs[3]);
  me.setAttribute("yy", _coefs[4]);
  me.setAttribute("ya", _coefs[5]);
  me.setAttribute("red", _red);
  me.setAttribute("green", _green);
  me.setAttribute("blue", _blue);
  me.setAttribute("nonlinear", _nonLinear);
  return me;
}
