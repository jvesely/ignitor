#include <QtGui>
#include "mainWindow.h"
#include "optionDialog.h"
mainWindow::mainWindow() {

  setWindowIcon(QIcon(":images/icon.png"));
  setWindowTitle("Ignitor");

  createActions();
  createTabs();
  createMenus();
  createToolBar();
  createStatusBar();

  setCentralWidget(tabs);

  readSettings();
  newFile();
}
void mainWindow::createTabs() {
  tabs = new QTabWidget(this);

  connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(tabChanged()));

  QToolButton *newTabButton = new QToolButton(tabs);
  newTabButton->setCursor(Qt::ArrowCursor);
  newTabButton->setAutoRaise(true);
  newTabButton->setIcon(QIcon(":/images/document_16.png"));
  connect(newTabButton, SIGNAL(clicked()), this, SLOT(newFile()));
  newTabButton->setToolTip(tr("Add fractal"));
  tabs->setCornerWidget(newTabButton, Qt::TopLeftCorner);

  QToolButton *closeTabButton = new QToolButton(tabs);

  closeTabButton->setCursor(Qt::ArrowCursor);
  closeTabButton->setAutoRaise(true);
  closeTabButton->setIcon(QIcon(":/images/delete_16.png"));
  connect(closeTabButton, SIGNAL(clicked()), this, SLOT(closeTab()));
  closeTabButton->setToolTip(tr("Close page"));
  closeTabButton->setEnabled(false);
  // closeTabButton->setPalette(pal);
  tabs->setCornerWidget(closeTabButton, Qt::TopRightCorner);

  tabs->addAction(newAct);
  tabs->addAction(openAct);
  tabs->addAction(saveXMLAct);
  tabs->addAction(savePictAct);
  tabs->addAction(renderAct);
  tabs->addAction(stopRenderAct);
  tabs->addAction(settingsAct);
  tabs->addAction(closeTabAct);
  tabs->setContextMenuPolicy(Qt::ActionsContextMenu);
  tabs->repaint();
}
void mainWindow::readSettings() {
  QSettings settings("Student", "Ignitor");
  QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
  QSize size = settings.value("size", QSize(640, 480)).toSize();
  resize(size);
  move(pos);
}
void mainWindow::writeSettings() {
  QSettings settings("Student", "Ignitor");
  settings.setValue("pos", pos());
  settings.setValue("size", size());
}
void mainWindow::createActions() {
  newAct =
      new QAction(QIcon(":/images/document_16.png"), tr("New fractal"), this);
  newAct->setShortcut(tr("Ctrl+T"));
  newAct->setStatusTip(tr("Create a new fractal"));
  connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

  openAct =
      new QAction(QIcon(":/images/folder_16.png"), tr("&Open fractal"), this);
  openAct->setShortcut(tr("Ctrl+O"));
  openAct->setStatusTip(tr("Open saved fractal"));
  connect(openAct, SIGNAL(triggered()), this, SLOT(openXML()));

  saveXMLAct =
      new QAction(QIcon(":/images/save_16.png"), tr("&Save XML"), this);
  saveXMLAct->setShortcut(tr("Ctrl+S"));
  saveXMLAct->setStatusTip(tr("Save fractal"));
  connect(saveXMLAct, SIGNAL(triggered()), this, SLOT(saveXML()));

  savePictAct = new QAction(QIcon(":/images/icon.png"), tr("Save &Pict"), this);
  savePictAct->setShortcut(tr("Ctrl+P"));
  savePictAct->setStatusTip(tr("Save picture"));
  connect(savePictAct, SIGNAL(triggered()), this, SLOT(savePict()));

  renderAct = new QAction(QIcon(":/images/flag_16.png"), tr("&Render"), this);
  renderAct->setShortcut(tr("Ctrl+R"));
  renderAct->setStatusTip(tr("Start rendering"));
  connect(renderAct, SIGNAL(triggered()), this, SLOT(startRender()));

  settingsAct = new QAction(QIcon(":/images/gear_16.png"), tr("Options"), this);
  settingsAct->setShortcut(tr("Ctrl+A"));
  settingsAct->setStatusTip(tr("Alter Options"));
  connect(settingsAct, SIGNAL(triggered()), this, SLOT(settingsOpen()));

  closeTabAct =
      new QAction(QIcon(":/images/delete_16.png"), tr("Close Fractal"), this);
  closeTabAct->setShortcut(tr("Ctrl+W"));
  closeTabAct->setStatusTip(tr("Close Fractal"));
  connect(closeTabAct, SIGNAL(triggered()), this, SLOT(closeTab()));

  exitAct = new QAction(tr("E&xit"), this);
  exitAct->setShortcut(tr("Ctrl+X"));
  exitAct->setStatusTip(tr("Leave Ignitor"));
  connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

  stopRenderAct =
      new QAction(QIcon(":/images/block_16.png"), tr("S&top"), this);
  stopRenderAct->setShortcut(tr("Ctrl+T"));
  stopRenderAct->setStatusTip(tr("Stop Rendering"));
  connect(stopRenderAct, SIGNAL(triggered()), this, SLOT(stopRender()));
}
void mainWindow::createMenus() {
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(newAct);
  fileMenu->addAction(openAct);
  fileMenu->addAction(saveXMLAct);
  fileMenu->addAction(savePictAct);
  fileMenu->addSeparator();
  //	fileMenu->addAction(closeTabAct);
  fileMenu->addAction(exitAct);

  fractalMenu = menuBar()->addMenu(tr("&Fractal"));
  fractalMenu->addAction(renderAct);
  fractalMenu->addAction(stopRenderAct);
  fractalMenu->addAction(settingsAct);
}
void mainWindow::createToolBar() {
  toolBar = addToolBar(tr("File"));
  toolBar->addAction(newAct);
  toolBar->addAction(openAct);
  toolBar->addAction(saveXMLAct);
  toolBar->addAction(savePictAct);
  // toolBar->addAction(closeTabAct);
  toolBar->addSeparator();
  toolBar->addAction(settingsAct);
  toolBar->addAction(renderAct);
  toolBar->addAction(stopRenderAct);

  toolBar->setMovable(false);
  toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
}
void mainWindow::createStatusBar() {
  statusBar()->showMessage(tr("Ready"));
  _prog = new QProgressBar;
  _prog->hide();

  statusBar()->addPermanentWidget(_prog);
  connect(_prog, SIGNAL(valueChanged(int)), this, SLOT(progChanged()));
}
void mainWindow::openXML() {
  newFile();
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Fractal"), ".",
                                                  tr("XML files (*.xml)"));
  if (fileName.isEmpty()) {
    closeTab();
    return;
  }
  if (!((fractal *)tabs->currentWidget())->loadXML(fileName)) {
    QMessageBox::warning(this, tr("Loading failed!"),
                         tr("Ignitor was unable to load saved fractal!"));
    closeTab();
  } else {
    tabs->setTabText(tabs->currentIndex(), fileName);
    tabs->setTabToolTip(tabs->currentIndex(), fileName);
  }
}
void mainWindow::saveXML() {
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save Fractal"), ".",
                                                  tr("XML files (*.xml)"));
  if (fileName.isEmpty())
    return;
  tabs->setTabText(tabs->currentIndex(), fileName);
  tabs->setTabToolTip(tabs->currentIndex(), fileName);
  getCurrentFractal()->saveXML(fileName);
}
void mainWindow::savePict() {
  QString fileName = QFileDialog::getSaveFileName(
      this, tr("Save Fractal Picture"), ".", tr("Picture files (*.png *.jpg)"));
  if (fileName.isEmpty())
    return;
  if (!getCurrentFractal()->savePict(fileName))
    QMessageBox::warning(this, tr("Saving failed!"),
                         tr("Ignitor was unable to save fractal picture!"));
}

