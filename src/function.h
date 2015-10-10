#ifndef _FUNCTION_H_
#define _FUNCTION_H_
#include <qdom.h>
class FunctionTab;
class Function {
  friend class FunctionTab;
  double _coefs[6]; // linear coeficients and function colour
  double _red;
  double _green;
  double _blue;
  int _nonLinear; // non linear function

  bool nonLinear(double &x, double &y);

public:
  Function();
  Function(const QDomElement &func);
  Function(const double coefs[5], const double red, const double green,
           const double blue, const int nonLin);
  bool operator()(double &x, double &y, double &r, double &g, double &b);

  QDomElement toXML(QDomDocument &doc) const;
};
#endif
