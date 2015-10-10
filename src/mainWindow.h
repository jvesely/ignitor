#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include "fractal.h"
#include <QMainWindow>
#include <QTabWidget>
#include <QProgressBar>



class mainWindow:public QMainWindow{

	Q_OBJECT
	
	public:
    mainWindow();

  private slots:
    void newFile();
    void openXML();
    void saveXML();
		void savePict();
		void startRender();
		void settingsOpen();
		void closeTab();
		void tabChanged();
		void progChanged();
		void stopRender();
	signals:
		void activate(fractal * active);

	private:
		void createActions();
		void createMenus();
    void createToolBar();
    void createStatusBar();
		void createTabs();

		void readSettings();
    void writeSettings();

		fractal * getCurrentFractal();

		QTabWidget *tabs;
		QProgressBar * _prog;
		QMenu *fileMenu;
    QMenu *fractalMenu;
    QToolBar *toolBar;
    QAction *newAct;
    QAction *openAct;
    QAction *saveXMLAct;
		QAction *savePictAct;
    QAction *exitAct;
		QAction *settingsAct;
		QAction *renderAct;
		QAction *closeTabAct;
		QAction *stopRenderAct;
};
#endif