void mainWindow::newFile() {
  fractal *nextFr;
  nextFr = new fractal();
  nextFr->setAcceptDrops(true);
  tabs->addTab(nextFr, "new");
  connect(this, SIGNAL(activate(fractal *)), nextFr,
          SLOT(activated(fractal *)));
  connect(nextFr, SIGNAL(setMin(int)), _prog, SLOT(setMinimum(int)));
  connect(nextFr, SIGNAL(setMax(int)), _prog, SLOT(setMaximum(int)));
  connect(nextFr, SIGNAL(setCurrent(int)), _prog, SLOT(setValue(int)));
  connect(nextFr, SIGNAL(changed()), this, SLOT(tabChanged()));
  tabs->setCurrentIndex(tabs->count() - 1);
  tabChanged();
}
void mainWindow::settingsOpen() {
  OptionDialog *options = new OptionDialog(getCurrentFractal());
  delete options;
}
void mainWindow::startRender() {
  fractal *currentFr = getCurrentFractal();
  if (!currentFr->render())
    currentFr->write("Rendering Failed");
}
void mainWindow::stopRender() { getCurrentFractal()->stop(); }
void mainWindow::closeTab() {
  int i = tabs->currentIndex();
  if (i)
    --i;
  delete tabs->currentWidget();
  tabs->setCurrentIndex(i);
  if (!i)
    tabChanged();
}
void mainWindow::tabChanged() {

  bool enabled = false;

  if (tabs->count() > 0) {
    enabled = getCurrentFractal()->getRendered();
    emit activate(getCurrentFractal());
    stopRenderAct->setVisible(getCurrentFractal()->isRunning());
  }

  savePictAct->setEnabled(enabled);

  saveXMLAct->setEnabled(tabs->count() > 0);
  renderAct->setEnabled(tabs->count() > 0);
  closeTabAct->setEnabled(tabs->count() > 1);
  tabs->cornerWidget()->setEnabled(tabs->count() > 1);
  settingsAct->setEnabled(tabs->count() > 0);
}
void mainWindow::progChanged() {
  if (_prog->value() == 0)
    _prog->hide();
  else
    _prog->show();
}
fractal *mainWindow::getCurrentFractal() {
  return (fractal *)(tabs->currentWidget());
}
