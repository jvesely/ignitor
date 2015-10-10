#include "mainWindow.h"
#include <QApplication>


int main(int argc, char **argv){
	Q_INIT_RESOURCE(ignitor);

  QApplication app(argc, argv);
  mainWindow mainWin;
  mainWin.show();
  return app.exec();
}
