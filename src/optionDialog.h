#ifndef _OPTION_DIALOG_H_
#define _OPTION_DIALOG_H_
#include <QDialog>
#include <QDialogButtonBox>
#include <QTabWidget>
#include "fractal.h"
#include "ui_mainOpt.h"
#include "ui_functionTab.h"

class FunctionTab : public QWidget, private Ui_functionTab {

  Q_OBJECT

  QColor colour;

public:
  FunctionTab(const Function &from);
  operator Function();
public slots:
  void changeColour();
};
class GeneralOptions : public QWidget, private Ui_mainOpt {
public:
  GeneralOptions(fractal *origin);
  void setFuncCount(int n);
  int getIterations();
  int getWidth();
  int getHeight();
};
class OptionDialog : public QDialog {

  Q_OBJECT

  fractal *myFractal;
  QTabWidget *tabs;
  GeneralOptions *mainOpt;

  QDialogButtonBox *buttonBox;
  QTabWidget *createTabs();

public:
  OptionDialog(fractal *par);
public slots:
  void remFunc();
  void addFunc();
  void tabChange(bool deleted);
  void accept();
};

#endif
